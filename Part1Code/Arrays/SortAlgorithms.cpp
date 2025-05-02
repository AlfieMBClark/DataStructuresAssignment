#include "SortAlgorithms.h"
#include <cstring>


// Buffers
TransactionRow* transactionbuffer = nullptr;
ReviewRow* reviewBuffer = nullptr;
TransReviewPair* traRevBuffer = nullptr;

// General Merge Sort for trans, rev and transrev array
void MergeSort(void* dataArray, void* Buffer, int rowSize, int left, int right, int colIndex, int fieldLen) {
    if (left >= right) return;

    int mid = (left + right) / 2;

    // Sort halves
    MergeSort(dataArray, Buffer, rowSize, left, mid, colIndex, fieldLen);
    MergeSort(dataArray, Buffer, rowSize, mid + 1, right, colIndex, fieldLen);

    // Merge tgt
    char* data = (char*)dataArray;
    char* temp = (char*)Buffer;

    int i = left, j = mid + 1, k = left;

    while (i <= mid && j <= right) {
        char* rowI = data + (i * rowSize);
        char* rowJ = data + (j * rowSize);

        char* fieldI = rowI + (colIndex * fieldLen);
        char* fieldJ = rowJ + (colIndex * fieldLen);

        //comp values
        if (strcmp(fieldI, fieldJ) <= 0) {
            memcpy(temp + (k * rowSize), rowI, rowSize);
            i++;
        } else {
            memcpy(temp + (k * rowSize), rowJ, rowSize);
            j++;
        }
        k++;
    }

    //copy leftover in left
    while (i <= mid) {
        memcpy(temp + (k * rowSize), data + (i * rowSize), rowSize);
        i++; k++;
    }

    //copy leftover in right
    while (j <= right) {
        memcpy(temp + (k * rowSize), data + (j * rowSize), rowSize);
        j++; k++;
    }

    //copy sorted data into og array
    for (int idx = left; idx <= right; ++idx) {
        memcpy(data + (idx * rowSize), temp + (idx * rowSize), rowSize);
    }
}

//Wrappers
void mergeSortTransactions(char array[][TransactionFields][FieldLength], int left, int right, int colToSort) {
    if (transactionbuffer == nullptr) {
        transactionbuffer = new TransactionRow[transactionCount];
    }
    int rowSize = sizeof(TransactionRow);
    MergeSort(array, transactionbuffer, rowSize, left, right, colToSort, FieldLength);
}

void mergeSortReviews(char array[][ReviewFields][FieldLength], int left, int right, int colToSort) {
    if (reviewBuffer == nullptr) {
        reviewBuffer = new ReviewRow[reviewCount];
    }
    int rowSize = sizeof(ReviewRow);
    MergeSort(array, reviewBuffer, rowSize, left, right, colToSort, FieldLength);
}

void mergeSortTransReviewArray(char (*array)[2][FieldLength], int left, int right, int colToSort) {
    if (traRevBuffer == nullptr) {
        traRevBuffer = new TransReviewPair[transactionCount];
    }
    int elementSize = sizeof(char) * 2 * FieldLength;
    MergeSort(array, traRevBuffer, elementSize, left, right, colToSort, FieldLength);
}



void mergeSortWordCounts(WordCount words[], int low, int high, WordCount temp[]) {
    if (low >= high) return;

    int mid = (low + high) / 2;
    mergeSortWordCounts(words, low, mid, temp);
    mergeSortWordCounts(words, mid + 1, high, temp);

    int i = low, j = mid + 1, k = low;

    while (i <= mid && j <= high) {
        if (words[i].count >= words[j].count) {
            temp[k++] = words[i++];
        } else {
            temp[k++] = words[j++];
        }
    }

    while (i <= mid) temp[k++] = words[i++];
    while (j <= high) temp[k++] = words[j++];

    for (int x = low; x <= high; ++x) {
        words[x] = temp[x];
    }
}


//Insertion Sort
void insertionSort(char data[][TransactionFields][FieldLength], int rowCount, int columnIndex) {
    for (int i = 1; i < rowCount; ++i) {
        char temp[TransactionFields][FieldLength];
        memcpy(temp, data[i], sizeof(temp));  // Store the current row

        int j = i - 1;
        while (j >= 0 && strcmp(data[j][columnIndex], temp[columnIndex]) > 0) {
            memcpy(data[j + 1], data[j], sizeof(data[j]));  // Shift row up
            --j;
        }

        memcpy(data[j + 1], temp, sizeof(temp));  // Insert the saved row
    }
}

void insertionSort(WordCount array[], int size) {
    for (int i = 1; i < size; ++i) {
        WordCount temp = array[i];
        int j = i - 1;
        while (j >= 0 && array[j].count < temp.count) {
            array[j + 1] = array[j];
            --j;
        }
        array[j + 1] = temp;
    }
}