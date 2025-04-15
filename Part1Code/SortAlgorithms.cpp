#include "SortAlgorithms.h"
#include <cstring>

// ---------- General Comparison Function for Merge Sort----------
// Compares two C-style strings and returns true if the first is "less than" the second.
// This is used for ordering during the sort.
// returns 0 if both strings are equal, <0 if a comes before b and >0 if a is aafter b

bool compareByDate(const Transaction& a, const Transaction& b) {
    return strcmp(a.date, b.date) < 0;
}

bool compareByCategory(const Transaction& a, const Transaction& b) {
    return strcmp(a.category, b.category)<0;
}

// ---------- Merge Function ----------
// Merges two sorted subarrays into a single sorted segment.
// Parameters:
// - arr: the full array being sorted
// - left: starting index of the first half
// - mid: ending index of the first half
// - right: ending index of the second half
void merge(Transaction* fullarray, int left, int mid, int right, TransactionComparator cmp) {
    
    // Calculate sizes of temporary subarrays
    int size1 = mid - left + 1;
    int size2 = right - mid;

    // Create temporary arrays for left and right halves
    Transaction* leftArr = new Transaction[size1];
    Transaction* rightArr = new Transaction[size2];

    // Copy data into the temporary arrays
    for (int i = 0; i < size1; ++i)
        leftArr[i] = fullarray[left + i];
    for (int i = 0; i < size2; ++i)
        rightArr[i] = fullarray[mid + 1 + i];

    
        int i = 0, j = 0, k = left;
         // Merge the two temporary arrays back into the main array
        while (i < size1 && j < size2) {
            if (cmp(leftArr[i], rightArr[j])) {
                fullarray[k++] = leftArr[i++];
            } else {
                fullarray[k++] = rightArr[j++];
            }
        }
        
         // Copy any remaining elements from the left half
        while (i < size1) fullarray[k++] = leftArr[i++];

        // Copy any remaining elements from the right half
        while (j < size2) fullarray[k++] = rightArr[j++];
    
        // Free up allocated memory
        delete[] leftArr;
        delete[] rightArr;
    }

// ---------- Merge Sort Function ----------

// Parameters:
// - arr: the array to be sorted
// - left: starting index of the portion to be sorted
// - right: ending index of the portion to be sorted
void mergeSort(Transaction* fullarray, int left, int right, TransactionComparator cmp) {
    if (left < right) {
        // Calculate the middle index
        int mid = left + (right - left) / 2;

        // Sort the left half
        mergeSort(fullarray, left, mid, cmp);

        // Sort the right half
        mergeSort(fullarray, mid + 1, right, cmp);

        // Merge the two sorted halves
        merge(fullarray, left, mid, right, cmp);
    }
}
