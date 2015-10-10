
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <hdf5.h>
#include <tiffio.h>

#include "util.h"

#define MAX_NAME     (64*1024)
#define MAX_FILENAME MAX_NAME

#define pixel int32_t
  // uint32_t

double profile_read  = 0;
double profile_write = 0;

char dataset_name[MAX_NAME];

void
usage(void)
{
  printf("usage: mt2 x y z output_file < input_file\n");
}

static void
set_defaults(void)
{
  strcpy(dataset_name, "/entry");
}

static void
check_msg_impl(const char* format, ...)
{
  int buffer_size = 10240;
  char buffer[buffer_size];
  int count = 0;
  char* p = &buffer[0];
  va_list ap;
  va_start(ap, format);
  count += sprintf(p, "mt2: error: ");
  count += vsnprintf(buffer+count, (size_t)(buffer_size-count),
                     format, ap);
  va_end(ap);
  printf("%s\n", buffer);
  fflush(NULL);
  exit(1);
}

/** Nice vargs error check and message */
#define check_msg(condition, format, args...)  \
    { if (!(condition))                          \
       check_msg_impl(format, ## args);        \
    }

#define report_bad_int(s) { \
    printf("could not parse as int: %s\n", s); \
    return false; \
}

bool
scan_args(int argc, char** argv,
          int* x, int* y, int* z, char** output_filename)
{
  if (argc != 5)
  {
    usage();
    return false;
  }

  int n;
  n = sscanf(argv[1], "%d", x);
  if (n != 1) report_bad_int(argv[1]);
  sscanf(argv[2], "%d", y);
  if (n != 1) report_bad_int(argv[2]);
  sscanf(argv[3], "%d", z);
  if (n != 1) report_bad_int(argv[3]);

  printf("dimensions: %d %d %d\n", *x, *y, *z);

  *output_filename = argv[4];
  return true;
}

static bool
init_output(int x, int y, int z, char* filename,
            hid_t* file_id, hid_t* dataset_id, hid_t* dataspace_id)
{
  printf("opening: %s ...\n", filename);
  *file_id = H5Fcreate(filename, H5F_ACC_TRUNC,
                       H5P_DEFAULT, H5P_DEFAULT);

  hsize_t dims[3];
  dims[0] = z;
  dims[1] = y;
  dims[2] = x;
  hsize_t  cdims[3];
  cdims[0] = 1;
  cdims[1] = 1000;
  cdims[2] = 1000;
  
  *dataspace_id = H5Screate_simple(3, dims, NULL);
  check_msg(dataspace_id > 0, "H5Screate_simple failed.");

  hid_t plist = H5Pcreate(H5P_DATASET_CREATE);
  H5Pset_chunk(plist, 3, cdims);
  H5Pset_deflate(plist, 1);
    
  *dataset_id = H5Dcreate2(*file_id, dataset_name, H5T_STD_I32LE,
                           *dataspace_id,
                           H5P_DEFAULT, plist, H5P_DEFAULT);
  check_msg(dataspace_id > 0, "H5Screate2 failed.");
  H5Pclose(plist);
  return true;
}

static inline void
chomp(char* s)
{
  char* c = strchr(s, '\n');
  if (c != NULL)
    *c = '\0';
}

static bool
next_file(char* filename)
{
  char* result = fgets(filename, MAX_FILENAME, stdin);
  if (result == NULL)
    return false;
  chomp(filename);
  return true;
}

static bool
read_tiff(char* filename, int x, int y, int k, pixel* data)
{
  TIFFSetWarningHandler(NULL);
  TIFF* tiff = TIFFOpen(filename, "r");
  check_msg(tiff != NULL, "could not open: %s", filename);

  int width, height;

  TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH,  &width);
  TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, &height);
  check_msg(width  == x, "TIFF width(%d) != x(%d)",  width,  x);
  check_msg(height == y, "TIFF height(%d) != y(%d)", height, y);

  int size = TIFFScanlineSize(tiff);
  // printf("scanline size: %i\n", size); 
  check_msg(size == x*sizeof(pixel), "scanlize size error!");

  pixel* p = data;
  for (uint32 j = 0; j < y; j++, p += x)
  {
    int rc = TIFFReadScanline(tiff, p, j, 0);
    check_msg(rc == 1, "TIFFReadScanline error!");
  }
  
  TIFFClose(tiff);
  return true;
}

