#include "SortAlgorithms.h"
#include <cstring>

static Row buffer[Rows];

void mergeSort(Row EntireArray[], int leftSide, int rightSide, int column) {
    if (leftSide >= rightSide) return;
    int mid = (leftSide + rightSide) / 2;

    mergeSort(EntireArray, leftSide,  mid, column);
    mergeSort(EntireArray, mid+1,rightSide, column);

    int i = leftSide, j = mid+1, k = leftSide;
    while (i <= mid && j <= rightSide) {
        if (strcmp(EntireArray[i][column], EntireArray[j][column]) <= 0)
            memcpy(buffer[k++], EntireArray[i++], sizeof(Row));
        else
            memcpy(buffer[k++], EntireArray[j++], sizeof(Row));
    }
    while (i <= mid)   memcpy(buffer[k++], EntireArray[i++], sizeof(Row));
    while (j <= rightSide) memcpy(buffer[k++], EntireArray[j++], sizeof(Row));

    for (int idx = leftSide; idx <= rightSide; ++idx)
        memcpy(EntireArray[idx], buffer[idx], sizeof(Row));
}
