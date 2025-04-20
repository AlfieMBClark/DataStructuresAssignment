#ifndef SORT_ALGORITHMS_H
#define SORT_ALGORITHMS_H

#include "DataCleaning.h"

// A “row” is one transaction: TransactionFields × FieldLength
typedef char Row[TransactionFields][FieldLength];

// Merge‑sort arr[left..right] by the string in column `col`
void mergeSort(Row EntireArray[], int leftSide, int rightSide, int column);

#endif  // SORT_ALGORITHMS_H
