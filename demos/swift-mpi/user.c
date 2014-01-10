
#include <assert.h>
#include <stdio.h>

#include <mpi.h>

double
mpi_merge(long long int task_comm_int, char* filename)
{
  MPI_Comm task_comm = (MPI_Comm) task_comm_int;

  int task_rank;
  int task_size;
  MPI_Comm_rank(task_comm, &task_rank);
  MPI_Comm_size(task_comm, &task_size);

  // Read my value
  double value;
  MPI_File fh;
  MPI_Offset offset = task_rank*sizeof(double);
  MPI_Status status;
  MPI_File_open(task_comm, filename, MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
  MPI_File_read_at_all(fh, offset, &value, 1, MPI_DOUBLE, &status);
  MPI_File_close(&fh);
  printf("task: %i/%i: value: %0.1f\n", task_rank, task_size, value);

  // Do MPI_Reduce to obtain result
  double result;
  MPI_Reduce(&value, &result, 1, MPI_DOUBLE, MPI_SUM, 0, task_comm);

  // Return result
  if (task_rank != 0)
    // Return dummy value
    return 0.0;
  result /= task_size;
  printf("MPI_Reduce average result: %0.1f\n", result);
  return result;
}

double
f(double x, double y)
{
  return 2*x + 3*y;
}
