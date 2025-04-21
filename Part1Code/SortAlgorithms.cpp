#include "SortAlgorithms.h"
#include <cstring>

static TransactionRow buffer[Rows];

void mergeSort(TransactionRow EntireArray[], int leftSide, int rightSide, int column) {
    if (leftSide >= rightSide) return;
    int mid = (leftSide + rightSide) / 2;

    mergeSort(EntireArray, leftSide,  mid, column);
    mergeSort(EntireArray, mid+1,rightSide, column);

    int i = leftSide, j = mid+1, k = leftSide;
    while (i <= mid && j <= rightSide) {
        if (strcmp(EntireArray[i][column], EntireArray[j][column]) <= 0)
            memcpy(buffer[k++], EntireArray[i++], sizeof(TransactionRow));
        else
            memcpy(buffer[k++], EntireArray[j++], sizeof(TransactionRow));
    }
    while (i <= mid)   memcpy(buffer[k++], EntireArray[i++], sizeof(TransactionRow));
    while (j <= rightSide) memcpy(buffer[k++], EntireArray[j++], sizeof(TransactionRow));

    for (int idx = leftSide; idx <= rightSide; ++idx)
        memcpy(EntireArray[idx], buffer[idx], sizeof(TransactionRow));
}

static ReviewRow reviewBuffer[Rows];
void mergeSortReviews(ReviewRow entireArray[],int leftSide,int rightSide,int column){

    if (leftSide >= rightSide) return;

    int mid = (leftSide + rightSide) / 2;
    mergeSortReviews(entireArray, leftSide,  mid, column);
    mergeSortReviews(entireArray, mid+1,   rightSide, column);

    int i = leftSide,      
    j = mid + 1,       
    k = leftSide;     

    while (i <= mid && j <= rightSide) {
        if (strcmp(entireArray[i][column],entireArray[j][column]) <= 0){
            memcpy(reviewBuffer[k++],entireArray[i++],sizeof(ReviewRow));
        } else {
            memcpy(reviewBuffer[k++],entireArray[j++],sizeof(ReviewRow));
        }
    }
   
    while (i <= mid) {
        memcpy(reviewBuffer[k++],
        entireArray[i++],
        sizeof(ReviewRow));
    }
    while (j <= rightSide) {
        memcpy(reviewBuffer[k++],
        entireArray[j++],
        sizeof(ReviewRow));
    }

    
    for (int idx = leftSide; idx <= rightSide; ++idx) {
        memcpy(entireArray[idx],
        reviewBuffer[idx],
        sizeof(ReviewRow));
    }
}