
#ifndef SORT_ALGORITHMS_H
#define SORT_ALGORITHMS_H

#include "DataCleaning.h"
#include "LoadData.h"

// WordCount struct
struct WordCount {
    char word[FieldLength];
    int count; 
};
typedef char TransactionRow[TransactionFields][FieldLength];
typedef char ReviewRow[ReviewFields][FieldLength];
typedef char TransReviewPair[2][FieldLength]; 

//Merge Sort
void MergeSort(void* dataArray, void* Buffer, int rowSize, int left, int right, int colIndex, int fieldLen);
void mergeSortTransactions(char array[][TransactionFields][FieldLength], int left, int right, int colToSort);
void mergeSortReviews(char array[][ReviewFields][FieldLength], int left, int right, int colToSort);
void mergeSortTransReviewArray(char (*array)[2][FieldLength], int left, int right, int colToSort = 0);
void mergeSortWordCounts(WordCount arr[], int left, int right, WordCount buffer[]);


//Insertion Sort
void insertionSort(WordCount array[], int size);
void insertionSort(char data[][TransactionFields][FieldLength], int rowCount, int columnIndex);


// Function declarations for heap sort
void heapify(char arr[][TransactionFields][FieldLength], int n, int i, int column);
void heapSort(char arr[][TransactionFields][FieldLength], int n, int column);
// Function declarations for heap sort and heapify (for WordCount)
void heapify(WordCount* arr, int n, int i);  // Heapify for WordCount array
void heapSort(WordCount* arr, int n);  // Heap sort for WordCount array
// Function to find 1-star reviews
void findOneStarReviews(int& first, int& last, int reviewCount, char reviews[][ReviewFields][FieldLength]);
// Function to count word frequencies in the 1-star reviews
void countWordFrequencies(int first, int last, char reviews[][ReviewFields][FieldLength], WordCount* wc, int& unique);
// Function to display the top N most frequent words
void displayTopWords(WordCount* wc, int unique, int N);







#endif 