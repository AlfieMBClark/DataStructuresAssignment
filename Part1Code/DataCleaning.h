#ifndef DATA_CLEANING_H
#define DATA_CLEANING_H

#include <string>

static const int Rows              = 5000;
static const int TransactionFields = 6;
static const int ReviewFields      = 4;
static const int FieldLength       = 255;

extern char transactions[Rows][TransactionFields][FieldLength];
extern int  transactionCount;

extern char reviews[Rows][ReviewFields][FieldLength];
extern int  reviewCount;

void cleanTransactions(const char* inputFile);
void cleanReviews     (const char* inputFile);

#endif 
