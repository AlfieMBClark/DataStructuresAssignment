#ifndef LOADDATA_H
#define LOADDATA_H

#include <string>
using namespace std;

//Structs
struct Transaction {string customerID;string product;string category;double price;string date;string paymentMethod;};

struct Review {string productID;string customerID;int rating;string reviewText;};

//Node structs
struct TransactionNode {
    Transaction data;
    TransactionNode* next;
    TransactionNode(const Transaction& t);
};

struct ReviewNode {
    Review data;
    ReviewNode* next;
    ReviewNode(const Review& r);
};

// Function
TransactionNode* loadTransactions(const string& filename);
ReviewNode* loadReviews(const string& filename);

#endif
