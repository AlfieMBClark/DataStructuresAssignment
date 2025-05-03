#include "LoadData.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>

using namespace std;

// TransactionNode ctor
TransactionNode::TransactionNode(const Transaction& t)
  : data(t), next(nullptr) {}

// ReviewNode ctor
ReviewNode::ReviewNode(const Review& r)
  : data(r), next(nullptr) {}

TransactionNode* loadTransactions(const char* filename) {
    ifstream in(filename);

    string line;
    getline(in, line); // skip header

    TransactionNode* head = nullptr;
    TransactionNode* tail = nullptr;

    while (getline(in, line)) {
        stringstream ss(line);
        string f[6];
        for (int i = 0; i < 6; ++i)
            if (!getline(ss, f[i], i<5? ',':'\n')) continue;

        Transaction t;
        strncpy(t.customerID,f[0].c_str(),MAX_FIELD-1);
        strncpy(t.product,f[1].c_str(),MAX_FIELD-1);
        strncpy(t.category,f[2].c_str(),MAX_FIELD-1);
        t.price = atof(f[3].c_str());
        strncpy(t.date,f[4].c_str(),MAX_FIELD-1);
        strncpy(t.payment,f[5].c_str(),MAX_FIELD-1);

        TransactionNode* node = new TransactionNode(t);
        if (!head) head = tail = node;
        else { tail->next = node; tail = node; }
    }
    return head;
}

ReviewNode* loadReviews(const char* filename) {
    ifstream in(filename);

    string line;
    getline(in, line); // skip header

    ReviewNode* head = nullptr;
    ReviewNode* tail = nullptr;

    while (getline(in, line)) {
        stringstream ss(line);
        string f[4];
        for (int i = 0; i < 4; ++i)
            if (!getline(ss, f[i], i<3? ',':'\n')) continue;

        Review r;
        strncpy(r.productID,f[0].c_str(), MAX_FIELD-1);
        strncpy(r.customerID,f[1].c_str(), MAX_FIELD-1);
        r.rating = atoi(f[2].c_str());
        strncpy(r.text,f[3].c_str(), MAX_FIELD-1);

        ReviewNode* node = new ReviewNode(r);
        if (!head) head = tail = node;
        else { tail->next = node; tail = node; }
    }
    return head;
}

int listLength(TransactionNode* head) {
    int cnt = 0;
    while (head) { ++cnt; head = head->next; }
    return cnt;
}

int listLength(ReviewNode* head) {
    int cnt = 0;
    while (head) { ++cnt; head = head->next; }
    return cnt;
}
