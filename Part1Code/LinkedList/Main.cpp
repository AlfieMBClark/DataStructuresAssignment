#include "LoadData.h"

#include <iostream>
#include <iomanip>
#include <cstring>
#include <chrono>  
#include <cctype>

using namespace std;
using Clock = chrono::high_resolution_clock;


void Alfie(){
    cout << "Merge Sort & Binary Search\n";

    //Question 1
    cout << "Question 1: Sort by date.\n\n";
}


int main(){
    
    TransactionNode* txHead = loadTransactions("transactions_cleaned.csv");
    ReviewNode*      rvHead = loadReviews    ("reviews_cleaned.csv");
    if (!txHead || !rvHead) {
        cerr << "Error loading data\n";
        return 1;
    }
    cout << "Loaded " << listLength(txHead) << " transactions and "<< listLength(rvHead) << " reviews.\n";

    
    //Prints out all the data
    int totalTransactions =0;
    int totalReviews=0;
    //cout << "\nAll loaded transactions:\n";
    //cout << "CustID | Product | Category | Price | Date | Payment\n";
    for (TransactionNode* cur = txHead; cur != nullptr; cur = cur->next) {
        auto &t = cur -> data;
        cout << t.customerID << "|"<< t.product<< " | "<< t.category<< " | "<< t.price<< " | "<< t.date<< " | "<< t.paymentMethod<< "\n";
        totalTransactions++;
    }

    // 4) Print every review
    //cout << "\nAll loaded reviews:\n";
    //cout << "ProdID | CustID | Rating | Text\n";
    for (ReviewNode* cur = rvHead; cur != nullptr; cur = cur->next) {
        auto &r = cur->data;
        cout << r.productID  << " | "<< r.customerID << " | "<< r.rating     << " | "<< r.Reviewtext << "\n";
        totalReviews++;
    }
    
    cout << "Total Reviews: "<<totalReviews<<"\tTotal Transactions: "<<totalTransactions;

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
                break;
            default:
                cout << "\nInvalid choice. Please try again.\n";
        }

    } while (choice != 0);

   
    
    return 0;
}