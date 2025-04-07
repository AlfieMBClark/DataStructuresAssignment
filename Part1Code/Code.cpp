#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <iomanip>

using namespace std;

// ---------- Transaction Structure ----------
//Defines what data type will be stored
struct Transaction {
    char customerID[50];
    char product[50];
    char category[50];
    double price;
    char date[20];
    char paymentMethod[30];

    //Compares if two transactions are identical
    bool equals(const Transaction& other) const {
        return strcmp(customerID, other.customerID) == 0 &&
               strcmp(product, other.product) == 0 &&
               strcmp(category, other.category) == 0 &&
               price == other.price &&
               strcmp(date, other.date) == 0 &&
               strcmp(paymentMethod, other.paymentMethod) == 0;
    }
};

// ---------- Review Structure ----------
//Defines what data type will be stored
struct Review {
    char productID[50];
    char customerID[50];
    int rating;
    char reviewText[255];

    //Compares if two reviews are identical
    bool equals(const Review& other) const {
        return strcmp(productID, other.productID) == 0 &&
               strcmp(customerID, other.customerID) == 0 &&
               rating == other.rating &&
               strcmp(reviewText, other.reviewText) == 0;
    }
};


//Function to removes all occurrences of a specific character from a string (e.g., $ or , in price).
string removeChar(const string& str, char ch) {
    string result = "";
    for (size_t i = 0; i < str.length(); ++i) {
        if (str[i] != ch) {
            result += str[i];
        }
    }
    return result;
}

// ---------- Transaction Array Class ----------
class TransactionArray {
private:

    //Data is dynamic array to hold transactions
    //Cpacity is max size before resizing
    //Size is current number of elemtns
    Transaction* data;
    int capacity;
    int size;

    //---Methods---
    //Resize doubles array size when full since we dont know how many rows will be valid (more efficient than increasing it by 1)
    void resize() {
        capacity *= 2;
        Transaction* newData = new Transaction[capacity];
        for (int i = 0; i < size; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
    }

    //Public Methods
public:
    TransactionArray(int initialCap = 100) : capacity(initialCap), size(0) {
        data = new Transaction[capacity];
    }

    ~TransactionArray() {
        delete[] data;
    }

    //Checks for duplicates
    bool exists(const Transaction& t) {
        for (int i = 0; i < size; i++) {
            if (data[i].equals(t)) return true;
        }
        return false;
    }

    //Adds unique transactions to the array
    void add(Transaction t) {
        if (size == capacity) resize();
        if (!exists(t)) {
            data[size++] = t;
        }
    }

    //---Getters---
    int getSize() const {
        return size;
    }

    Transaction* getData() const {
        return data;
    }
};

// ---------- Review Array Class ----------
class ReviewArray {
private:
    Review* data;
    int capacity;
    int size;

