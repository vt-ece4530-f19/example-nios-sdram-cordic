#include <system.h>
#include <stdio.h>
#include "sys/alt_timestamp.h"

#define PI 843314856       
#define AG_CONST 163008218

// Note on <32,28> fixed point precision:
//
// <32,28> is a fixed point number with the weight of the lsb equal to 2^-28
// The weight of the msb in a <32,28> is -2^4 (two's complement).
// Thus, the value '1' is represented as (1 << 28)
// Similarly, the fixed point number (1 << 26) corresponds to the value 0.25
//
// This simple scaling allows fractional precision with integer calculations

static const int angles[] = {
  210828714,
  124459457,
   65760959,
   33381289,
   16755421,
    8385878,
    4193962,
    2097109,
    1048570,
     524287,
     262143,
     131071,
      65535,
      32767,
      16383,
       8191,
       4095,
       2047,
       1024,
        511 };

#define DATALEN 1000

int __attribute__((section (".localmem"))) target_angle[DATALEN];
int __attribute__((section (".localmem"))) result_X[DATALEN];
int __attribute__((section (".localmem"))) result_Y[DATALEN];

//int target_angle[DATALEN];
//int result_X[DATALEN];
//int result_Y[DATALEN];

void prepare_angle() {
  unsigned rnstate = 1;                                                          
  int i;                                                                       
  for (i=0; i<DATALEN; i++) {                                                
    rnstate = (rnstate >> 1) ^ (-(signed int)(rnstate & 1) & 0xd0000001u);     
    target_angle[i] = (int) rnstate % (PI/2);
  }                                                                            
}

void golden_cordic(int target, int *rX, int *rY) {
  int X, Y, T, current;
  unsigned step;
  X       = AG_CONST;  
  Y       = 0;	       
  current = 0;
  for(step=0; step < 20; step++) {
    if (target > current) {
      T          =  X - (Y >> step);
      Y          = (X >> step) + Y;
      X          = T;
      current   += angles[step];
    } else {
      T          = X + (Y >> step);
      Y          = -(X >> step) + Y;
      X          = T;
      current   -= angles[step]; 
    }
  }
  *rX = X;
  *rY = Y;
}

void hardware_cordic(int target, int *rX, int *rY) {
  ALT_CI_CORDICCI_0( 0, target, 0);
  *rX = ALT_CI_CORDICCI_0( 1, 0, 0);
  *rY = ALT_CI_CORDICCI_0( 2, 0, 0);
}

void check_result() {
  int X, Y;
  int dX, dY;
  int errors = 0;
  int i;
  int sum_abs_error = 0;
  for (i=0; i<DATALEN; i++) {                                                
    golden_cordic(target_angle[i], &X, &Y);
    dX = X - result_X[i];
    dY = Y - result_Y[i];
    if (dX) 
      errors++;
    if (dY) 
      errors++;
    sum_abs_error += (dX > 0) ? dX : -dX;
    sum_abs_error += (dY > 0) ? dY : -dY;
  }
  printf("Errors: %d\n", errors);
  printf("Sum_abs_error: %d\n", sum_abs_error);

}

int main(void) {
  unsigned i;
  unsigned long ticks, overhead;

  alt_timestamp_start();

  printf("Starting Cordic Measurement\n");
  ticks = alt_timestamp();
  for (i = 0; i< DATALEN; i++)
      ;
  overhead = alt_timestamp() - ticks;

  printf("Overhead correction: %ld\n", overhead);

  prepare_angle();

  ticks = alt_timestamp();
  for (i=0; i<DATALEN; i++)
    golden_cordic(target_angle[i], &(result_X[i]), &(result_Y[i]));
  ticks = alt_timestamp() - ticks - overhead;
  printf("Software Cordic Cycles: %ld\n", ticks);

  ticks = alt_timestamp();
  for (i=0; i<DATALEN; i++)
    hardware_cordic(target_angle[i], &(result_X[i]), &(result_Y[i]));
  ticks = alt_timestamp() - ticks - overhead;
  printf("Hardware Cordic Cycles: %ld\n", ticks);
  check_result();
  
  return 0; 
}
