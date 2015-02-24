
/**
   MERGE-ALL.SWIFT

   Run this from the top-level directory.
*/

import files;
import io;
import string;
import sys;

app merge_tiffs(string temperature_directory, string scan)
{
  "/home/wozniak/proj/d-e/nexus/bin/lsmo/nxmerge.sh"
    "-D" temperature_directory
    "-d" scan  "-e" ".tif" "-b" "darkbeg";
}

/*
app (file nxmax_out) nxmax(string directory, file nxs)
{
  "/home/wozniak/proj/d-e/nexus/bin/nxmax.sh" directory nxs nxmax_out;
}

app (file nxfind_out) nxfind(string directory, file nxs, file nxmax_out)
{
  "/home/wozniak/proj/d-e/nexus/bin/nxfind.sh" directory nxs nxmax_out nxfind_out;
}
*/

main
{
  data = argv("data");
  list = input(argv("list"));

  string work[] = file_lines(list);

  string scans[] = [ "ff1scan", "fb1scan",
                     "ff2scan", "fb2scan",
                     "sfscan",  "sbscan",
                     "ubfscan", "ubbscan" ];

  foreach directory in work
  {
    temperature_directory = data/directory;
    foreach scan in scans
    {
      // Merge each scan:
      merge_tiffs(temperature_directory, scan);

      /*
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
      */
    }
  }
}
