#ifndef SORT_ALGORITHMS_H
#define SORT_ALGORITHMS_H

#include "DataCleaning.h"

typedef char TransactionRow[TransactionFields][FieldLength];
typedef char ReviewRow[ReviewFields][FieldLength];


void mergeSort(TransactionRow EntireArray[], int leftSide, int rightSide, int column);
void mergeSortReviews(ReviewRow EntireArray[],int left,int right,int column);

#endif  
