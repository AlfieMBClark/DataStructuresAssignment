#include "SortAlgorithms.h"
#include <cstring>

// Buffer defined based on transactionCount
TransactionRow* buffer = nullptr;

void mergeSort(char (*EntireArray)[TransactionFields][FieldLength], int leftSide, int rightSide, int column) {
    // Allocate buffer if needed
    if (buffer == nullptr) {
        buffer = new TransactionRow[transactionCount];
    }
    
    if (leftSide >= rightSide) return;
    int mid = (leftSide + rightSide) / 2;

    mergeSort(EntireArray, leftSide, mid, column);
    mergeSort(EntireArray, mid+1, rightSide, column);

    int i = leftSide, j = mid+1, k = leftSide;
    while (i <= mid && j <= rightSide) {
        if (strcmp(EntireArray[i][column], EntireArray[j][column]) <= 0)
            memcpy(buffer[k++], EntireArray[i++], sizeof(TransactionRow));
        else
            memcpy(buffer[k++], EntireArray[j++], sizeof(TransactionRow));
    }
    while (i <= mid)   memcpy(buffer[k++], EntireArray[i++], sizeof(TransactionRow));
    while (j <= rightSide) memcpy(buffer[k++], EntireArray[j++], sizeof(TransactionRow));

    for (int idx = leftSide; idx <= rightSide; ++idx)
        memcpy(EntireArray[idx], buffer[idx], sizeof(TransactionRow));
}

// Buffer defined based on reviewCount
ReviewRow* reviewBuffer = nullptr;

void mergeSortReviews(char (*entireArray)[ReviewFields][FieldLength], int leftSide, int rightSide, int column) {
    // Allocate buffer if needed
    if (reviewBuffer == nullptr) {
        reviewBuffer = new ReviewRow[reviewCount];
    }

    if (leftSide >= rightSide) return;

    int mid = (leftSide + rightSide) / 2;
    mergeSortReviews(entireArray, leftSide, mid, column);
    mergeSortReviews(entireArray, mid+1, rightSide, column);

    int i = leftSide;      
    int j = mid + 1;       
    int k = leftSide;     

    while (i <= mid && j <= rightSide) {
        if (strcmp(entireArray[i][column], entireArray[j][column]) <= 0) {
            memcpy(reviewBuffer[k++], entireArray[i++], sizeof(ReviewRow));
        } else {
            memcpy(reviewBuffer[k++], entireArray[j++], sizeof(ReviewRow));
        }
    }
   
    while (i <= mid) {
        memcpy(reviewBuffer[k++],
        entireArray[i++],
        sizeof(ReviewRow));
    }
    while (j <= rightSide) {
        memcpy(reviewBuffer[k++],
        entireArray[j++],
        sizeof(ReviewRow));
    }

    for (int idx = leftSide; idx <= rightSide; ++idx) {
        memcpy(entireArray[idx],
        reviewBuffer[idx],
        sizeof(ReviewRow));
    }
}

// WordCount buffer with fixed size
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