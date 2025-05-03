#include "LoadData.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>

using namespace std;

char* allocString(const string& str) {
    char* ptr = new char[str.length() + 1];
    strcpy(ptr, str.c_str());
    return ptr;
}

// Constructor
TransactionNode::TransactionNode(const Transaction& t) {
    data.customerID=allocString(t.customerID);
    data.product= allocString(t.product);
    data.category= allocString(t.category);
    data.price= t.price;
    data.date= allocString(t.date);
    data.paymentMethod=allocString(t.paymentMethod);
    next= nullptr;
}

// Destructor
TransactionNode::~TransactionNode() {
    delete[] data.customerID;
    delete[] data.product;
    delete[] data.category;
    delete[] data.date;
    delete[] data.paymentMethod;
}

// Constructor
ReviewNode::ReviewNode(const Review& r) {
    data.productID=allocString(r.productID);
    data.customerID= allocString(r.customerID);
    data.rating= r.rating;
    data.Reviewtext=allocString(r.Reviewtext);
    next = nullptr;
}

// Destructor
ReviewNode::~ReviewNode() {
    delete[] data.productID;
    delete[] data.customerID;
    delete[] data.Reviewtext;
}

TransactionNode* loadTransactions(const char* filename) {
    ifstream file(filename);
    string line;
    getline(file, line); // Skip header

    TransactionNode* head = nullptr;
    TransactionNode* tail = nullptr;

    while (getline(file, line)) {
        stringstream ss(line);
        string f[6];
        for (int i = 0; i < 6; ++i)
            getline(ss, f[i], (i < 5) ? ',' : '\n');

        Transaction t;
        t.customerID= (char*)f[0].c_str();
        t.product= (char*)f[1].c_str();
        t.category= (char*)f[2].c_str();
        t.price= atof(f[3].c_str());
        t.date= (char*)f[4].c_str();
        t.paymentMethod= (char*)f[5].c_str();

        TransactionNode* node = new TransactionNode(t);
        if (!head)
            head = tail = node;
        else {
            tail->next = node;
            tail = node;
        }
    }
    return head;
}

ReviewNode* loadReviews(const char* filename) {
    ifstream file(filename);
    string line;
    getline(file, line); // Skip header

    ReviewNode* head = nullptr;
    ReviewNode* tail = nullptr;

    while (getline(file, line)) {
        stringstream ss(line);
        string f[4];
        for (int i = 0; i < 4; ++i)
            getline(ss, f[i], (i < 3) ? ',' : '\n');

        Review r;
        r.productID=(char*)f[0].c_str();
        r.customerID=(char*)f[1].c_str();
        r.rating=atoi(f[2].c_str());
        r.Reviewtext= (char*)f[3].c_str();

        ReviewNode* node=new ReviewNode(r);
        if (!head)
            head = tail = node;
        else {
            tail->next = node;
            tail = node;
        }
    }
    return head;
}

int listLength(TransactionNode* head) {
    int count = 0;
    while (head) {
        count++;
        head = head->next;
    }
    return count;
}

int listLength(ReviewNode* head) {
    int count = 0;
    while (head) {
        count++;
        head = head->next;
    }
    return count;
}
