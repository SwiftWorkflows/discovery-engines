
import io;
import files;
import string;
import sys;

app qsub(string line[])
{
  "qsub-block" line;
}

app (void o)
cctw(string directory, string jobname, string command, int threads)
{
  "qsub-block" "-j" threads directory jobname command;
}

app (void o)
cctw_merge(string directory, string jobname, string a)
{
  "qsub-block" "-j" 1 directory jobname a;
}

S = 8; // Number of partial CCTWs
threads = 12;

P = "sm2ru3ge5-2/db_0073b_1/150K";
fn = 1;
  
int A[];
foreach i in [0:S-1]
{
  command = sprintf(
    "cctw transform " +
    "--script %s/f1_transform.pars " +
    "%s/f%i.nxs\\#/entry/data/data " +
    "-o %s/f%i_transform-%i.nxs\\#/entry/data " +
    "--normalization 0 " +
    "-S %i/%i -j %i",
    P, P, fn, P, fn, i, i, S, threads);
  void o = cctw(getenv("PWD"), "cctw-%i"%i,
                command,
                threads);
  o => A[i] = 0;
}

wait deep (A)
{
  cctw_merge(getenv("PWD"), "cctw-merge.zsh", "sm2ru3ge5-2/db_0073b_1/150K");
}
