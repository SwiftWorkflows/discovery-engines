
/**
   MERGE-CBFS.SWIFT

   Work with absolute paths
*/

import files;
import io;
import string;
import sys;

app (file o) merge_cbfs(string arguments[])
{
  // nxmerge -d f1 -p scan -e cbf -o f1
  "/home/wozniak/proj/d-e/nexus/bin/nxmerge.sh" arguments;
}

main
{
  // data: I.e., /home/bessrc/sharedbigdata/data1/osborn-2014-1/bfap00
  data = argv("data");
  printf("merge-cbfs.swift in %s", data);

  file label_dirs[] = glob(data/"*");

  foreach label in label_dirs
  {
    printf("label directory: %s", filename(label));
    file temperatures[] = glob(filename(label)/"*K");
    foreach temperature in temperatures
    {
      printf("temperature: %s", filename(temperature));

      file f1;
      file f2;
      // Merge each scan:
      directory1 = "%s/f1"     % (filename(temperature));
      output1    = "%s/f1.nxs" % (filename(temperature));
      directory2 = "%s/f2"     % (filename(temperature));
      output2    = "%s/f2.nxs" % (filename(temperature));
      merge_cbfs(split("-d " + directory1 +
                       " -o " + output1 +
                       " -p scan -e cbf", " "));
      merge_cbfs(split("-d " + directory2 +
                       " -o " + output2 +
                       " -p scan -e cbf", " "));
    }
  }
}
