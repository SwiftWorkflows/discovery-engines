
import io;
import files;
import string;
import sys;

app (file o) convert(file i)
{
  "convert-bin-l2n" @i @o;
}

main
{
  string data = argv("data");
  string reduced = argv("reduced");
  file F[] = glob(data/reduced/"*bin*");
  foreach f in F
  {
    printf("file: %s", filename(f));
    s = replace(filename(f), data, data/"converted", 0);
    printf(s);
    file o<s> = convert(f);
  }
}
