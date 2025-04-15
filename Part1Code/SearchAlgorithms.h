#ifndef SEARCH_ALGORITHMS_H
#define SEARCH_ALGORITHMS_H

#include "DataCleaning.h"

// Function pointer type for binary search comparison
typedef int (*TransactionFieldComparator)(const Transaction&, const char* target);

// General binary search on a sorted Transaction array
int binarySearch(Transaction* arr, int size, const char* target, TransactionFieldComparator comparator);

// Example field-based comparators
int compareCategory(const Transaction& t, const char* target);
int compareDate(const Transaction& t, const char* target);

// function to calculate percentage for a given category and payment method
double percentageWithCategoryAndMethod(Transaction* arr, int size, const char* category, const char* paymentMethod);

#endif