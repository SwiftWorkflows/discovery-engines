
import io;
import sys;

(float z) placebo_1(float x, float y) "tclfop" "0.0"
[ "set <<z>> [ placebo_1 <<x>> <<y>> ]" ];

main
{
  start = toint(argp(1));
  end   = toint(argp(2));
  printf("HI: %s %i %i", argv("p"), start, end);
}

