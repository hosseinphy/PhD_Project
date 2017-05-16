// test program mttest.cpp, see mtreadme.txt for information
#include "mtrand.h"
#include <cstdio>

int main() {
  //long s=1243564124;
  unsigned long s = 89UL;
  //unsigned long init[4] = {0x123, 0x234, 0x345, 0x456}, length = 4;
  //MTRand_int32  irand(init, length); 
  MTRand_int32  mt(s);
// 32-bit int generator
// this is an example of initializing by an array
// you may use MTRand(seed) with any 32bit integer
// as a seed for a simpler initialization
  MTRand drand; // double in [0, 1) generator, already init

// generate the same numbers as in the original C test program
  std::printf("1000 32-bit integer random numbers:\n");
  for (int i = 0; i < 50; ++i) {

//    std::printf("%2u ", irand());
    std::printf("%10u", mt()%400);
    /*if ((i % 5) == 4)*/ std::printf("\n");
  }
 // std::printf("\n1000 random numbers in [0, 1):\n");
 // for (int i = 0; i < 1000; ++i) {
 //   std::printf("%10.8f ", drand());
 //   if ((i % 5) == 4) std::printf("\n");
 // }
}
