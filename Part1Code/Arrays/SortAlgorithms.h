
#ifndef SORT_ALGORITHMS_H
#define SORT_ALGORITHMS_H

#include "DataCleaning.h"
#include "LoadData.h"

// WordCount struct
struct WordCount {
    char word[FieldLength];
    int count; 
};

//Merge Sort
typedef char TransactionRow[TransactionFields][FieldLength];
typedef char ReviewRow[ReviewFields][FieldLength];
typedef char TransReviewPair[2][FieldLength]; 
void MergeSort(void* dataArray, void* Buffer, int rowSize, int left, int right, int colIndex, int fieldLen);
void mergeSortTransactions(char array[][TransactionFields][FieldLength], int left, int right, int colToSort);
void mergeSortReviews(char array[][ReviewFields][FieldLength], int left, int right, int colToSort);
void mergeSortTransReviewArray(char (*array)[2][FieldLength], int left, int right, int colToSort = 0);
void mergeSortWordCounts(WordCount arr[], int left, int right, WordCount buffer[]);

//Insertion Sort
void insertionSort(WordCount array[], int size);
void insertionSort(char data[][TransactionFields][FieldLength], int rowCount, int columnIndex);
#endif 