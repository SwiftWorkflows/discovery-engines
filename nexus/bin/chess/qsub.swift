
import io;
import files;
import string;
import sys;

app qsub(string line[])
{
  "qsub-block" line;
}

list = argp(1);

printf(list);

string lines[] = file_lines(input(list));
foreach line in lines
{
  qsub(split(line, " "));
}
