#include "SearchAlgorithms.h"
#include <cstring>
#include <iostream>
#include <iomanip>

using namespace std;

// Binary search with a custom comparator
int binarySearch(Transaction* searchArray, int size, const char* target, TransactionFieldComparator comparator) {
    int left = 0, right = size - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        int cmp = comparator(searchArray[mid], target);

        if (cmp == 0) return mid;          // Match found
        else if (cmp < 0) left = mid + 1;  // Look right
        else right = mid - 1;              // Look left
    }

    return -1; // Not found
}

// Comparator to compare category field
int compareCategory(const Transaction& t, const char* target) {
    return strcmp(t.category, target);
}

// Comparator to compare date field
int compareDate(const Transaction& t, const char* target) {
    return strcmp(t.date, target);
}

// function to compute percentage of a given category paid with a given method
double percentageWithCategoryAndMethod(Transaction* searchArray, int size, const char* category, const char* paymentMethod) {
    int index = binarySearch(searchArray, size, category, compareCategory);

    int total = 0;
    int matchCount = 0;

    if (index != -1) {
        int i = index;
        while (i >= 0 && strcmp(searchArray[i].category, category) == 0) {
            if (strcmp(searchArray[i].paymentMethod, paymentMethod) == 0) matchCount++;
            total++;
            i--;
        }
        i = index + 1;
        while (i < size && strcmp(searchArray[i].category, category) == 0) {
            if (strcmp(searchArray[i].paymentMethod, paymentMethod) == 0) matchCount++;
            total++;
            i++;
        }
    }

    return (total > 0) ? (double)matchCount / total * 100.0 : 0.0;
}