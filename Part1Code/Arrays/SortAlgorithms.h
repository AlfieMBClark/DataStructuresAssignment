
#ifndef SORT_ALGORITHMS_H
#define SORT_ALGORITHMS_H

#include "LoadData.h"
#include <cstring>
using namespace std;

// WordCount struct
struct WordCount {
    char word[FieldLength];
    int count; 
};

typedef char TransactionRow[TransactionFields][FieldLength];
typedef char ReviewRow[ReviewFields][FieldLength];
typedef char TransReviewPair[2][FieldLength]; 

class MergeSort {
    private:
        TransactionRow* transTmp = nullptr;
        ReviewRow* revTmp = nullptr;
        TransReviewPair* transRevTmp = nullptr;
    
    public:
        // Sort transactions
        void sortTransactions(TransactionRow* array, int left, int right, int colIndex, int totalRows) {
            if (transTmp == nullptr) {
                transTmp = new TransactionRow[totalRows]; // Allocate
            }
            int rowSize = sizeof(TransactionRow);
            basicMergeSort(array, transTmp, rowSize, left, right, colIndex, FieldLength);
        }
    
        // Sort reviews
        void sortReviews(ReviewRow* array, int left, int right, int colIndex, int totalRows) {
            if (revTmp == nullptr) {
                revTmp = new ReviewRow[totalRows]; //same as transactiom
            }
            int rowSize = sizeof(ReviewRow);
            basicMergeSort(array, revTmp, rowSize, left, right, colIndex, FieldLength);
        }
    
        // Sort trans-rev pairs
        void sortTransRevArray(TransReviewPair* array, int left, int right, int colIndex, int totalRows) {
            if (transRevTmp == nullptr) {
                transRevTmp = new TransReviewPair[totalRows];//same as transaction
            }
            int rowSize = sizeof(TransReviewPair);
            basicMergeSort(array, transRevTmp, rowSize, left, right, colIndex, FieldLength);
        }
    
        // Sort WordCount array by freq
        void sortWordCounts(WordCount* words, int low, int high, WordCount* temp) {
            if (low >= high) return;
    
            int mid = (low + high) / 2;
            sortWordCounts(words, low, mid, temp);
            sortWordCounts(words, mid + 1, high, temp);
    
            int i = low, j = mid + 1, k = low;
            while (i <= mid && j <= high) {
                if (words[i].count >= words[j].count) {
                    temp[k++] = words[i++];
                } else {
                    temp[k++] = words[j++];
                }
            }
    
            // Remaining
            while (i <= mid) temp[k++] = words[i++];
            while (j <= high) temp[k++] = words[j++];
    
            for (int x = low; x <= high; ++x) {
                words[x] = temp[x];
            }
        }
    
    private:
        // merge sort for array
        void basicMergeSort(void* dataArray, void* tempBuffer, int rowSize, int left, int right, int colIndex, int fieldSize) {
            if (left >= right) return;
    
            int mid = (left + right) / 2;
    
            // Sort halves
            basicMergeSort(dataArray, tempBuffer, rowSize, left, mid, colIndex, fieldSize);
            basicMergeSort(dataArray, tempBuffer, rowSize, mid + 1, right, colIndex, fieldSize);
    
            // merge
            char* data = static_cast<char*>(dataArray);
            char* temp = static_cast<char*>(tempBuffer);
    
            int i = left, j = mid + 1, k = left;
            while (i <= mid && j <= right) {
                char* rowA = data + (i * rowSize);
                char* rowB = data + (j * rowSize);
    
                char* keyA = rowA + (colIndex * fieldSize);
                char* keyB = rowB + (colIndex * fieldSize);
    
                if (strcmp(keyA, keyB) <= 0) {
                    memcpy(temp + (k * rowSize), rowA, rowSize);
                    ++i;
                } else {
                    memcpy(temp + (k * rowSize), rowB, rowSize);
                    ++j;
                }
                ++k;
            }
    
            while (i <= mid) {
                memcpy(temp + (k * rowSize), data + (i * rowSize), rowSize);
                ++i; ++k;
            }
    
            while (j <= right) {
                memcpy(temp + (k * rowSize), data + (j * rowSize), rowSize);
                ++j; ++k;
            }
    
            // Copy sorted data back to original
            for (int idx = left; idx <= right; ++idx) {
                memcpy(data + (idx * rowSize), temp + (idx * rowSize), rowSize);
            }
        }
    };



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


//Bubble sort algortihm

void bubbleSort(char arr[][TransactionFields][FieldLength], int n, int column) {
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (strcmp(arr[j][column], arr[j + 1][column]) > 0) {
                // Swap rows
                for (int k = 0; k < TransactionFields; ++k) {
                    char temp[FieldLength];
                    strcpy(temp, arr[j][k]);
                    strcpy(arr[j][k], arr[j + 1][k]);
                    strcpy(arr[j + 1][k], temp);
                }
            }
        }
    }
}





#endif 