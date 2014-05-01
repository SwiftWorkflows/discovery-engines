
#include <stdio.h>

int main()
{
  float t;
  fread(&t, sizeof(float), 1, stdin);
  printf("%0.3f\n", t);
  return 0;
}
