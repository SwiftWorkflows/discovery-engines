
import sys;
import io;
import files;

main
{
  string data = argv("data");
  file F[] = glob(data/"*.bin*");
  foreach f in F
  {
    printf("file: %s", filename(f));
  }
}