static bool
write_hdf(int x, int y, int k,
          hid_t dataset_id, hid_t dataspace_id, pixel* data)
{
  hsize_t count[3];
  count[0] = 1;
  count[1] = y;
  count[2] = x;
  hsize_t offset[3];
  offset[0] = k;
  offset[1] = 0;
  offset[2] = 0;
  hsize_t* stride = NULL;
  hsize_t* block = NULL;

  hsize_t dimsm[3];
  dimsm[0] = 1;
  dimsm[1] = y;
  dimsm[2] = x;
  hid_t memspace_id = H5Screate_simple(3, dimsm, NULL);

  herr_t status;
  status = H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET,
                               offset, stride, count, block);
  check_msg(status >= 0, "H5Sselect_hyperslab() failed.")
  status = H5Dwrite(dataset_id, H5T_STD_I32LE, memspace_id,
                    dataspace_id, H5P_DEFAULT, data);
  check_msg(status >= 0, "H5Dwrite() failed.")
  return true;
}

static bool
rw_loop(int x, int y, int z, hid_t dataset_id, hid_t dataspace_id)
{
  double start, stop;
  char current_nxs[MAX_FILENAME];
  pixel* data = malloc(x*y * sizeof(pixel));
  int k = 0;
  while (true)
  {
    check_msg(k <= z, "input file count exceeds z!")
    bool rc = next_file(current_nxs);
    if (!rc) break;
    printf("reading: %s ...\n", current_nxs);
    fflush(stdout);
    start = time_double();
    read_tiff(current_nxs, x, y, k, data);
    stop = time_double();
    profile_read += stop-start;
    start = time_double();
    write_hdf(x, y, k, dataset_id, dataspace_id, data);
    stop = time_double();
    profile_write += stop-start;
    k++;
  }
  free(data);
  return true;
}

static void
close_all(hid_t file_id, hid_t dataset_id, hid_t dataspace_id)
{
  herr_t status;
  status = H5Dclose(dataset_id);
  check_msg(status >= 0, "could not H5Dclose() dataset");
  status = H5Sclose(dataspace_id);
  check_msg(status >= 0, "could not H5Dclose() dataspace");
  status = H5Fclose(file_id);
  check_msg(status >= 0, "could not H5Dclose() file");
}

static bool
scan_opts(int argc, char** argv,
          bool* use_existing_ds)
{
  // Defaults:
  *use_existing_ds = false;

  int opt;
  while ((opt = getopt(argc, argv, "ud:")) != -1)
  {
    switch (opt)
    {
      case 'u':
        *use_existing_ds = true;
        break;
      case 't':
        strcpy(dataset_name, optarg);
        break;
    }
  }
  return true;
}

int
main(int argc, char* argv[])
{
  char* output_filename;
  int x, y, z;
  hid_t file_id, dataset_id, dataspace_id;

  set_defaults();
  
  bool rc;
  bool use_existing_ds;
  rc = scan_opts(argc, argv, &use_existing_ds);
  check_msg(rc, "error in arguments!");
  rc = scan_args(argc, argv, &x, &y, &z, &output_filename);
  check_msg(rc, "error in arguments!");

  rc = init_output(x, y, z, output_filename,
                   &file_id, &dataset_id, &dataspace_id);
  check_msg(rc, "could not write to: %s\n", output_filename);

  rc = rw_loop(x, y, z, dataset_id, dataspace_id);

  close_all(file_id, dataset_id, dataspace_id);

  printf("done.\n");
  printf("profile_read:  %0.3f\n", profile_read);
  printf("profile_write: %0.3f\n", profile_write);
  return EXIT_SUCCESS;
}

