#include <stdio.h>

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

void golden_cordic(int target, int *rX, int *rY) {
  int X, Y, T, current;
  unsigned step;
  X       = AG_CONST;  
  Y       = 0;	       
  current = 0;
  for(step=0; step < 20; step++) {
    printf("%8x %8x %8x\n", X, Y, current);
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
  printf("%8x %8x %8x\n", X, Y, current);
  *rX = X;
  *rY = Y;
}

int main(void) {

  int X, Y;
  golden_cordic(16000, &X, &Y);
  return 0;
}
