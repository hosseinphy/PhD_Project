#include <iostream>  
#include <map>  
#include <algorithm>  
  
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
  
int main(){  
    srand(time(0));  
    int result[200];  
    // Generate 10 random numbers within the range 1 to 100  
    GenerateRandomNumbers(result,1,400,200);  
  
    for(int i=0;i<200;++i)  
        cout<<result[i]<<endl;  
    return 0;  
}
