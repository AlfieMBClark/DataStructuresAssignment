#include "DataCleaning.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstring>

using namespace std;

// Removes all occurrences of a specific character from a string
string removeChar(const string& str, char ch) {
    string result = "";
    for (char c : str) {
        if (c != ch) result += c;
    }
    return result;
}

// Trims whitespace from the beginning and end of a string
string trim(const string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");
    return (start == string::npos) ? "" : str.substr(start, end - start + 1);
}

// Cleans the transactions CSV by removing invalid rows, formatting data, and writing a cleaned file
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

        // Extract each field from the CSV line
        if (!getline(ss, customerID, ',')) continue;
        if (!getline(ss, product, ',')) continue;
        if (!getline(ss, category, ',')) continue;
        if (!getline(ss, priceStr, ',')) continue;
        if (!getline(ss, date, ',')) continue;
        if (!getline(ss, paymentMethod)) continue;

        // Clean and trim each field
        customerID = trim(customerID);
        product = trim(product);
        category = trim(category);
        priceStr = trim(removeChar(priceStr, '$'));   // Remove currency symbol
        priceStr = removeChar(priceStr, ',');          // Remove thousand separator
        date = trim(date);
        paymentMethod = trim(paymentMethod);

        // Skip if any field is empty
        if (customerID.empty() || product.empty() || category.empty() ||
            priceStr.empty() || date.empty() || paymentMethod.empty()) {
            skipped++;
            continue;
        }

        // Convert price to double, skip on error
        double price;
        try {
            //String to Double
            price = stod(priceStr);
        } catch (...) {
            skipped++;
            continue;
        }

        // Reformat date to yyyy-mm-dd
        if (date.length() == 10 && date[2] == '/' && date[5] == '/') {
            date = date.substr(6, 4) + "-" + date.substr(3, 2) + "-" + date.substr(0, 2);
        } else {
            skipped++;
            continue;
        }

        // Write the cleaned row to the output file
        outFile << customerID << "," << product << "," << category << ","
                << fixed << setprecision(2) << price << "," << date << "," << paymentMethod << "\n";
    }

    cout << "Cleaned transactions saved. Skipped: " << skipped << " rows.\n";
    inFile.close();
    outFile.close();
}

// Cleans the reviews CSV by removing invalid rows, formatting data, and writing a cleaned file
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

        // Extract each field from the CSV line
        if (!getline(ss, productID, ',')) continue;
        if (!getline(ss, customerID, ',')) continue;
        if (!getline(ss, ratingStr, ',')) continue;
        if (!getline(ss, reviewText)) continue;

        // Clean and trim each field
        productID = trim(productID);
        customerID = trim(customerID);
        ratingStr = trim(ratingStr);
        reviewText = trim(removeChar(reviewText, '"')); // Remove quotation marks

        // Skip if any field is empty
        if (productID.empty() || customerID.empty() || ratingStr.empty() || reviewText.empty()) {
            skipped++;
            continue;
        }

        // Convert rating to integer and validate it
        int rating;
        try {
            //String to Integer
            rating = stoi(ratingStr);
            if (rating < 1 || rating > 5) throw std::invalid_argument("Invalid");
        } catch (...) {
            skipped++;
            continue;
        }

        // Write the cleaned row to the output file
        outFile << productID << "," << customerID << "," << rating << "," << reviewText << "\n";
    }

    cout << "Cleaned reviews saved. Skipped: " << skipped << " rows.\n";
    inFile.close();
    outFile.close();
}
