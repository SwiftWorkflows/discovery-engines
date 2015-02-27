
import io;
import math;
import string;
import sys;

app (file o) transform_sh(file i)
{
  "./transform.sh" i o;
}

app (file o) merge_sh(file i, file j)
{
  "./merge.sh" i j o;
}

(file o) merge(file d[], int start, int stop)
{
  if (stop-start == 1)
  {
    o = M(d[start], d[stop]);
  }
  else
  {
    n = stop-start;
    s = n %/ 2; // integer divide
    o = M(merge(d, start,     start+s),
          merge(d, start+s+1, stop));
  }
}


nxs = input(argv("nxs"));
tasks = round(2**n);
file d[];
foreach i in [0:tasks-1]
{
  d[i] = transform_sh(a);
}
file final <"final.data"> = merge(d, 0, tasks-1);
