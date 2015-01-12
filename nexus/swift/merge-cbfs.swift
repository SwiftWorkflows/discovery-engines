
/**
   MERGE-CBFS.SWIFT

   Run this from the top-level directory.
*/

import files;
import io;
import string;
import sys;

app merge_cbfs(string arguments[])
// data: I.e., /home/bessrc/sharedbigdata/data1/osborn-2014-1/bfap00
{
  // nxmerge -d f1 -p scan -e cbf -o f1
  "/home/wozniak/proj/d-e/nexus/bin/nxmerge.sh" arguments;
}

main
{
  data = argv("data");
  printf("merge-cbfs.swift in %s", data);

  file kts[] = glob(data/"kt*");

  foreach kt in kts
  {
    printf("kt: %s", filename(kt));
    file temperatures[] = glob(filename(kt)/"*K");
    foreach temperature in temperatures
    {
      printf("temperature: %s", filename(temperature));

      // Merge each scan:
      foreach f in [1:2]
      {
        directory = "%s/f%i"     % (filename(temperature), f);
        output    = "%s/f%i.nxs" % (filename(temperature), f);
        merge_cbfs(split("-d " + directory + " -o " + output +
                         " -p scan -e cbf",
                         " "));
      }
    }
  }
}
