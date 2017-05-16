#include <iostream>  
#include <map>  
#include <algorithm>  

#include <cstdio>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

#include "pcg_basic.h"

using namespace std;  
  
unsigned int rand_int(){  
    unsigned int result=0;  
    for(unsigned int i=0;i<sizeof(int);++i){  
        result=(result<<8)+rand()%256;  
    }  
    return result;  
}  
  
int rand_range(int lower, int upper){    
    if(lower>upper){ // Swap if lower>upper    
        swap(lower,upper);  
    }  
    return rand_int()%(upper-lower+1)+lower;    
}  
  
int GenerateRandomNumbers(int* result, int lower, int upper, int count){  
      
    if(lower>upper){ // Swap if lower>upper  
        swap(lower,upper);  
    }  
      
    if(count>upper-lower+1){ // It is impossible to generate the array  
        return 0;  
    }  
      
    int diff=upper-lower;         
      
    for(int i=0;i<=count;++i){ // Initialize the array with the first numbers  
        result[i]=i+lower;  
    }  
      
    map<int,int> number_map;  
      
    for(int i=0;i<=count;++i){  
        int index=rand_range(i,diff);  
        if(index<=count){ // The index is in the array, so swap the items  
            swap(result[i],result[index]);  
        }  
        else{  
            map<int,int>::iterator it=number_map.find(index+lower);  
            if(it!=number_map.end()){ // Lookup the number_map  
                swap(it->second,result[i]);  
            }  
            else{  
                number_map.insert(make_pair(index+lower,result[i])); // Add the item into the number_map  
                result[i]=index+lower;  
            }  
        }  
    }  
      
    return count;  
}  
  
//int main(){ 


int main(int argc, char** argv){
    int rounds = 50;
    bool nondeterministic_seed = false;
    int round, i;

    ++argv;
    --argc;
    if (argc > 0 && strcmp(argv[0], "-r") == 0) {
        nondeterministic_seed = true;
        ++argv;
        --argc;
    }
    if (argc > 0) {
        rounds = atoi(argv[0]);
    }

    // In this version of the code, we'll use a local rng, rather than the
    // global one.

    pcg32_random_t rng;

    // You should *always* seed the RNG.  The usual time to do it is the
    // point in time when you create RNG (typically at the beginning of the
    // program).
    //
    // pcg32_srandom_r takes two 64-bit constants (the initial state, and the
    // rng sequence selector; rngs with different sequence selectors will
    // *never* have random sequences that coincide, at all) - the code below
    // shows three possible ways to do so.

    if (nondeterministic_seed) {
        // Seed with external entropy -- the time and some program addresses
        // (which will actually be somewhat random on most modern systems).
        // A better solution, entropy_getbytes, using /dev/random, is provided
        // in the full library.
        
        pcg32_srandom_r(&rng, time(NULL) ^ (intptr_t)&printf, 
			      (intptr_t)&rounds);
    } else {
        // Seed with a fixed constant

        pcg32_srandom_r(&rng, 42u, 54u);
    }

    for (round = 1; round <= rounds; ++round) {
     //   printf("Round %d:\n", round);
        /* Make some 32-bit numbers */
     //   printf("  32bit:");
     //   for (i = 0; i < 6; ++i)
     //       printf(" 0x%08x", pcg32_random_r(&rng));
     //   printf("\n");

        /* Toss some coins */
     //   printf("  Coins: ");
     //   for (i = 0; i < 65; ++i)
     //       printf("%c", pcg32_boundedrand_r(&rng, 2) ? 'H' : 'T');
     //   printf("\n");

        /* Roll some dice */
     //   printf("  Rolls:");
        for (i = 0; i < 30; ++i) {
            printf(" %2d", (int)pcg32_boundedrand_r(&rng, 400) + 1);
        }
        printf("\n");


  //  srand(time(0));  
    int result[200];  
    // Generate 10 random numbers within the range 1 to 100  
    GenerateRandomNumbers(result,1,400,200);  
  
/*    for(int i=0;i<200;++i)  
        cout<<result[i]<<endl;  
    return 0; */ 
}
}
