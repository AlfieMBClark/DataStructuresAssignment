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
    loadReviews     ("cleaned_reviews.csv");
    mergeSortReviews(reviews, 0, reviewCount - 1, 2);


    int Found1StarReview = binarySearchReviews(reviews, reviewCount, "1", 2);
    if (Found1StarReview < 0) {
        return 0;
    }



    int firstReview = Found1StarReview, LastReview=Found1StarReview;
    while (firstReview>0 && strcmp(reviews[firstReview-1][2],"1")==0){
        --firstReview;
    }

    while(LastReview+1<reviewCount && strcmp(reviews[LastReview+1][2],"1")==0){
        ++LastReview;
    }


    cout << firstReview <<"\n";
    cout << LastReview <<"\n";

    static char oneStarsArray[Rows][ReviewFields][FieldLength];
    int oneStarCount = LastReview - firstReview + 1;
    int Total1StarReviews=0;

    cout << "\n1 Star Reviews:\n";
    for (int i = firstReview; i <= LastReview; ++i) {
        if (strcmp(reviews[i][2], "1") == 0) {
            ++Total1StarReviews;
            cout <<  reviews[i][0]  << ": " << reviews[i][1] << ": "<< reviews[i][2] << ": " << reviews[i][3] << "\n"; 
        }
    }
    cout << "\n number of 1 stars:" << Total1StarReviews;

    for(int i=firstReview; i<=LastReview; ++i){
        
    }
    

    return 0;
}
