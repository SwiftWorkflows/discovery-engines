
import io;
import math;
import string;
import sys;

app (file o) transform_sh(file nxs, int i)
{
  "./transform.sh" o M N nxs i;
}

app (file o) merge_sh(file i, file j)
{
  "./merge.sh" o i j;
}

(file o) merge(file d[], int start, int stop)
{
  if (stop-start == 0)
  {
    o = d[start];
  }
  else if (stop-start == 1)
  {
    o = merge_sh(d[start], d[stop]);
  }
  else
  {
    n = stop-start;
    s = n %/ 2; // integer divide
    o = merge_sh(merge(d, start,     start+s),
                 merge(d, start+s+1, stop));
  }
}

global const int M = 10;
global const int N = 10;

main
{
  file init_nxs = input(argv("init"));
  file final_nxs<argv("final")>;
  int tasks =  M*N;
  file chunks[];
  foreach i in [0:tasks-1]
  {
    file chunk<"chunks/%i.nxs"%i> = transform_sh(init_nxs, i);
    chunks[i] = chunk;
  }
  final_nxs = merge(chunks, 0, tasks-1);
}
