#include "SortAlgorithms.h"
#include <cstring>

// Buffer for transactions
TransactionRow* buffer = nullptr;

// Buffer for reviews
ReviewRow* reviewBuffer = nullptr;

// Generic merge sort function for arrays
void MergeSort(void* entireArray, void* buffer, int elementSize, int leftSide, int rightSide, int column, int fieldLength) {
    if (leftSide >= rightSide) return;
    
    int mid = (leftSide + rightSide) / 2;
    
    MergeSort(entireArray, buffer, elementSize, leftSide, mid, column, fieldLength);
    MergeSort(entireArray, buffer, elementSize, mid+1, rightSide, column, fieldLength);
    
    // Merge the two halves
    int i = leftSide, j = mid+1, k = leftSide;
    char* typedArray = (char*)entireArray;
    char* typedBuffer = (char*)buffer;
    
    while (i <= mid && j <= rightSide) {
        char* elemI = typedArray + (i * elementSize);
        char* elemJ = typedArray + (j * elementSize);
        
        char* colI = elemI + (column * fieldLength);
        char* colJ = elemJ + (column * fieldLength);
        
        // Compare column 
        if (strcmp(colI, colJ) <= 0) {
            memcpy(typedBuffer + (k * elementSize), elemI, elementSize);
            i++;
        } else {
            memcpy(typedBuffer + (k * elementSize), elemJ, elementSize);
            j++;
        }
        k++;
    }
    
    // Copy remaining elements
    while (i <= mid) {
        memcpy(typedBuffer + (k * elementSize), typedArray + (i * elementSize), elementSize);
        i++;
        k++;
    }
    
    while (j <= rightSide) {
        memcpy(typedBuffer + (k * elementSize), typedArray + (j * elementSize), elementSize);
        j++;
        k++;
    }
    
    for (int idx = leftSide; idx <= rightSide; idx++) {
        memcpy(typedArray + (idx * elementSize), typedBuffer + (idx * elementSize), elementSize);
    }
}

// function for transactions
void mergeSortTransactions(char (*entireArray)[TransactionFields][FieldLength], int leftSide, int rightSide, int column) {
    if (buffer == nullptr) {
        buffer = new TransactionRow[transactionCount];
    }
    int elementSize = sizeof(char) * TransactionFields * FieldLength;
    MergeSort(entireArray, buffer, elementSize, leftSide, rightSide, column, FieldLength);
}

//function for reviews
void mergeSortReviews(char (*entireArray)[ReviewFields][FieldLength], int leftSide, int rightSide, int column) {
    if (reviewBuffer == nullptr) {
        reviewBuffer = new ReviewRow[reviewCount];
    }
    
    int elementSize = sizeof(char) * ReviewFields * FieldLength;
    
    MergeSort(entireArray, reviewBuffer, elementSize, leftSide, rightSide, column, FieldLength);
}


WordCount wcBuffer[5000];
static void mergeWordCounts(WordCount arr[], int left, int mid, int right) {
    int i = left, j = mid+1, k = left;

    while (i <= mid && j <= right) {
        if (arr[i].count >= arr[j].count) {
            wcBuffer[k++] = arr[i++];
        } else {
            wcBuffer[k++] = arr[j++];
        }
    }
    while (i <= mid)   wcBuffer[k++] = arr[i++];
    while (j <= right) wcBuffer[k++] = arr[j++];

    for (int x = left; x <= right; ++x) {
        arr[x] = wcBuffer[x];
    }
}

void mergeSortWordCounts(WordCount arr[], int left, int right) {
    if (left >= right) return;
    int mid = (left + right) / 2;
    mergeSortWordCounts(arr, left, mid);
    mergeSortWordCounts(arr, mid+1, right);
    mergeWordCounts(arr, left, mid, right);
}
