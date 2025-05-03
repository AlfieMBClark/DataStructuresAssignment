#include "LoadData.h"
#include "SortAlgorithmsLL.h"

#include <iostream>
#include <iomanip>
#include <cstring>
#include <chrono>  
#include <cctype>


using namespace std;
using Clock = chrono::high_resolution_clock;


void Alfie(){
    cout << "\nMerge Sort & Binary Search\n";
    Loader loader("transactions_cleaned.csv", "reviews_cleaned.csv");

    if (!loader.loadTrans()||!loader.loadRev()) {
        cerr << "Failed to load files";
        return;
    }

    // print trans data
    // TransactionNode* current = loader.getTransHead();
    // while (current != nullptr) {
    //     const Transaction& t = current->data;

    //     cout << "Customer ID: " << t.customerID
    //               << ", Product: " << t.product
    //               << ", Category: " << t.category
    //               << ", Price: " << t.price
    //               << ", Date: " << t.date
    //               << ", Payment Method: " << t.paymentMethod
    //               << std::endl;

    //     current = current->next;
    // }
    
    // Question 1
    cout << "Question 1: Sort by date.\n\n";
    
}

void Hadi() {

    //Question 1: Sorting transactions by date and counting them

    Loader loader("cleaned_transactions.csv", "cleaned_reviews.csv"); // Load data into linked lists
    loader.loadAll();

    auto bubblSortLLQ1Start = Clock::now(); // Start timer for Question 1
    TransactionNode* transHead = loader.getTransHead();
    bubbleSortTransactionsByDate(transHead); //Sorts linked list by date
    countTransactionsByDate(transHead, loader.getTransCount()); //Prints transaction counts by date
    auto bubbleSortLLQ1End = Clock::now();  // End timer for Question 1
    cout << "Bubble Sort completed in: "
         << chrono::duration_cast<chrono::milliseconds>(bubbleSortLLQ1End - bubblSortLLQ1Start).count() << " ms\n";
    }




int main(){
    
    int choice;

    do {
        cout << "\n \tMENU\t\n";
        cout << "1. Merge Sort + Binary Search\n";
        cout << "2. Place 2\n";
        cout << "3. Place 3\n";
        cout << "4. Bubble Sort + Jump Search 4\n";
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
                cout << "\nBubble Sort + Jump Search 4\n";
                Hadi();
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