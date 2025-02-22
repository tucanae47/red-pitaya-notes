#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <sys/mman.h>

int main(int argc, char *argv[])
{
  int fd, i;
  volatile void *cfg, *sts;
  volatile int32_t *a, *b;
  volatile int32_t *p;
  char *end;
  long number;
  int32_t n[2], result;

  for(i = 0; i < 2; ++i)
  {
    errno = 0;
    number = (i < argc - 1) ? strtol(argv[i + 1], &end, 10) : 0;
    if(argc != 3 || errno != 0 || end == argv[i + 1])
    {
      printf("Usage: test a b\n");
      return EXIT_FAILURE;
    }
    n[i] = number;
    printf("0x%08x %d\n", n[i], n[i]);
  }

  if((fd = open("/dev/mem", O_RDWR)) < 0)
  {
    fprintf(stderr, "Cannot open /dev/mem.\n");
    return EXIT_FAILURE;
  }

  sts = mmap(NULL, sysconf(_SC_PAGESIZE), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0x40000000);
  cfg = mmap(NULL, sysconf(_SC_PAGESIZE), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0x40001000);

  a = cfg + 0;
  b = cfg + 4;

  p = sts;

  *a = n[0];
  *b = n[1];

  result = *p;

  if(result >> 23) result |= 0xff << 24;

  printf("0x%08x %d\n", result, result);

  return EXIT_SUCCESS;
}
