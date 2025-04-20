#include "DataCleaning.h"
#include "SortAlgorithms.h"
#include "SearchAlgorithms.h"

#include <iostream>
#include <iomanip>
#include <cstring>
#include <cctype>
using namespace std;

int main(){
    // Clean Data
    cleanTransactions("transactions.csv");
    cleanReviews    ("reviews.csv");

    cout << "Loaded " << transactionCount << " transactions, " << reviewCount    << " reviews.\n";

    loadTransactions("cleaned_transactions.csv");
    loadReviews     ("cleaned_reviews.csv");

    //Question 1
    mergeSort(transactions, 0, transactionCount - 1, 4);

    cout << "\nAll transactions sorted by date:\n";
    cout << "Date: CustomerID: Product: Category: Price : PaymentMethod\n";
    for (int i = 0; i < transactionCount; ++i) {
        cout << transactions[i][4] << ":"<< transactions[i][0] << ": "<< transactions[i][1] << " - "<< transactions[i][2] << " - $"<< transactions[i][3] << " - "<< transactions[i][5] << "\n";
    }

    cout << "\nTotal Transactions: " << transactionCount << "\n";


    //Question 2
    mergeSort(transactions, 0, transactionCount - 1, 2);

    //Find 1 index
    int anyfound = binarySearch(transactions,transactionCount,"Electronics",2);
    if (anyfound < 0) {
        cout << "No Electronics records.\n";
        return 0;
    }


    int FirstInstance = anyfound, LastInstance = anyfound;

    //goes back from 1st index to find first instance
    while (FirstInstance > 0 && strcmp(transactions[FirstInstance-1][2], "Electronics") == 0) {
        --FirstInstance;
    }
    //goes forward from 1st index to find lastinstance
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

    double PercentageForElectronicsCreditCard = (double)CreditCardCount/totalElectronics*100;
    cout <<"\nTotal Number of Purchases for Electronics: "<< totalElectronics;
    cout <<"\nTotal Number of Electronics with Credit Cards: "<< CreditCardCount;
    cout<< fixed << setprecision(2)<< "\nElectronics Purchases Using Credit Card: " << PercentageForElectronicsCreditCard << "%\n";



    //Question 3
    //mergeSort(reviews, 0, reviewCount-1,2);



    return 0;
}
