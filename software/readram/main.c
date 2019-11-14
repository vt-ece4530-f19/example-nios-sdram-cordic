#include <system.h>
#include <stdio.h>
#include "sys/alt_timestamp.h"

#define ARRAYSIZE 10000

// volatile unsigned int __attribute__((section (".localmem"))) a[ARRAYSIZE];

volatile unsigned int a[ARRAYSIZE];

int main() {
  register unsigned int i;
  register int c;
  unsigned long ticks;
  unsigned long overhead;

  alt_timestamp_start();

  printf("Timer Frequency %lu\n", alt_timestamp_freq());

  for (i=0; i<9; i++) {
    ticks = alt_timestamp();
	  for (i = 0; i< ARRAYSIZE; i++)
  		;
    overhead = alt_timestamp() - ticks;
    printf("%5ld ", overhead);
  }
  printf("\n");

  printf("RAM write ticks: ");
  for (i=0; i<9; i++) {
    ticks = alt_timestamp();
    for (i = 0; i< ARRAYSIZE; i++)
  		a[i] = i;
    ticks = alt_timestamp() - ticks - overhead;
    printf("%5ld (per write %5ld)", ticks, ticks/ARRAYSIZE);
  }
  printf("\n");

  printf("RAM read ticks: ");
  for (i=0; i<9; i++) {
    ticks = alt_timestamp();
    for (i = 0; i< ARRAYSIZE; i++)
  		c += a[i];
    ticks = alt_timestamp() - ticks - overhead;
    printf("%5ld (per read %5ld)", ticks, ticks/ARRAYSIZE);
  }
  printf("\n");

  i = c;

  return 0;
}
