#include "SortAlgorithms.h"
#include <cstring>


// Buffers
TransactionRow* transactionbuffer = nullptr;
ReviewRow* reviewBuffer = nullptr;
TransReviewPair* traRevBuffer = nullptr;

// Merge Sort
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

//MergeCount
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


//##################################################################################################
//##################################################################################################
// heap sort algorithm for transactions by date 

void heapify(char arr[][TransactionFields][FieldLength], int n, int i, int column) {
    int largest = i; // root
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    // Compare with left child
    if (left < n && strcmp(arr[left][column], arr[largest][column]) > 0)
        largest = left;

    // Compare with right child
    if (right < n && strcmp(arr[right][column], arr[largest][column]) > 0)
        largest = right;

    // Swap if needed
    if (largest != i) {
        for (int j = 0; j < TransactionFields; ++j) {
            char temp[FieldLength];
            strcpy(temp, arr[i][j]);
            strcpy(arr[i][j], arr[largest][j]);
            strcpy(arr[largest][j], temp);
        }

        // Recursively heapify
        heapify(arr, n, largest, column);
    }
}

void heapSort(char arr[][TransactionFields][FieldLength], int n, int column) {
    // Build max heap
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i, column);

    // Extract elements from heap
    for (int i = n - 1; i > 0; i--) {
        // Swap root with last element
        for (int j = 0; j < TransactionFields; ++j) {
            char temp[FieldLength];
            strcpy(temp, arr[0][j]);
            strcpy(arr[0][j], arr[i][j]);
            strcpy(arr[i][j], temp);
        }

        // Heapify reduced heap
        heapify(arr, i, 0, column);
    }
}
//##################################################################################################
//##################################################################################################

// Function to maintain the heap property (max-heap) for transactions
// For WordCount (sorting by frequency)
void heapify(WordCount* wc, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && wc[left].count > wc[largest].count)
        largest = left;

    if (right < n && wc[right].count > wc[largest].count)
        largest = right;

    if (largest != i) {
        WordCount temp = wc[i];
        wc[i] = wc[largest];
        wc[largest] = temp;
        heapify(wc, n, largest);
    }
}

void heapSort(WordCount* wc, int n) {
    for (int i = n / 2 - 1; i >= 0; --i)  // Build max heap
        heapify(wc, n, i);

    for (int i = n - 1; i > 0; --i) {
        WordCount temp = wc[0];
        wc[0] = wc[i];
        wc[i] = temp;
        heapify(wc, i, 0);  // Heapify reduced heap
    }
}

//##################################################################################################
//##################################################################################################

