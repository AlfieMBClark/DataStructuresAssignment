
#ifndef LOAD_DATA_H
#define LOAD_DATA_H

#include <string>
#include "LoadData.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
using namespace std;

// Constants for array dimensions
static const int TransactionFields = 6;   
static const int ReviewFields      = 4;  
static const int FieldLength       = 255; 

char (*transactions)[TransactionFields][FieldLength] = nullptr;
int transactionCount = 0;

char (*reviews)[ReviewFields][FieldLength] = nullptr;
int reviewCount = 0;

void loadTransactions(const char* cleanedFile) {
    // Count lines 
    ifstream countFile(cleanedFile);
    if (!countFile) {
        cerr << "Couldn't open " << cleanedFile << "\n";
        return;
    }

    string line;
    transactionCount = 0;
    getline(countFile, line); // Skip header
    while (getline(countFile, line)) transactionCount++;
    countFile.close();

    //cout<<"Trans Rows: "<< transactionCount;

    if (transactionCount == 0) {
        cout << "No data trans.\n";
        return;
    }

    // Allocate
    transactions = new char[transactionCount][TransactionFields][FieldLength];
    // Load data 
    ifstream fin(cleanedFile);
    if (!fin) {
        cerr << "Couldn't reopen " << cleanedFile << "\n";
        return;
    }

    getline(fin, line); // Skip header
    int row = 0;
    while (getline(fin, line) && row < transactionCount) {
        string fields[TransactionFields];
        stringstream ss(line);

        for (int i = 0; i < TransactionFields; ++i) {
            getline(ss, fields[i], (i < TransactionFields - 1) ? ',' : '\n');
            strncpy(transactions[row][i], fields[i].c_str(), FieldLength - 1);
            transactions[row][i][FieldLength - 1] = '\0';
        }
        row++;
    }

    fin.close();
    //cout << "Trans Data Loaded: "<<row;
}

void loadReviews(const char* cleanedFile) {
    // Count rows
    ifstream countFile(cleanedFile);
    if (!countFile) {
        cerr << "Couldn't open " << cleanedFile;
    }

    string line;
    reviewCount = 0;
    getline(countFile, line); // Skip header
    while (getline(countFile, line)) reviewCount++;
    countFile.close();
    //cout<<"Rev Rows: "<< reviewCount;

    if (reviewCount == 0) {
        cout << "No review data found.\n";
        return;
    }

    // Allocate
    reviews = new char[reviewCount][ReviewFields][FieldLength];

    // Load data
    ifstream fin(cleanedFile);
    if (!fin) {
        cerr << "Couldn't reopen " << cleanedFile ;
        return;
    }

    getline(fin, line); // Skip
    int row = 0;
    while (getline(fin, line) && row < reviewCount) {
        string fields[ReviewFields];
        stringstream ss(line);

        for (int i = 0; i < ReviewFields; ++i) {
            getline(ss, fields[i], (i < ReviewFields - 1) ? ',' : '\n');
            strncpy(reviews[row][i], fields[i].c_str(), FieldLength - 1);
            reviews[row][i][FieldLength - 1] = '\0';
        }
        row++;
    }

    fin.close();
    //cout << "Rev Data Loaded: "<<row;
}

#endif 