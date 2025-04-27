#include "DataCleaning.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
using namespace std;

// Definitions of the globals declared in DataCleaning.h
char transactions[Rows][TransactionFields][FieldLength];
int  transactionCount = 0;

char reviews[Rows][ReviewFields][FieldLength];
int  reviewCount      = 0;

// Trim whitespace from both ends of a string
static string trim(const string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    size_t b = s.find_last_not_of (" \t\r\n");
    return (a == string::npos)
         ? ""
         : s.substr(a, b - a + 1);
}

// Remove all occurrences of `ch` from `s`
static string removeChar(const string& s, char ch) {
    string out;
    out.reserve(s.size());
    for (char c : s) {
        if (c != ch) out += c;
    }
    return out;
}

void cleanTransactions(const char* inputFile) {
    ifstream fin(inputFile);
    if (!fin.is_open()) {
        cerr << "Failed to open transactions input file: " << inputFile << "\n";
        return;
    }
    ofstream fout("cleaned_transactions.csv");
    if (!fout.is_open()) {
        cerr << "Failed to open cleaned_transactions.csv for writing\n";
        fin.close();
        return;
    }

    // Write header
    fout << "customerID,product,category,price,date,paymentMethod\n";

    string line;
    getline(fin, line);  // skip  header

    while (getline(fin, line)) {
        string fields[TransactionFields];
        stringstream ss(line);
        
        for (int i = 0; i < TransactionFields; ++i) {
            if (i < TransactionFields - 1)
                getline(ss, fields[i], ',');
            else
                getline(ss, fields[i]);
            fields[i] = trim(fields[i]);
        }

        bool bad = false;
        for (auto &f : fields) {
            if (f.empty()) { bad = true; break; }
        }
        if (bad) continue;

        // Validate and clean price
        string price = removeChar(removeChar(fields[3], '$'), ',');
        try {
            stod(price);
        } catch (...) {
            continue;
        }

        // Reformat date from dd/mm/yyyy to yyyy-mm-dd
        if (fields[4].size() == 10 &&fields[4][2] == '/' &&fields[4][5] == '/') {
            fields[4] = fields[4].substr(6,4) + "-" +fields[4].substr(3,2) + "-" +fields[4].substr(0,2);
        } else {
            continue;
        }
        fout<< fields[0] << ','<< fields[1] << ','<< fields[2] << ','<< price       << ','<< fields[4] << ','<< fields[5] << "\n";
    }

    fin.close();
    fout.close();
}

void cleanReviews(const char* inputFile) {
    ifstream fin(inputFile);
    if (!fin.is_open()) {
        cerr << "Failed to open reviews input file: " << inputFile << "\n";
        return;
    }
    ofstream fout("cleaned_reviews.csv");
    if (!fout.is_open()) {
        cerr << "Failed to open cleaned_reviews.csv for writing\n";
        fin.close();
        return;
    }

    // header
    fout << "productID,customerID,rating,reviewText\n";

    string line;
    getline(fin, line);  // skip header

    while (getline(fin, line)) {
        string fields[ReviewFields];
        stringstream ss(line);
        for (int i = 0; i < ReviewFields; ++i) {
            if (i < ReviewFields - 1)
                getline(ss, fields[i], ',');
            else
                getline(ss, fields[i]);
            fields[i] = trim(fields[i]);
        }

        bool bad = false;
        for (auto &f : fields) {
            if (f.empty()) { bad = true; break; }
        }
        if (bad) continue;

        // Validate rating
        try {
            int r = stoi(fields[2]);
            if (r < 1 || r > 5) continue;
        } catch (...) {
            continue;
        }

        // Strip quotation marks from review text
        fields[3] = removeChar(fields[3], '"');

        fout<< fields[0] << ','<< fields[1] << ','<< fields[2] << ','<< fields[3] << "\n";
    }

    fin.close();
    fout.close();
}

void loadTransactions(const char* cleanedFile) {
    ifstream fin(cleanedFile);
    if (!fin.is_open()) {
        cerr << "Failed to open " << cleanedFile << "\n";
        return;
    }

    string line;
    getline(fin, line);  // skip header
    transactionCount = 0;

    while (getline(fin, line) && transactionCount < Rows) {
        string fields[TransactionFields];
        stringstream ss(line);
        for (int i = 0; i < TransactionFields; ++i) {
            if (i < TransactionFields - 1)
                getline(ss, fields[i], ',');
            else
                getline(ss, fields[i]);
            strncpy(transactions[transactionCount][i],
                    fields[i].c_str(),
                    FieldLength - 1);
            transactions[transactionCount][i][FieldLength - 1] = '\0';
        }
        ++transactionCount;
    }

    fin.close();
}

void loadReviews(const char* cleanedFile) {
    ifstream fin(cleanedFile);
    if (!fin.is_open()) {
        cerr << "Failed to open " << cleanedFile << "\n";
        return;
    }

    string line;
    getline(fin, line);  // skip header
    reviewCount = 0;

    while (getline(fin, line) && reviewCount < Rows) {
        string fields[ReviewFields];
        stringstream ss(line);
        for (int i = 0; i < ReviewFields; ++i) {
            if (i < ReviewFields - 1)
                getline(ss, fields[i], ',');
            else
                getline(ss, fields[i]);
            strncpy(reviews[reviewCount][i],
                    fields[i].c_str(),
                    FieldLength - 1);
            reviews[reviewCount][i][FieldLength - 1] = '\0';
        }
        ++reviewCount;
    }

    fin.close();
}