    void resize() {
        capacity *= 2;
        Review* newData = new Review[capacity];
        for (int i = 0; i < size; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
    }

public:
    ReviewArray(int initialCap = 100) : capacity(initialCap), size(0) {
        data = new Review[capacity];
    }

    ~ReviewArray() {
        delete[] data;
    }

    bool exists(const Review& r) {
        for (int i = 0; i < size; i++) {
            if (data[i].equals(r)) return true;
        }
        return false;
    }

    void add(Review r) {
        if (size == capacity) resize();
        if (!exists(r)) {
            data[size++] = r;
        }
    }

    int getSize() const {
        return size;
    }

    Review* getData() const {
        return data;
    }
};

// ---------- Trim Whitespace ----------
//Remove leading/trailing white space
string trim(const string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");
    return (start == string::npos) ? "" : str.substr(start, end - start + 1);
}

// ---------- Clean Transaction CSV ----------
void cleanTransactions(const char* inputFile, const char* outputFile) {
    //Opens input and out files
    ifstream inFile(inputFile);
    ofstream outFile(outputFile);
    if (!inFile || !outFile) {
        cout << "File error!" << endl;
        return;
    }

    string line;
    getline(inFile, line); // Skip header
    outFile << "customerID,product,category,price,date,paymentMethod" << endl;

    TransactionArray transactions;
    int skipped = 0;

    //Read each line in original csv File
    while (getline(inFile, line)) {
        stringstream ss(line);
        string customerID, product, category, priceStr, date, paymentMethod;

         //Extracts next portion of string using commas
        if (!getline(ss, customerID, ',')) continue;
        if (!getline(ss, product, ',')) continue;
        if (!getline(ss, category, ',')) continue;
        if (!getline(ss, priceStr, ',')) continue;
        if (!getline(ss, date, ',')) continue;
        if (!getline(ss, paymentMethod)) continue;
        //If no commas at the end it means row has been read
        //If missing field the further validation solves it


        //Trim fields
        customerID = trim(customerID);
        product = trim(product);
        category = trim(category);
        priceStr = trim(priceStr);
        date = trim(date);
        paymentMethod = trim(paymentMethod);

        //If any empty data, skip the line
        if (customerID.empty() || product.empty() || category.empty() ||
            priceStr.empty() || date.empty() || paymentMethod.empty()) {
            skipped++;
            continue;
        }

        // Remove $ and commas from price (10,000 becomes 10000)
        priceStr = removeChar(priceStr, '$');
        priceStr = removeChar(priceStr, ',');

        //Declares price as double
        double price;
        try {
            //stod to conver string to double
            price = stod(priceStr);
        } catch (...) {
            skipped++;
            continue;
        }

        // Reformat date to yyyy-mm-dd
        //Checks if dat string is exactly 10 characters long (eg 04/07/2025) with slashes at index 2 and 5
        if (date.length() == 10 && date[2] == '/' && date[5] == '/') {
            //If input is valid reformat
            //Extrace year, month and day and concatenate them in desired format
            date = date.substr(6,4) + "-" + date.substr(3,2) + "-" + date.substr(0,2);
        } else {
            skipped++;
            continue;
        }

        //Constructs Transaction, adds to array if unique
        Transaction t;
        strcpy(t.customerID, customerID.c_str());
        strcpy(t.product, product.c_str());
        strcpy(t.category, category.c_str());
        strcpy(t.paymentMethod, paymentMethod.c_str());
        strcpy(t.date, date.c_str());
        t.price = price;

        transactions.add(t);
    }
    
    //Writes all values in Transactions to file
    for (int i = 0; i < transactions.getSize(); i++) {
        Transaction t = transactions.getData()[i];
        outFile << t.customerID << "," << t.product << "," << t.category << ","
                << fixed << setprecision(2) << t.price << "," << t.date << "," << t.paymentMethod << endl;
    }

    cout << "Cleaned transactions written: " << transactions.getSize() << ", Skipped: " << skipped << endl;
}

// ---------- Clean Review CSV ----------
void cleanReviews(const char* inputFile, const char* outputFile) {
    //Opens input and out files
    ifstream inFile(inputFile);
    ofstream outFile(outputFile);
    if (!inFile || !outFile) {
        cout << "File error!" << endl;
        return;
    }

    string line;
    getline(inFile, line); // Skip header
    outFile << "productID,customerID,rating,reviewText" << endl;

    ReviewArray reviews;
    int skipped = 0;

    //Read each line in original csv File
    while (getline(inFile, line)) {

        //Wrap line in stringstream so it can be easily split by commas
        stringstream ss(line);
        //Decalre string variables
        string productID, customerID, ratingStr, reviewText;

        //Extracts next portion of string using commas
        if (!getline(ss, productID, ',')) continue;
        if (!getline(ss, customerID, ',')) continue;
        if (!getline(ss, ratingStr, ',')) continue;
        if (!getline(ss, reviewText)) continue;
        //If no commas at the end it means row has been read
        //If missing field the further validation solves it
        

        //Trim data
        productID = trim(productID);
        customerID = trim(customerID);
        ratingStr = trim(ratingStr);
        reviewText = trim(reviewText);

        //If any empty data, skip the line
        if (productID.empty() || customerID.empty() || ratingStr.empty() || reviewText.empty()) {
            skipped++;
            continue;
        }

        //Creates Rating data and ensure between 1 and 5
        int rating;
        try {
            //stor functions converts ratingStr to integer
            rating = stoi(ratingStr);
            if (rating < 1 || rating > 5) throw std::invalid_argument("out of range");
        } catch (...) {
            skipped++;
            continue;
        }

        //Constructs Reviews, adds to array if unique
        Review r;
        strcpy(r.productID, productID.c_str());
        strcpy(r.customerID, customerID.c_str());
        strcpy(r.reviewText, reviewText.c_str());
        r.rating = rating;

        reviews.add(r);
    }

    //Write to File
    for (int i = 0; i < reviews.getSize(); i++) {
        Review r = reviews.getData()[i];
        outFile << r.productID << "," << r.customerID << "," << r.rating << "," << r.reviewText << endl;
    }

    cout << "Cleaned reviews written: " << reviews.getSize() << ", Skipped: " << skipped << endl;
}


// Main function
int main() {
    cleanTransactions("C:\\Users\\alfie\\OneDrive\\Desktop\\DataStructuresAssignment\\Part1Code\\transactions.csv", "cleaner_transactions.csv");
    cleanReviews("C:\\Users\\alfie\\OneDrive\\Desktop\\DataStructuresAssignment\\Part1Code\\reviews.csv", "cleaner_reviews.csv");

    return 0;
}