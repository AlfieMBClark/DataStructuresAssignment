#include "LoadData.h"

#include <iostream>
#include <iomanip>
#include <cstring>
#include <chrono>  
#include <cctype>

using namespace std;
using Clock = chrono::high_resolution_clock;


void Alfie(){
    const string transactionsFile = "transactions_cleaned.csv";
    const string reviewsFile = "reviews_cleaned.csv";

    TransactionNode* txHead = loadTransactions(transactionsFile);
    ReviewNode* rvHead = loadReviews(reviewsFile);

    if (!txHead || !rvHead) {
        std::cerr << "Error loading data\n";
        return;
    }

    int totalTransactions = 0;
    int totalReviews = 0;

    //cout << "All loaded transactions:\n";
    //cout << "CustID | Product | Category | Price | Date | Payment\n";
    for (TransactionNode* cur = txHead; cur != nullptr; cur = cur->next) {
        const auto& t = cur->data;
        //cout << t.customerID << " | " << t.product << " | "<< t.category << " | " << t.price << " | "<< t.date << " | " << t.paymentMethod << "\n";
        totalTransactions++;
    }

    //cout << "\nAll loaded reviews:\n";
    //cout << "ProdID | CustID | Rating | Text\n";
    for (ReviewNode* cur = rvHead; cur != nullptr; cur = cur->next) {
        const auto& r = cur->data;
        //cout << r.productID << " | " << r.customerID << " | "<< r.rating << " | " << r.reviewText << "\n";
        totalReviews++;
    }

    cout << "\nTotal Reviews: " << totalReviews<< "\tTotal Transactions: " << totalTransactions << "\n";

    //Clena
    while (txHead) {
        TransactionNode* temp = txHead;
        txHead = txHead->next;
        delete temp;
    }

    while (rvHead) {
        ReviewNode* temp = rvHead;
        rvHead = rvHead->next;
        delete temp;
    }
    cout << "Merge Sort & Binary Search\n";

    //Question 1
    cout << "Question 1: Sort by date.\n\n";
}


int main(){
    
    int choice;

    do {
        cout << "\n===== MENU =====\n";
        cout << "1. Merge Sort + Binary Search\n";
        cout << "2. Place 2\n";
        cout << "3. Place 3\n";
        cout << "4. Place 4\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "\nRunning Merge Sort and Binary Search...\n";
                Alfie();
                break;
            case 2:
                cout << "\nPlace 2\n";
                break;
            case 3:
                cout << "\nPlace 3\n";
                break;
            case 4:
                cout << "\nPlace 4\n";
                break;
            case 5:
                cout << "\nExiting...\n";
                return 0;
                break;
            default:
                cout << "\nInvalid choice. Please try again.\n";
        }

    } while (choice != 0);

}