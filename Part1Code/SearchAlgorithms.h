#ifndef SEARCH_ALGORITHMS_H
#define SEARCH_ALGORITHMS_H

#include "DataCleaning.h"

int  binarySearch(char SortedArray[][TransactionFields][FieldLength],int  size,const char* target,int  column);

double percentageWithCategoryAndMethod(char SortedArray[][TransactionFields][FieldLength],int  size,const char* category,const char* paymentMethod);

#endif  // SEARCH_ALGORITHMS_H
