#include "SortAlgorithms.h"
#include <cstring>

// ---------- General Comparison Function ----------
// Compares two C-style strings and returns true if the first is "less than" the second.
// This is used for ordering during the sort.
bool compareDates(const char* date1, const char* date2) {
    return strcmp(date1, date2) < 0;
}

// ---------- Merge Function ----------
// Merges two sorted subarrays into a single sorted segment.
// Parameters:
// - arr: the full array being sorted
// - left: starting index of the first half
// - mid: ending index of the first half
// - right: ending index of the second half
void merge(Transaction* arr, int left, int mid, int right) {
    // Calculate sizes of temporary subarrays
    int size1 = mid - left + 1;
    int size2 = right - mid;

    // Create temporary arrays for left and right halves
    Transaction* leftArr = new Transaction[size1];
    Transaction* rightArr = new Transaction[size2];

    // Copy data into the temporary arrays
    for (int i = 0; i < size1; ++i)
        leftArr[i] = arr[left + i];
    for (int i = 0; i < size2; ++i)
        rightArr[i] = arr[mid + 1 + i];

    // Merge the two temporary arrays back into the main array
    int i = 0, j = 0, k = left;
    while (i < size1 && j < size2) {
        if (compareDates(leftArr[i].date, rightArr[j].date)) {
            arr[k++] = leftArr[i++];
        } else {
            arr[k++] = rightArr[j++];
        }
    }

    // Copy any remaining elements from the left half
    while (i < size1)
        arr[k++] = leftArr[i++];

    // Copy any remaining elements from the right half
    while (j < size2)
        arr[k++] = rightArr[j++];

    // Free up allocated memory
    delete[] leftArr;
    delete[] rightArr;
}

// ---------- Merge Sort Function ----------
// A recursive function that implements merge sort on an array.
// Parameters:
// - arr: the array to be sorted
// - left: starting index of the portion to be sorted
// - right: ending index of the portion to be sorted
void mergeSort(Transaction* arr, int left, int right) {
    if (left < right) {
        // Calculate the middle index
        int mid = left + (right - left) / 2;

        // Sort the left half
        mergeSort(arr, left, mid);

        // Sort the right half
        mergeSort(arr, mid + 1, right);

        // Merge the two sorted halves
        merge(arr, left, mid, right);
    }
}
