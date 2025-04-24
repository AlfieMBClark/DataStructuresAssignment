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

    loadTransactions("cleaned_transactions.csv");
    loadReviews     ("cleaned_reviews.csv");
    
    char uniqueCustomerIDs[Rows][FieldLength];
    int uniqueIds = 0;

    for (int i=0; i<transactionCount; ++i){
        char* currentID = transactions[i][0];
        bool exists=false;
        for (int j =0; j<uniqueIds; ++j){
            if(strcmp(uniqueCustomerIDs[j], currentID)==0){
                exists=true;
                break;
            }
        }

        if(!exists){
            strncpy(uniqueCustomerIDs[uniqueIds], currentID, FieldLength-1);
            uniqueCustomerIDs[uniqueIds][FieldLength-1]='\0';
            ++uniqueIds;
        }
    }
    cout << "Found " << uniqueIds << " unique customers:\n\n";
    



    //Question 1
    auto mergeSortStartTime = Clock::now();
    mergeSort(transactions, 0, transactionCount - 1, 4);


    auto mergeSortEndTime = Clock::now();
    auto mergesortTime = chrono::duration_cast<std::chrono::microseconds>(mergeSortEndTime - mergeSortStartTime);

    
    if (transactionCount > 0) {
        std::string currentDate(transactions[0][4]);
        int countPerDay = 0;

        for (int i = 0; i < transactionCount; ++i) {
            std::string date(transactions[i][4]);
            if (date == currentDate) {
                ++countPerDay;
            } else {
                cout << currentDate << " : " << countPerDay << "\n";
                currentDate   = date;
                countPerDay   = 1;
            }
        }
        cout << currentDate << " : " << countPerDay << "\n";
    }

    cout << "\nTotal Transactions: " << transactionCount << "\n";
    cout << "Merge Sort took: " << mergesortTime.count() << "micro seconds\n\n";

    
    
    
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
    //sorting and finding 1 star Reviews
    auto Question3MergeAndBinaryStart = Clock::now();

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

    //Reading and counting words
    static char oneStarsArray[Rows][ReviewFields][FieldLength];
    int oneStarCount = LastReview - firstReview + 1;
    int Total1StarReviews=0;
    static const int MAX_WORDS = 5000;
    static char words[MAX_WORDS][FieldLength];
    static int  counts[MAX_WORDS];
    int uniqueWords = 0;

    cout << "\n1 Star Reviews:\n";
    for (int i = firstReview; i <= LastReview; ++i) {
        if (strcmp(reviews[i][2], "1") == 0) {
            ++Total1StarReviews;
        }
    }
    cout << "number of 1 stars:" << Total1StarReviews;
    
    // zero-initialize counts
    for (int w = 0; w < MAX_WORDS; ++w) {
        counts[w] = 0;
    }

    // walk each 1-star review’s text
    for (int idx = firstReview; idx <= LastReview; ++idx) {
        char tmp[FieldLength];
        strcpy(tmp, reviews[idx][3]);

        // normalize
        for (int i = 0; tmp[i]; ++i) {
            if (isalpha((unsigned char)tmp[i])) {
                tmp[i] = tolower((unsigned char)tmp[i]);
            } else {
                tmp[i] = ' ';
            }
        }

        // split on spaces
        for (char* token = strtok(tmp, " "); token; token = strtok(nullptr, " ")) {
            int w;
            // see if we already have this word
            for (w = 0; w < uniqueWords; ++w) {
                if (strcmp(words[w], token) == 0) {
                    counts[w]++;
                    break;
                }
            }
            // if it’s new, add it
            if (w == uniqueWords && uniqueWords < MAX_WORDS) {
                strcpy(words[uniqueWords], token);
                counts[uniqueWords] = 1;
                uniqueWords++;
            }
        }
    }

    //  WordCount array 
    static WordCount wc[MAX_WORDS];
    for (int i = 0; i < uniqueWords; ++i) {
        strcpy(wc[i].word,  words[i]);
        wc[i].count = counts[i];
    }
    mergeSortWordCounts(wc, 0, uniqueWords - 1);
    auto Question3MergeAndBinaryend = Clock::now();
    auto Question3MergeAndBinaryTotal = std::chrono::duration_cast<chrono::milliseconds>(Question3MergeAndBinaryend - Question3MergeAndBinaryStart);

    cout << "\nWord frequencies:\n";
    for (int i = 0; i < uniqueWords; ++i) {
        cout << wc[i].word << ": " << wc[i].count << "\n";
    }
    cout << "\nThe total time taken for merge sort and binary search was: "<< Question3MergeAndBinaryTotal.count() <<" milliseconds";

    return 0;
}
