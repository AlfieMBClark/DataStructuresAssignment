#include "LoadData.h"

#include <iostream>
#include <iomanip>
#include <cstring>
#include <chrono>  
#include <cctype>

using namespace std;
using Clock = chrono::high_resolution_clock;


int main(){
    
    TransactionNode* txHead = loadTransactions("transactions_cleaned.csv");
    ReviewNode*      rvHead = loadReviews    ("reviews_cleaned.csv");
    if (!txHead || !rvHead) {
        cerr << "Error loading data\n";
        return 1;
    }
    cout << "Loaded " << listLength(txHead) << " transactions and "<< listLength(rvHead) << " reviews.\n";

    /*
    //Prints out all the data
    cout << "\nAll loaded transactions:\n";
    cout << "CustID | Product | Category | Price | Date | Payment\n";
    for (TransactionNode* cur = txHead; cur != nullptr; cur = cur->next) {
        auto &t = cur->data;
        cout 
        << t.customerID << " | "
        << t.product    << " | "
        << t.category   << " | "
        << t.price      << " | "
        << t.date       << " | "
        << t.payment    << "\n";
    }

    // 4) Print every review
    cout << "\nAll loaded reviews:\n";
    cout << "ProdID | CustID | Rating | Text\n";
    for (ReviewNode* cur = rvHead; cur != nullptr; cur = cur->next) {
        auto &r = cur->data;
        cout 
        << r.productID  << " | "
        << r.customerID << " | "
        << r.rating     << " | "
        << r.text       << "\n";
    }
    */

   
    

    //Question 1
    //Merge Sort By Date
    
    return 0;
}