#ifndef SORT_ALGORITHMS_H
#define SORT_ALGORITHMS_H

#include "DataCleaning.h"


//------Merge Sort-----------
// Function pointer type for custom comparison between Transactions
typedef bool (*TransactionComparator)(const Transaction&, const Transaction&);
// Merge Sort functions
void mergeSort(Transaction* arr, int left, int right, TransactionComparator cmp);
void merge(Transaction* arr, int left, int mid, int right, TransactionComparator cmp);
// Comparator Functions
bool compareByDate(const Transaction& a, const Transaction& b);
bool compareByCategory(const Transaction& a, const Transaction& b);

#endif
