
#ifndef LOAD_DATA_H
#define LOAD_DATA_H

#include <string>

// Constants for array dimensions
static const int TransactionFields = 6;   
static const int ReviewFields      = 4;  
static const int FieldLength       = 255; 

extern char (*transactions)[TransactionFields][FieldLength];
extern int  transactionCount;
extern char (*reviews)[ReviewFields][FieldLength];
extern int  reviewCount;

void loadTransactions(const char* cleanedFile);
void loadReviews     (const char* cleanedFile);

#endif 