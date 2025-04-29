#include "DataCleaning.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
using namespace std;

// Initialize our global variables
char (*transactions)[TransactionFields][FieldLength] = nullptr;
int transactionCount = 0;

char (*reviews)[ReviewFields][FieldLength] = nullptr;
int reviewCount = 0;

void loadTransactions(const char* cleanedFile) {
    // First pass: count the records
    ifstream countFile(cleanedFile);
    if (!countFile.is_open()) {
        cerr << "Failed to open " << cleanedFile << " for counting\n";
        return;
    }

    string line;
    transactionCount = 0;
    getline(countFile, line);  // Skip header
    
    while (getline(countFile, line)) {
        transactionCount++;
    }
    countFile.close();
    
    cout << "Found " << transactionCount << " transactions\n";
    
    // Clean up any previous allocation
    if (transactions != nullptr) {
        delete[] transactions;
        transactions = nullptr;
    }
    
    // Allocate the exact amount of memory needed
    if (transactionCount > 0) {
        transactions = new char[transactionCount][TransactionFields][FieldLength];
    } else {
        cout << "No transactions to load\n";
        return;
    }

    // Second pass: actually load the data
    ifstream fin(cleanedFile);
    if (!fin.is_open()) {
        cerr << "Failed to open " << cleanedFile << " for reading\n";
        return;
    }

    getline(fin, line);  // Skip header
    int index = 0;
    
    while (getline(fin, line) && index < transactionCount) {
        string fields[TransactionFields];
        stringstream ss(line);
        
        for (int i = 0; i < TransactionFields; ++i) {
            if (i < TransactionFields - 1)
                getline(ss, fields[i], ',');
            else
                getline(ss, fields[i]);
            
            strncpy(transactions[index][i], fields[i].c_str(), FieldLength - 1);
            transactions[index][i][FieldLength - 1] = '\0';
        }
        
        index++;
    }
    
    fin.close();
    cout << "Loaded " << index << " transactions\n";
}

void loadReviews(const char* cleanedFile) {
    // First pass: count the records
    ifstream countFile(cleanedFile);
    if (!countFile.is_open()) {
        cerr << "Failed to open " << cleanedFile << " for counting\n";
        return;
    }

    string line;
    reviewCount = 0;
    getline(countFile, line);  // Skip header
    
    while (getline(countFile, line)) {
        reviewCount++;
    }
    countFile.close();
    
    cout << "Found " << reviewCount << " reviews\n";
    
    // Clean up any previous allocation
    if (reviews != nullptr) {
        delete[] reviews;
        reviews = nullptr;
    }
    
    // Allocate the exact amount of memory needed
    if (reviewCount > 0) {
        reviews = new char[reviewCount][ReviewFields][FieldLength];
    } else {
        cout << "No reviews to load\n";
        return;
    }

    // Second pass: actually load the data
    ifstream fin(cleanedFile);
    if (!fin.is_open()) {
        cerr << "Failed to open " << cleanedFile << " for reading\n";
        return;
    }

    getline(fin, line);  // Skip header
    int index = 0;
    
    while (getline(fin, line) && index < reviewCount) {
        string fields[ReviewFields];
        stringstream ss(line);
        
        for (int i = 0; i < ReviewFields; ++i) {
            if (i < ReviewFields - 1)
                getline(ss, fields[i], ',');
            else
                getline(ss, fields[i]);
            
            strncpy(reviews[index][i], fields[i].c_str(), FieldLength - 1);
            reviews[index][i][FieldLength - 1] = '\0';
        }
        
        index++;
    }
    
    fin.close();
    cout << "Loaded " << index << " reviews\n";
}