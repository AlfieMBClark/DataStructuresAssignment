
#ifndef SORT_ALGORITHMS_H
#define SORT_ALGORITHMS_H

#include "DataCleaning.h"
#include "LoadData.h"

// WordCount struct remains the same
struct WordCount {
    char word[FieldLength];
    int count; 
};

typedef char TransactionRow[TransactionFields][FieldLength];
typedef char ReviewRow[ReviewFields][FieldLength];

void MergeSort(void* entireArray, void* buffer, int elementSize, int leftSide, int rightSide, int column, int fieldLength);

// Original function signatures for backward compatibility
void mergeSortTransactions(char (*entireArray)[TransactionFields][FieldLength], int leftSide, int rightSide, int column);
void mergeSortReviews(char (*entireArray)[ReviewFields][FieldLength], int left, int right, int column);
void mergeSortWordCounts(WordCount arr[], int left, int right);

#endif
