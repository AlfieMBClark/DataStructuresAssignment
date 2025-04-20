#ifndef SORT_ALGORITHMS_H
#define SORT_ALGORITHMS_H

#include "DataCleaning.h"

typedef char TransactionRow[TransactionFields][FieldLength];
typedef char ReviewRow[ReviewFields][FieldLength];

// Merge‑sort arr[left..right] by the string in column `col`
void mergeSort(TransactionRow EntireArray[], int leftSide, int rightSide, int column);
void mergeSortReviews(ReviewRow arr[], int left, int right, int col);

#endif  // SORT_ALGORITHMS_H
