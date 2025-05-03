#ifndef LOADDATA_H
#define LOADDATA_H

struct Transaction {char* customerID;char* product;char* category;double price;char* date;char* paymentMethod;};

struct TransactionNode {Transaction data;TransactionNode* next;TransactionNode(const Transaction& t);~TransactionNode();};

struct Review {char* productID;char* customerID;int rating;char* Reviewtext;};

struct ReviewNode {Review data;ReviewNode* next;ReviewNode(const Review& r);~ReviewNode();};

TransactionNode* loadTransactions(const char* filename);
ReviewNode* loadReviews(const char* filename);

int listLength(TransactionNode* head);
int listLength(ReviewNode* head);

#endif
