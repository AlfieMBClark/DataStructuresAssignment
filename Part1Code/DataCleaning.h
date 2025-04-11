#ifndef DATACLEANING_H
#define DATACLEANING_H

// ---------- Data Structures ----------
struct Transaction {
    char customerID[50];
    char product[50];
    char category[50];
    double price;
    char date[20];
    char paymentMethod[30];
};

struct Review {
    char productID[50];
    char customerID[50];
    int rating;
    char reviewText[255];
};

// ---------- Cleaning Function Prototypes ----------
void cleanTransactions(const char* inputFile, const char* outputFile);
void cleanReviews(const char* inputFile, const char* outputFile);

#endif
