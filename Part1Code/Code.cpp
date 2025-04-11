#include "DataCleaning.h"
#include "SortAlgorithms.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstring>

using namespace std;

// ---------- Transaction Array ----------
class TransactionArray {
private:
    Transaction* data;
    int capacity;
    int size;

    void resize() {
        capacity *= 2;
        Transaction* newData = new Transaction[capacity];
        for (int i = 0; i < size; i++) newData[i] = data[i];
        delete[] data;
        data = newData;
    }

public:
    TransactionArray(int cap = 100) : capacity(cap), size(0) {
        data = new Transaction[capacity];
    }

    ~TransactionArray() {
        delete[] data;
    }

    void add(Transaction t) {
        if (size == capacity) resize();
        data[size++] = t;
    }

    Transaction* getData() const { return data; }
    int getSize() const { return size; }
};

// ---------- Review Array ----------
class ReviewArray {
private:
    Review* data;
    int capacity;
    int size;

    void resize() {
        capacity *= 2;
        Review* newData = new Review[capacity];
        for (int i = 0; i < size; i++) newData[i] = data[i];
        delete[] data;
        data = newData;
    }

public:
    ReviewArray(int cap = 100) : capacity(cap), size(0) {
        data = new Review[capacity];
    }

    ~ReviewArray() {
        delete[] data;
    }

    void add(Review r) {
        if (size == capacity) resize();
        data[size++] = r;
    }

    Review* getData() const { return data; }
    int getSize() const { return size; }
};

// ---------- Loaders ----------
void loadTransactionsFromFile(const char* filename, TransactionArray& transactions) {
    ifstream file(filename);
    string line;
    getline(file, line); // skip header

    while (getline(file, line)) {
        stringstream ss(line);
        string customerID, product, category, priceStr, date, paymentMethod;

        getline(ss, customerID, ',');
        getline(ss, product, ',');
        getline(ss, category, ',');
        getline(ss, priceStr, ',');
        getline(ss, date, ',');
        getline(ss, paymentMethod);

        Transaction t;
        strcpy(t.customerID, customerID.c_str());
        strcpy(t.product, product.c_str());
        strcpy(t.category, category.c_str());
        t.price = stod(priceStr);
        strcpy(t.date, date.c_str());
        strcpy(t.paymentMethod, paymentMethod.c_str());

        transactions.add(t);
    }

    file.close();
}

void loadReviewsFromFile(const char* filename, ReviewArray& reviews) {
    ifstream file(filename);
    string line;
    getline(file, line); // skip header

    while (getline(file, line)) {
        stringstream ss(line);
        string productID, customerID, ratingStr, reviewText;

        getline(ss, productID, ',');
        getline(ss, customerID, ',');
        getline(ss, ratingStr, ',');
        getline(ss, reviewText);

        Review r;
        strcpy(r.productID, productID.c_str());
        strcpy(r.customerID, customerID.c_str());
        r.rating = stoi(ratingStr);
        strcpy(r.reviewText, reviewText.c_str());

        reviews.add(r);
    }

    file.close();
}

// ---------- Main ----------
int main() {
    //Calls Clean Transaction Functions in the DataCleaning File
    cleanTransactions("transactions.csv", "cleaner_transactions.csv");
    cleanReviews("reviews.csv", "cleaner_reviews.csv");

    TransactionArray transactions;
    ReviewArray reviews;

    //Calls the 2 load from file functions from above
    loadTransactionsFromFile("cleaner_transactions.csv", transactions);
    loadReviewsFromFile("cleaner_reviews.csv", reviews);

    //----------Question 1---------------------

    //---Merge Sort---
    //Calls the mergesort function from the SortAlgorithms File
    mergeSort(transactions.getData(), 0, transactions.getSize() - 1);

    //Outputs all the data in date order
    cout << "\nSorted Transactions by Date:\n";
    for (int i = 0; i < transactions.getSize(); ++i) {
        Transaction t = transactions.getData()[i];
        cout << t.date << " - " << t.customerID << " - " << t.product
             << " - $" << fixed << setprecision(2) << t.price << endl;
    }

    cout << "\nTotal Transactions: " << transactions.getSize() << endl;
    cout << "Total Reviews: " << reviews.getSize() << endl;

    return 0;
}
