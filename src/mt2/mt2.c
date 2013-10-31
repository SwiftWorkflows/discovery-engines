
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <hdf5.h>

#define MAX_FILENAME (64*1024)

void
usage(void)
{
  printf("usage: mt2 x y z output_file < input_file\n");
}

void check(int condition, char* msg)
{
  if (!condition)
  {
    printf("mt2: %s\n", msg);
    exit(1);
  }
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
  count += sprintf(p, "error: ");
  count += vsnprintf(buffer+count, (size_t)(buffer_size-count), format, ap);
  va_end(ap);
  printf("%s\n", buffer);
  fflush(NULL);
  exit(1);
}

/** Called when the check_msg() condition fails */
static void check_msg_impl(const char* format, ...);

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
init_output(char* filename, hid_t* file_id)
{
  printf("opening: %s ...\n", filename);
  *file_id = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

  return true;
}

static bool
next_file(char* filename)
{
  char* result = fgets(filename, MAX_FILENAME, stdin);
  if (result == NULL)
    return false;
  return true;
}

static bool
rw_loop(int z)
{
  int count = 0;
  char current_nxs[MAX_FILENAME];
  printf("z: %d\n", z);
  while (true)
  {
    check_msg(count <= z, "input file count exceeds z!")
    bool rc = next_file(current_nxs);
    if (!rc) break;
    printf("reading: %s ...\n", current_nxs);
    count++;
  }
  return true;
}

int
main(int argc, char* argv[])
{
  char input_filename[MAX_FILENAME];
  char* output_filename;
  int x, y, z;
  hid_t file_id;

  bool rc;

  rc = scan_args(argc, argv, &x, &y, &z, &output_filename);
  check(rc, "error in arguments!");

  rc = init_output(output_filename, &file_id);
  check_msg(rc, "could not write to: %s\n", output_filename);

  rc = rw_loop(z);

  return EXIT_SUCCESS;
}

