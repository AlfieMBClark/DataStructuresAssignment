#include "SortAlgorithms.h"
#include <cstring>

static TransactionRow buffer[Rows];

void mergeSort(TransactionRow EntireArray[], int leftSide, int rightSide, int column) {
    if (leftSide >= rightSide) return;
    int mid = (leftSide + rightSide) / 2;

    mergeSort(EntireArray, leftSide,  mid, column);
    mergeSort(EntireArray, mid+1,rightSide, column);

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


static ReviewRow reviewBuffer[Rows];
static void mergeReviews(ReviewRow a[], int l, int m, int r, int col) {
    int n1 = m - l + 1, n2 = r - m;
    
    for (int i = 0; i < n1; ++i)
      memcpy(reviewBuffer[l + i], a[l + i], sizeof(ReviewRow[0]));
    
    for (int i = 0; i < n2; ++i)
      memcpy(reviewBuffer[m+1 + i], a[m+1 + i], sizeof(ReviewRow[0]));

    int i = l, j = m+1, k = l;
    while (i <= m && j <= r) {
        if (strcmp(reviewBuffer[i][col], reviewBuffer[j][col]) <= 0)
            memcpy(a[k++], reviewBuffer[i++], sizeof(ReviewRow[0]));
        else
            memcpy(a[k++], reviewBuffer[j++], sizeof(ReviewRow[0]));
    }
    while (i <= m)   memcpy(a[k++], reviewBuffer[i++], sizeof(ReviewRow[0]));
    while (j <= r)   memcpy(a[k++], reviewBuffer[j++], sizeof(ReviewRow[0]));
}
void mergeSortReviews(ReviewRow array[], int left, int right, int column) {
    if (left < right) {
        int m = (left + right) / 2;
        mergeSortReviews(array, left,   m, column);
        mergeSortReviews(array, m+1, right, column);
        mergeReviews     (array, left,   m, right, column);
    }
}


static WordCount wordcountBuffer[10000];
static void mergeWordCounts(WordCount a[], int l, int m, int r) {
    int i = l, j = m+1, k = l;
    while (i <= m && j <= r) {
        if (a[i].count >= a[j].count)
        wordcountBuffer[k++] = a[i++];
        else
        wordcountBuffer[k++] = a[j++];
    }
    while (i <= m) wordcountBuffer[k++] = a[i++];
    while (j <= r) wordcountBuffer[k++] = a[j++];
    for (int x = l; x <= r; ++x) a[x] = wordcountBuffer[x];
}
void mergeSortWordCounts(WordCount a[], int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;
        mergeSortWordCounts(a, l,   m);
        mergeSortWordCounts(a, m+1, r);
        mergeWordCounts    (a, l,   m,   r);
    }
}