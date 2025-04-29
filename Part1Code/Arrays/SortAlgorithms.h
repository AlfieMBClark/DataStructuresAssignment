#ifndef SORT_ALGORITHMS_H
#define SORT_ALGORITHMS_H

#include "DataCleaning.h"
#include "LoadData.h"

// WordCount struct remains the same
struct WordCount {
    char word[FieldLength];
    int count; 
};

// We keep the original typedefs as they are used in the mergeSort implementation
// but we'll update the function signatures
typedef char TransactionRow[TransactionFields][FieldLength];
typedef char ReviewRow[ReviewFields][FieldLength];

// Updated function signatures for dynamic arrays
void mergeSort(char (*EntireArray)[TransactionFields][FieldLength], int leftSide, int rightSide, int column);
void mergeSortReviews(char (*EntireArray)[ReviewFields][FieldLength], int left, int right, int column);
void mergeSortWordCounts(WordCount arr[], int left, int right);

#endif 