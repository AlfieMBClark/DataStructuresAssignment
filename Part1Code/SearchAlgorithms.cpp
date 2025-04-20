#include "SearchAlgorithms.h"
#include <cstring>

int binarySearch(char InputedArray[][TransactionFields][FieldLength],int  size,const char* target,int  column) {
    int l=0,r=size-1;
    
    while(l<=r) {
        int m=(l+r)/2;
        int cmp=strcmp(InputedArray[m][column],target);
        if(cmp<0)      
            l=m+1;
        else if(cmp>0) 
            r=m-1;
        else           
            return m;
    }
    return -1;
}

double percentageWithCategoryAndMethod(char InputedArray[][TransactionFields][FieldLength],int  size,const char* category,const char* paymentMethod) {
    
    int idx=binarySearch(InputedArray,size,category,2);
    if(idx<0) return 0.0;
    int total=0, match=0;

    for(int i=idx; i>=0 && strcmp(InputedArray[i][2],category)==0; --i){
        if(strcmp(InputedArray[i][5],paymentMethod)==0) 
            ++match;
            ++total;
    }

    for(int i=idx+1; i<size && strcmp(InputedArray[i][2],category)==0; ++i){
        if(strcmp(InputedArray[i][5],paymentMethod)==0) ++match;
        ++total;
    }

    return total ? (double)match/total*100.0 : 0.0;
}
