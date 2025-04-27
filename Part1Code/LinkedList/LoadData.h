#ifndef LOADDATA_H
#define LOADDATA_H

static const int MAX_FIELD = 255;

struct Transaction {
    char customerID[MAX_FIELD];
    char product   [MAX_FIELD];
    char category  [MAX_FIELD];
    double price;
    char date      [MAX_FIELD];
    char payment   [MAX_FIELD];
};

struct TransactionNode {
    Transaction data;
    TransactionNode* next;
    TransactionNode(const Transaction& t);
};

struct Review {
    char productID [MAX_FIELD];
    char customerID[MAX_FIELD];
    int  rating;
    char text      [MAX_FIELD];
};

struct ReviewNode {
    Review data;
    ReviewNode* next;
    ReviewNode(const Review& r);
};

TransactionNode* loadTransactions(const char* filename);
ReviewNode*      loadReviews     (const char* filename);

int listLength(TransactionNode* head);
int listLength(ReviewNode*   head);

#endif
