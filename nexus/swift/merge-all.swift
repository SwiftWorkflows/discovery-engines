
/**
   MERGE-ALL.SWIFT

   Run this from the top-level directory.
*/

import files;
import io;
import string;
import sys;

app (file background) merge_tiffs_background(string directory,
                                             string arguments[])
{
  "/home/wozniak/proj/d-e/bin/merge-tiffs.sh" background
    "-d" directory arguments;
}

app (file nxs) merge_tiffs(string directory, file background,
                           string arguments[])
{
  "/home/wozniak/proj/d-e/bin/merge-tiffs.sh" background
    "-d" directory arguments;
}

app (file nxmax_out) nxmax(string directory, file nxs)
{
  "/home/wozniak/proj/d-e/bin/nxmax.sh" directory nxs nxmax_out;
}

app (file nxfind_out) nxfind(string directory, file nxs, file nxmax_out)
{
  "/home/wozniak/proj/d-e/bin/nxfind.sh" directory nxs nxmax_out nxfind_out;
}

main
{
  list = argv("list");
  data = argv("data");
  printf("merge-all.swift in %s", data);

  string work[] = split(read(input(list)), "\n");

  foreach directory in work
  {
    if (directory != "")
    {
      // Create background NXS:
      file background<data/directory/"darkbeg.nxs"> =
        merge_tiffs_background(directory, split("-p darkbeg -c", " "));

      // Merge each scan:
      merge_tiffs(directory, background,
                  split("-p fb1scan -b darkbeg -o 0 -s 0.1 -r -c", " "));
      merge_tiffs(directory, background,
                  split("-p ff1scan -b darkbeg -o 0 -s 0.1 -c", " "));
      merge_tiffs(directory, background,
                  split("-p ff2scan -b darkbeg -o 0 -s 0.1 -c", " "));
      merge_tiffs(directory, background,
                  split("-p fb2scan -b darkbeg -o 0 -s 0.1 -r -c", " "));
      merge_tiffs(directory, background,
                  split("-p sfscan -b darkbeg -o 0 -s 0.1 -c", " "));
      merge_tiffs(directory, background,
                  split("-p sbscan -b darkbeg -o 0 -s 0.1 -r -c", " "));

      // Merge the UB scans:
      file ubf_nxs<data/directory/"ubfscan.nxs"> =
        merge_tiffs(directory, background,
                    split("-p ubfscan -b darkbeg -o 0 -s 0.1 -c", " "));
      file ubb_nxs<data/directory/"ubbscan.nxs"> =
        merge_tiffs(directory, background,
                    split("-p ubbscan -b darkbeg -o 0 -s 0.1 -r -c", " "));

      // NXmax:
      file ubf_nxmax_out<data/directory/"ubf.nxmax.out"> =
         nxmax(directory, ubf_nxs);
      file ubb_nxmax_out<data/directory/"ubb.nxmax.out"> =
        nxmax(directory, ubb_nxs);

      // NXfind:
       file ubf_nxfind_out<data/directory/"ubf.nxfind.out"> =
         nxfind(directory, ubf_nxs, ubf_nxmax_out);
      file ubb_nxfind_out<data/directory/"ubb.nxfind.out"> =
        nxfind(directory, ubb_nxs, ubb_nxmax_out);
    }
  }
}
