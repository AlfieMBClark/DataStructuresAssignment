#ifndef LOADDATA_H
#define LOADDATA_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib> 

using namespace std;

//Structs
struct Transaction {string customerID;string product;string category;double price;string date;string paymentMethod;};
struct Review {string productID;string customerID;int rating;string reviewText;};

//Nodes
struct TransactionNode {
    Transaction data;
    TransactionNode* next;
    TransactionNode(Transaction t) {
        data = t;
        next = NULL;
    }
};

struct ReviewNode {
    Review data;
    ReviewNode* next;
    ReviewNode(Review r) {
        data = r;
        next = NULL;
    }
};

//CLass
class Loader {
private:
    string transFile;
    string revFile;

    TransactionNode* transHead;
    ReviewNode* revHead;

    int transCount;
    int revCount;

public:
    Loader(string tx = "", string rv = "") {
        transFile = tx;
        revFile = rv;
        transHead = NULL;
        revHead = NULL;
        transCount = 0;
        revCount = 0;
    }

    bool loadTrans() {
        ifstream inFile(transFile.c_str());
        if (!inFile) {
            cout << "Could not open transaction file!" << endl;
            return false;
        }

        string line;
        getline(inFile, line); // header

        while (getline(inFile, line)) {
            stringstream ss(line);
            Transaction t;
            string priceStr;

            getline(ss, t.customerID, ',');
            getline(ss, t.product, ',');
            getline(ss, t.category, ',');
            getline(ss, priceStr, ',');
            getline(ss, t.date, ',');
            getline(ss, t.paymentMethod);

            if (t.customerID == "" || t.product == "") {
                continue;
            }

            t.price = atof(priceStr.c_str());

            TransactionNode* node = new TransactionNode(t);

            if (transHead == NULL) {
                transHead = node;
            } else {
                TransactionNode* current = transHead;
                while (current->next != NULL) {
                    current = current->next;
                }
                current->next = node;
            }

            transCount++;
        }

        cout << "Transactions loaded: " << transCount << endl;
        return true;
    }

    bool loadRev() {
        ifstream inFile(revFile.c_str());
        if (!inFile) {
            cout << "Could not open review file!" << endl;
            return false;
        }

        string line;
        getline(inFile, line); // header

        while (getline(inFile, line)) {
            stringstream ss(line);
            Review r;
            string ratingStr;

            getline(ss, r.productID, ',');
            getline(ss, r.customerID, ',');
            getline(ss, ratingStr, ',');
            getline(ss, r.reviewText);

            r.rating = atoi(ratingStr.c_str());

            ReviewNode* node = new ReviewNode(r);

            if (revHead == NULL) {
                revHead = node;
            } else {
                ReviewNode* current = revHead;
                while (current->next != NULL) {
                    current = current->next;
                }
                current->next = node;
            }

            revCount++;
        }

        cout << "Reviews loaded: " << revCount << endl;
        return true;
    }

    bool loadAll() {
        return loadTrans() && loadRev();
    }

    TransactionNode* getTransHead() {
        return transHead;
    }

    ReviewNode* getRevHead() {
        return revHead;
    }

    int getTransCount() {
        return transCount;
    }

    int getRevCount() {
        return revCount;
    }

    void printStats() {
        cout << "Total Transactions: " << transCount << endl;
        cout << "Total Reviews: " << revCount << endl;
    }
};

#endif
