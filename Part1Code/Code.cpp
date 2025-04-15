#include "DataCleaning.h"
#include "SortAlgorithms.h"
#include "SearchAlgorithms.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstring>

using namespace std;

// ---------- Transaction Array ----------
// Custom dynamic array class for storing Transaction objects
class TransactionArray {
    private:
        Transaction* data;  // Pointer to the dynamic array
        int capacity;       // Maximum capacity before needing to resize
        int size;           // Current number of elements stored
    
        // Doubles the size of the array and copies the existing elements
        void resize() {
            capacity *= 2;
            Transaction* newData = new Transaction[capacity];
            for (int i = 0; i < size; i++) newData[i] = data[i];
            delete[] data;  // Free old memory
            data = newData;
        }
    
    public:
        // Constructor to initialize the array with a default capacity
        TransactionArray(int cap = 100) : capacity(cap), size(0) {
            data = new Transaction[capacity];
        }
    
        // Destructor to clean up memory
        ~TransactionArray() {
            delete[] data;
        }
    
        // Adds a new transaction, resizing the array if necessary
        void add(Transaction t) {
            if (size == capacity) resize();
            data[size++] = t;
        }
    
        // Returns a pointer to the array
        Transaction* getData() const { return data; }
    
        // Returns the number of elements currently stored
        int getSize() const { return size; }
    };
    
    // ---------- Review Array ----------
    // Custom dynamic array class for storing Review objects
    class ReviewArray {
    private:
        Review* data;
        int capacity;
        int size;
    
        // Resizes the array by doubling its capacity
        void resize() {
            capacity *= 2;
            Review* newData = new Review[capacity];
            for (int i = 0; i < size; i++) newData[i] = data[i];
            delete[] data;
            data = newData;
        }
    
    public:
        // Constructor to initialize the array
        ReviewArray(int cap = 100) : capacity(cap), size(0) {
            data = new Review[capacity];
        }
    
        // Destructor to release memory
        ~ReviewArray() {
            delete[] data;
        }
    
        // Adds a new review, resizing the array if necessary
        void add(Review r) {
            if (size == capacity) resize();
            data[size++] = r;
        }
    
        // Returns pointer to the array
        Review* getData() const { return data; }
    
        // Returns current size of the array
        int getSize() const { return size; }
    };
    
    // ---------- Loaders ----------
    // Loads transactions from cleaned CSV file into a TransactionArray
    void loadTransactionsFromFile(const char* filename, TransactionArray& transactions) {
        ifstream file(filename);
        string line;
        getline(file, line); // Skip the header line
    
        // Read each line and parse transaction fields
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
            t.price = stod(priceStr);  // Convert price to double
            strcpy(t.date, date.c_str());
            strcpy(t.paymentMethod, paymentMethod.c_str());
    
            transactions.add(t);  // Add to dynamic array
        }
    
        file.close();
    }
    
    // Loads reviews from cleaned CSV file into a ReviewArray
    void loadReviewsFromFile(const char* filename, ReviewArray& reviews) {
        ifstream file(filename);
        string line;
        getline(file, line); // Skip the header line
    
        // Read each line and parse review fields
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
            r.rating = stoi(ratingStr);  // Convert rating to integer
            strcpy(r.reviewText, reviewText.c_str());
    
            reviews.add(r);  // Add to dynamic array
        }
    
        file.close();
    }
    
    // ---------- Main ----------
    int main() {
        // Run data cleaning for transactions and reviews (defined in DataCleaning.cpp)
        cleanTransactions("transactions.csv", "cleaner_transactions.csv");
        cleanReviews("reviews.csv", "cleaner_reviews.csv");
    
        // Create dynamic arrays to hold the cleaned data
        TransactionArray transactions;
        ReviewArray reviews;
    
        // Load cleaned data into custom arrays
        loadTransactionsFromFile("cleaner_transactions.csv", transactions);
        loadReviewsFromFile("cleaner_reviews.csv", reviews);
    
        // ---------- Question 1 ----------
        cout << "Question 1\n";

        cout<<"Merge Sort Results\n\n";
        // Merge Sort
        // Sort transactions using merge sort (by date)
        mergeSort(transactions.getData(), 0, transactions.getSize() - 1, compareByDate);
    
        // Display sorted transactions
        /*
        cout << "\nSorted Transactions by Date:\n";
        for (int i = 0; i < transactions.getSize(); ++i) {
            Transaction t = transactions.getData()[i];
            cout << t.date << " - " << t.customerID << " - " << t.product
                 << " - $" << fixed << setprecision(2) << t.price << endl;
        }
    
        // Display total number of transactions and reviews
        cout << "\nTotal Transactions: " << transactions.getSize() << endl;
        cout << "Total Reviews: " << reviews.getSize() << endl;
        */
        //Sorting 2





        //Question 2
        cout << "\nQuestion 2\n";
        cout << "Binary Search\n\n";


        mergeSort(transactions.getData(), 0, transactions.getSize() - 1, compareByCategory);

        // Display sorted transactions
        cout << "\nSorted Transactions by Category:\n";
        int numOfCreditCard=0;
        int TotalOfElectronics=0;
        for (int i = 0; i < transactions.getSize(); ++i) {
            Transaction t = transactions.getData()[i];
            // Checks if category is Electronics
            if (strcmp(t.category, "Electronics") == 0) {
                TotalOfElectronics++;
                // Check if payment method is Credit Card
                if (strcmp(t.paymentMethod, "Credit Card") == 0) {
                    numOfCreditCard++;
                    cout << numOfCreditCard << ": "<< t.category << " - " << t.customerID << " - "<< t.paymentMethod << endl;
                }
            }
        }
    
    double percent = percentageWithCategoryAndMethod(transactions.getData(), transactions.getSize(),"Electronics","Credit Card");

    cout<<"\nTotal Number of purchases in transactions: "<< TotalOfElectronics <<"\nNumber Of purchases for electronics using credit cards: " << numOfCreditCard;
    cout << "\nPercentage of Electronics paid with Credit Card: "<< fixed << setprecision(2) << percent << "%\n";
        return 0;
    }
    