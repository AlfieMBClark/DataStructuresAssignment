#ifndef SEARCH_ALGORITHMS_H
#define SEARCH_ALGORITHMS_H

#include "LoadData.h"
#include "SortAlgorithms.h"

int binarySearch(char SortedArray[][TransactionFields][FieldLength],int  size,const char* target,int column);

int binarySearchReviews(char SortedReviewArray[][ReviewFields][FieldLength],int  size,const char* target,int  column);

double percentageWithCategoryAndMethod(char SortedArray[][TransactionFields][FieldLength],int  size,const char* category,const char* paymentMethod);


// hashing algo declaration
void hashSearchCategoryPayment(char data[][TransactionFields][FieldLength], int size);



typedef char TransactionRow[TransactionFields][FieldLength];
typedef char ReviewRow[ReviewFields][FieldLength];
typedef char TransReviewPair[2][FieldLength]; 

// Function declarations for heap sort and heapify
void heapify(char arr[][TransactionFields][FieldLength], int n, int i, int column); // For transactions
void heapSort(char arr[][TransactionFields][FieldLength], int n, int column);  // For transactions

void heapify(WordCount* arr, int n, int i); // For WordCount array
void heapSort(WordCount* arr, int n);  // For WordCount array

#endif 
