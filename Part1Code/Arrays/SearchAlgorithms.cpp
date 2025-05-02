#include "SearchAlgorithms.h"
#include <cstring>

//binary search on transactions
int binarySearch(char SortedArray[][TransactionFields][FieldLength],int  size,const char* target,int  column) {
    int l=0,r=size-1;
    
    while(l<=r) {
        int m=(l+r)/2;
        int cmp=strcmp(SortedArray[m][column],target);
        if(cmp<0)      
            l=m+1;
        else if(cmp>0) 
            r=m-1;
        else           
            return m;
    }
    return -1;
}

double percentageWithCategoryAndMethod(char SortedArray[][TransactionFields][FieldLength],int  size,const char* category,const char* paymentMethod) {
    
    int idx=binarySearch(SortedArray,size,category,2);
    if(idx<0) return 0.0;
    int total=0, match=0;
    // Scan backwards
    for(int i=idx; i>=0 && strcmp(SortedArray[i][2],category)==0; --i){
        if(strcmp(SortedArray[i][5],paymentMethod)==0) 
            ++match;
            ++total;
    }
    // Scan forwards
    for(int i=idx+1; i<size && strcmp(SortedArray[i][2],category)==0; ++i){
        if(strcmp(SortedArray[i][5],paymentMethod)==0) ++match;
        ++total;
    }

    return total ? (double)match/total*100.0 : 0.0;
}

//binary search on reviews
int binarySearchReviews(char SortedReviewArray[][ReviewFields][FieldLength],int  size,const char* target,int  column) {
    int l = 0, r = size - 1;
    while (l <= r) {
        int m   = (l + r) / 2;
        int cmp = strcmp(SortedReviewArray[m][column], target);
        if      (cmp <  0) l = m + 1;
        else if (cmp >  0) r = m - 1;
        else               return m;
    }
    return -1;
}

//Linear search
int linearSearch(char** array, int n, const char* target) {
    for (int i = 0; i < n; ++i) {
        if (strcmp(array[i], target) == 0) {
            return i;
        }
    }
    return -1;
}
int linearSearch(char array[][FieldLength], int n, const char* target) {
    for (int i = 0; i < n; ++i) {
        if (strcmp(array[i], target) == 0)
            return i;
    }
    return -1;
}
