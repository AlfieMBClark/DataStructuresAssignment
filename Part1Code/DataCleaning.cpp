#include "DataCleaning.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

using namespace std;

// Storage
char transactions[Rows][TransactionFields][FieldLength];
int  transactionCount = 0;

char reviews[Rows][ReviewFields][FieldLength];
int  reviewCount      = 0;

// Trim whitespace
static string trim(const string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    size_t b = s.find_last_not_of(" \t\r\n");
    return (a==string::npos) ? "" : s.substr(a, b-a+1);
}

// Remove a character
static string removeChar(const string& s, char ch) {
    string out; out.reserve(s.size());
    for (char c : s) if (c!=ch) out+=c;
    return out;
}

void cleanTransactions(const char* inputFile) {
    ifstream file(inputFile);
    if (!file.is_open()) {
        cerr<<"Failed to open "<<inputFile<<"\n";
        return;
    }
    string line;
    getline(file, line); // skip header
    while (getline(file, line)) {
        string fields[TransactionFields];
        stringstream ss(line);
        for (int i = 0; i < TransactionFields; ++i)
            getline(ss, fields[i], (i< TransactionFields-1)? ',' : '\n'),
            fields[i] = trim(fields[i]);

        // skip any empty
        bool bad = false;
        for (auto &f: fields) if (f.empty()) { bad=true; break; }
        if (bad) continue;

        // validate price
        string p = removeChar(removeChar(fields[3],'$'),',');
        try { stod(p); } catch(...) { continue; }

        // reformat date dd/mm/yyyy → yyyy-mm-dd
        if (fields[4].size()==10 && fields[4][2]=='/'&&fields[4][5]=='/') {
            fields[4] = fields[4].substr(6,4)+"-"
                      + fields[4].substr(3,2)+"-"
                      + fields[4].substr(0,2);
        } else continue;

        // copy into array
        for (int j=0; j<TransactionFields; ++j) {
            strncpy(transactions[transactionCount][j],
                    fields[j].c_str(),
                    FieldLength-1);
            transactions[transactionCount][j][FieldLength-1] = '\0';
        }
        ++transactionCount;
    }
    file.close();
}

void cleanReviews(const char* inputFile) {
    ifstream file(inputFile);
    if (!file.is_open()) {
        cerr<<"Failed to open "<<inputFile<<"\n";
        return;
    }
    string line;
    getline(file, line); // skip header
    while (getline(file, line)) {
        string fields[ReviewFields];
        stringstream ss(line);
        for (int i=0; i<ReviewFields; ++i)
            getline(ss, fields[i], (i<ReviewFields-1)? ',' : '\n'),
            fields[i] = trim(fields[i]);

        bool bad=false;
        for (auto &f: fields) if (f.empty()) { bad=true; break; }
        if (bad) continue;

        // validate rating
        try {
            int r = stoi(fields[2]);
            if (r<1||r>5) continue;
        } catch(...) { continue; }

        // strip quotes from review text
        fields[3] = removeChar(fields[3], '"');

        for (int j=0; j<ReviewFields; ++j) {
            strncpy(reviews[reviewCount][j],
                    fields[j].c_str(),
                    FieldLength-1);
            reviews[reviewCount][j][FieldLength-1] = '\0';
        }
        ++reviewCount;
    }
    file.close();
}
