
import io;

(float z) placebo_1(float x, float y) "tclnfhedm" "0.0"
[ "set <<z>> [ placebo_1 <<x>> <<y>> ]" ];

main
{
  printf("HI");
  float z;
  x = 1.5;
  y = 2.5;
  z = placebo_1(x, y);
  printf("swift: z=%0.2f", z);
}

