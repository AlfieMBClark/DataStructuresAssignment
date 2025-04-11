#include "DataCleaning.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstring>

using namespace std;

string removeChar(const string& str, char ch) {
    string result = "";
    for (char c : str) {
        if (c != ch) result += c;
    }
    return result;
}

string trim(const string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");
    return (start == string::npos) ? "" : str.substr(start, end - start + 1);
}

void cleanTransactions(const char* inputFile, const char* outputFile) {
    ifstream inFile(inputFile);
    ofstream outFile(outputFile);
    if (!inFile || !outFile) {
        cout << "File error!" << endl;
        return;
    }

    string line;
    getline(inFile, line); // skip header
    outFile << "customerID,product,category,price,date,paymentMethod\n";

    int skipped = 0;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string customerID, product, category, priceStr, date, paymentMethod;

        if (!getline(ss, customerID, ',')) continue;
        if (!getline(ss, product, ',')) continue;
        if (!getline(ss, category, ',')) continue;
        if (!getline(ss, priceStr, ',')) continue;
        if (!getline(ss, date, ',')) continue;
        if (!getline(ss, paymentMethod)) continue;

        customerID = trim(customerID);
        product = trim(product);
        category = trim(category);
        priceStr = trim(removeChar(priceStr, '$'));
        priceStr = removeChar(priceStr, ',');
        date = trim(date);
        paymentMethod = trim(paymentMethod);

        if (customerID.empty() || product.empty() || category.empty() ||
            priceStr.empty() || date.empty() || paymentMethod.empty()) {
            skipped++;
            continue;
        }

        double price;
        try {
            price = stod(priceStr);
        } catch (...) {
            skipped++;
            continue;
        }

        if (date.length() == 10 && date[2] == '/' && date[5] == '/') {
            date = date.substr(6, 4) + "-" + date.substr(3, 2) + "-" + date.substr(0, 2);
        } else {
            skipped++;
            continue;
        }

        outFile << customerID << "," << product << "," << category << ","
                << fixed << setprecision(2) << price << "," << date << "," << paymentMethod << "\n";
    }

    cout << "Cleaned transactions saved. Skipped: " << skipped << " rows.\n";
    inFile.close();
    outFile.close();
}

void cleanReviews(const char* inputFile, const char* outputFile) {
    ifstream inFile(inputFile);
    ofstream outFile(outputFile);
    if (!inFile || !outFile) {
        cout << "File error!" << endl;
        return;
    }

    string line;
    getline(inFile, line); // skip header
    outFile << "productID,customerID,rating,reviewText\n";

    int skipped = 0;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string productID, customerID, ratingStr, reviewText;

        if (!getline(ss, productID, ',')) continue;
        if (!getline(ss, customerID, ',')) continue;
        if (!getline(ss, ratingStr, ',')) continue;
        if (!getline(ss, reviewText)) continue;

        productID = trim(productID);
        customerID = trim(customerID);
        ratingStr = trim(ratingStr);
        reviewText = trim(removeChar(reviewText, '"'));

        if (productID.empty() || customerID.empty() || ratingStr.empty() || reviewText.empty()) {
            skipped++;
            continue;
        }

        int rating;
        try {
            rating = stoi(ratingStr);
            if (rating < 1 || rating > 5) throw std::invalid_argument("Invalid");
        } catch (...) {
            skipped++;
            continue;
        }

        outFile << productID << "," << customerID << "," << rating << "," << reviewText << "\n";
    }

    cout << "Cleaned reviews saved. Skipped: " << skipped << " rows.\n";
    inFile.close();
    outFile.close();
}
