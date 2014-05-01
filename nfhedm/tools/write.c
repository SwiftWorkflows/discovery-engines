
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

int main()
{
  float t;
  fscanf(stdin, "%f", &t);
  int32_t i;
  memcpy(&i, &t, 4);
  i = htonl(i);
  fwrite(&i, sizeof(float), 1, stdout);
  return 0;
}
