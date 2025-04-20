#ifndef DATA_CLEANING_H
#define DATA_CLEANING_H

#include <string>


static const int Rows              = 5000;
static const int TransactionFields = 6;   
static const int ReviewFields      = 4;  
static const int FieldLength       = 255; 

// In‑memory storage for cleaned data
extern char transactions[Rows][TransactionFields][FieldLength];
extern int  transactionCount;

extern char reviews[Rows][ReviewFields][FieldLength];
extern int  reviewCount;

// Produce cleaned CSVs from raw input
void cleanTransactions(const char* inputFile);
void cleanReviews     (const char* inputFile);

// Load cleaned CSVs into the above arrays
void loadTransactions(const char* cleanedFile);
void loadReviews     (const char* cleanedFile);

#endif  // DATA_CLEANING_H