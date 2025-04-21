#include "DataCleaning.h"
#include "SortAlgorithms.h"
#include "SearchAlgorithms.h"

#include <iostream>
#include <iomanip>
#include <cstring>
#include <chrono>  
#include <cctype>

using namespace std;
using Clock = chrono::high_resolution_clock;


int main(){
    // Clean Data
    cleanTransactions("transactions.csv");
    cleanReviews    ("reviews.csv");

    cout << "Loaded " << transactionCount << " transactions, " << reviewCount    << " reviews.\n";

    loadTransactions("cleaned_transactions.csv");
    loadReviews     ("cleaned_reviews.csv");

    //Question 1
    auto mergeSortStartTime = Clock::now();
    mergeSort(transactions, 0, transactionCount - 1, 4);

    auto mergeSortEndTime = Clock::now();
    auto mergesortTime = chrono::duration_cast<std::chrono::microseconds>(mergeSortEndTime - mergeSortStartTime);

    cout << "\nAll transactions sorted by date:\n";
    cout << "Date: CustomerID: Product: Category: Price : PaymentMethod\n";
    for (int i = 0; i < transactionCount; ++i) {
        cout << transactions[i][4] << ":"<< transactions[i][0] << ": "<< transactions[i][1] << " - "<< transactions[i][2] << " - $"<< transactions[i][3] << " - "<< transactions[i][5] << "\n";
    }

    cout << "\nTotal Transactions: " << transactionCount << "\n";
    cout << "Merge Sort took: " << mergesortTime.count() << " ms\n\n";

    //Question 2
    mergeSort(transactions, 0, transactionCount - 1, 2);

    
    auto binarySearchStartTime = Clock::now();

    int anyfound = binarySearch(transactions,transactionCount,"Electronics",2);
    
    auto binarySearchEndTime = Clock::now();
    auto binarySearchTime = std::chrono::duration_cast<chrono::nanoseconds>(binarySearchEndTime - binarySearchStartTime);
    
    if (anyfound < 0) {
        cout << "No Electronics records.\n";
        return 0;
    }


    int FirstInstance = anyfound, LastInstance = anyfound;

    //back
    while (FirstInstance > 0 && strcmp(transactions[FirstInstance-1][2], "Electronics") == 0) {
        --FirstInstance;
    }
    //forward
    while (LastInstance + 1 < transactionCount &&strcmp(transactions[LastInstance+1][2], "Electronics") == 0) {
        ++LastInstance;
    }

    int totalElectronics = LastInstance - FirstInstance + 1;
    int CreditCardCount = 0;
    cout << "\nElectronics purchases paid by Credit Card:\n";
    for (int i = FirstInstance; i <= LastInstance; ++i) {
        if (strcmp(transactions[i][5], "Credit Card") == 0) {
            ++CreditCardCount;
            cout <<  transactions[i][0]  << ": " << transactions[i][2] << ": "<< transactions[i][1] << ": $" << transactions[i][3] << ": "<< transactions[i][5]<<"\n"; 
        }
    }
    auto binarySearchAndFilterEndtime = Clock::now();
    auto BinaryandFiltertime = std::chrono::duration_cast<chrono::milliseconds>(binarySearchAndFilterEndtime - binarySearchStartTime);

    double PercentageForElectronicsCreditCard = (double)CreditCardCount/totalElectronics*100;
    cout <<"\nTotal Number of Purchases for Electronics: "<< totalElectronics;
    cout <<"\nTotal Number of Electronics with Credit Cards: "<< CreditCardCount;
    cout<< fixed << setprecision(2)<< "\nElectronics Purchases Using Credit Card: " << PercentageForElectronicsCreditCard << "%\n";
    cout << "binarySearch took: " << binarySearchTime.count() << " nano seconds\n";
    cout << "binarySearch And Filtering took: " << BinaryandFiltertime.count() << " ms\n";



    //Question 3
    mergeSortReviews(reviews, 0, reviewCount - 1, /*rating col=*/2);

    int idx1 = binarySearchReviews(reviews, reviewCount, "1", 2);
    if (idx1 >= 0) {
        int lo = idx1, hi = idx1;
        while (lo > 0 && strcmp(reviews[lo-1][2], "1") == 0) --lo;
        while (hi+1 < reviewCount && strcmp(reviews[hi+1][2], "1") == 0) ++hi;

    
    

    return 0;
}
