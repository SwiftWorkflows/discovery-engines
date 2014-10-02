
/*
 * Convert-Dbls2HDF.c
 *
 *  Created on: Oct 1, 2014
 *      Author: wozniak
 */

#include "config.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "checks.h"
#include "SharedFuncsFit.h"

#if HAVE_HDF5

#include <hdf5.h>
#include <H5Fpublic.h>

struct h5target
{
    hid_t file_id;
    hid_t dataset_id;
    hid_t dataspace_id;
};

bool
h5file_setup(const char *filename, const char *dataset,
             struct h5target *target)
{
    target->file_id = H5Fcreate(filename, H5F_ACC_TRUNC,
                               H5P_DEFAULT, H5P_DEFAULT);
    hsize_t dims[1];
    dims[0] = 32;
    target->dataspace_id = H5Screate_simple(1, dims, NULL);
    check_msg(target->dataspace_id > 0, "H5Screate_simple failed.")

    target->dataset_id = H5Dcreate(target->file_id, dataset, H5T_IEEE_F64BE,
                                   target->dataspace_id,
                                   H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    check_msg(target->dataspace_id > 0, "H5Screate2 failed.")
    return true;
}

void
h5file_close(struct h5target *target)
{
    herr_t status;
    status = H5Dclose(target->dataset_id);
    check_msg(status >= 0, "could not H5Dclose() dataset");
    status = H5Sclose(target->dataspace_id);
    check_msg(status >= 0, "could not H5Dclose() dataspace");
    status = H5Fclose(target->file_id);
    check_msg(status >= 0, "could not H5Dclose() file");
}

static inline bool
write_hdf(off_t p, int n, struct h5target *target, double* data)
{
  hsize_t count[1];
  count[0] = n;
  hsize_t offset[1];
  offset[0] = p;
  hsize_t* stride = NULL;
  hsize_t* block = NULL;

  hsize_t dimsm[3];
  dimsm[0] = n;
  hid_t memspace_id = H5Screate_simple(1, dimsm, NULL);

  hid_t dataspace_id = target->dataspace_id;
  hid_t dataset_id   = target->dataset_id;

  herr_t status;
  status = H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET,
                               offset, stride, count, block);
  check_msg(status >= 0, "H5Sselect_hyperslab() failed.")
  status = H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, memspace_id,
                    dataspace_id, H5P_DEFAULT, data);
  check_msg(status >= 0, "H5Dwrite() failed.")
  return true;
}

bool
Dbls2HDF(const char *input, const char *output, const char *dataset)
{
    FILE* fp_in = fopen(input, "r");
    if (fp_in == NULL) file_not_found(input);

    struct stat s;
    int rc = fstat(fileno(fp_in), &s);
    if (rc == -1) crash("Could not stat input file: %s\n", input);
    off_t size = s.st_size;

    int max_chunk = 10*1024*1024;
    off_t total = 0;
    void* buffer = malloc(max_chunk);

    struct h5target target;
    h5file_setup(output, dataset, &target);

    while (total < size)
    {
        int chunk = min_integer(max_chunk, size-total);
        READ(buffer, 1, chunk, fp_in);
        write_hdf(total/8, chunk/8, &target, buffer);
        total += chunk;
    }

    h5file_close(&target);

    free(buffer);
    fclose(fp_in);
    return true;
}

#endif // HAVE_HDF5

int
main(int argc, char* argv[])
{
    if (! HAVE_HDF5) crash("You do not have HDF5!");

#if HAVE_HDF5

    if (argc != 4)
        crash("requires 2 filenames and output dataset name!\n");

    char* input   = argv[1];
    char* output  = argv[2];
    char* dataset = argv[3];

    bool result = Dbls2HDF(input, output, dataset);
    if (!result) {
        printf("%s failed!\n", argv[0]);
        abort();
    }
#endif // HAVE_HDF5
    exit(EXIT_SUCCESS);
 }
