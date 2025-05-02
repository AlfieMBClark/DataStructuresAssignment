#include "DataCleaning.h"
#include "LoadData.h"
#include "SortAlgorithms.h"
#include "SearchAlgorithms.h"

#include <iostream>
#include <iomanip>
#include <cstring>
#include <chrono>  
#include <cctype>

using namespace std;
using Clock = chrono::high_resolution_clock;


void Alfie(){
    // Load data into dynamically sized arrays
    loadTransactions("cleaned_transactions.csv");
    loadReviews     ("cleaned_reviews.csv");
    
    cout << "Working with " << transactionCount << " transactions and " << reviewCount << " reviews\n";


    // Question 1: Count transactions per day
    // Unique customer ID's
    char (*uniqueCustomerIDs)[FieldLength] = new char[transactionCount][FieldLength];
    int uIDCount = 0;

    for (int i = 0; i < transactionCount; ++i) {
        char* currentID = transactions[i][0];
        bool isNew = true;

        for (int j = 0; j < uIDCount; ++j) {
            if (strcmp(uniqueCustomerIDs[j], currentID) == 0) {
                isNew = false;
                break;
            }
        }

        if (isNew) {
            strncpy(uniqueCustomerIDs[uIDCount], currentID, FieldLength - 1);
            ++uIDCount;
        }
    }
    
    cout << "Found " << uIDCount << " unique customers.\n\n";
    

    //Merge Sort
    auto AlfieStartQ1 = Clock::now();
    mergeSortTransactions(transactions, 0, transactionCount - 1, 4);
    auto AlfieEndMergeSortQ1 = Clock::now();

    
    // Date+Count 2D array
    int uniqueDatesCount = 1; 
    string currentDate = transactions[0][4];

    for (int i = 1; i < transactionCount; ++i) {
        if (strcmp(transactions[i][4], currentDate.c_str()) != 0) {
            uniqueDatesCount++;
            currentDate = transactions[i][4];
        }
    }

    char (*dateTransactions)[2][FieldLength] = new char[uniqueDatesCount][2][FieldLength];
    
    //Count transactions
    currentDate = transactions[0][4];
    int dailyCount = 0;
    int arrayIndex = 0;

    cout << "\nTransactions by Date:\n";
    for (int i = 0; i < transactionCount; ++i) {
        string date = transactions[i][4];
        if (date == currentDate) {
            dailyCount++;
        } else {
            strcpy(dateTransactions[arrayIndex][0], currentDate.c_str());
            sprintf(dateTransactions[arrayIndex][1], "%d", dailyCount);
            arrayIndex++;
            cout << currentDate << ":\t" << dailyCount << "\n";
            currentDate = date;
            dailyCount = 1;
        }
    }

    strcpy(dateTransactions[arrayIndex][0], currentDate.c_str());
    sprintf(dateTransactions[arrayIndex][1], "%d", dailyCount);
    cout << currentDate << ":\t" << dailyCount << "\n";

    cout << "Total: " << transactionCount << "\n";



    // Transaction + Reviews Array
    char (*TransReviewArray)[2][FieldLength] = new char[transactionCount][2][FieldLength];
    int pairCount = 0;
    int totalTransactionsTR = 0;
    int totalReviewsTR = 0;
    
    for (int customerIndex = 0; customerIndex < uIDCount; ++customerIndex) {
        char* customerID = uniqueCustomerIDs[customerIndex];
        // Transaction per cust
        for (int i = 0; i < transactionCount; ++i) {
            if (strcmp(transactions[i][0], customerID) == 0) {
                ++totalTransactionsTR;
            }
        }
        //reviews per cust
        for (int i = 0; i < reviewCount; ++i) {
            if (strcmp(reviews[i][1], customerID) == 0) {
                ++totalReviewsTR;
            }
        }
        // Skip customers with more reviews than transactions
        if (totalReviewsTR > totalTransactionsTR) continue;
    
        for (int i = 0; i < transactionCount; ++i) {
            if (strcmp(transactions[i][0], customerID) == 0) {
                strcpy(TransReviewArray[pairCount][0], transactions[i][4]);
                strcpy(TransReviewArray[pairCount][1], customerID);
                ++pairCount;
            }
        }
    }

    //cout << TransReviewArray[1][0]<<"AND"<<TransReviewArray[1][1];
    //cout << "TR" << totalReviewsTR <<"TT"<< totalTransactionsTR;

    // Sort by date
    mergeSortTransReviewArray(TransReviewArray, 0, pairCount - 1);

    // for (int i=0; i<pairCount; i++){
    //     cout << TransReviewArray[i][0] << "And" <<TransReviewArray[i][1]<<endl;
    // }

    
    //  2D array for storing dates and counts
    char (*revDateCounts)[2][FieldLength] = new char[uniqueDatesCount][2][FieldLength];

    //Count Trans per day in TransReview Data
    char currentRevDate[FieldLength];
    strcpy(currentRevDate, TransReviewArray[0][0]);
    int dailyRevCount = 1;
    int TRAindex = 0;

    for (int i = 1; i < pairCount; ++i) {
        if (strcmp(TransReviewArray[i][0], currentRevDate) == 0) {
            dailyRevCount++;
        } else {
            strcpy(revDateCounts[TRAindex][0], currentRevDate);
            sprintf(revDateCounts[TRAindex][1], "%d", dailyRevCount);
            TRAindex++;
            //cout << currentRevDate << ":\t" << dailyRevCount << "\n";
            strcpy(currentRevDate, TransReviewArray[i][0]);
            dailyRevCount = 1;
        }
    }
    strcpy(revDateCounts[TRAindex][0], currentRevDate);
    sprintf(revDateCounts[TRAindex][1], "%d", dailyRevCount);

    //Compare
    cout << "\nComparing Trans+Review Transactions to Transactions:\n";
    int matchingDates = 0;
    int matchingCounts = 0;
    int NotmatchingDates = 0;
    int NotmatchingCounts = 0;
    bool FullMatch = true;

    for (int i = 0; i < uniqueDatesCount; i++) {
        for(int j=0; j<uniqueDatesCount; j++){
            if (strcmp(dateTransactions[i][0], revDateCounts[j][0]) == 0) {
                //cout<<dateTransactions[i][0]<<"\t";
                matchingDates++;
                if (strcmp(dateTransactions[i][1], revDateCounts[j][1]) == 0) {
                    //cout<<dateTransactions[i][1]<<endl;
                    matchingCounts++;
                } else {
                    FullMatch = false;
                    NotmatchingCounts++;
                }
            }
        }
    }
    
    if(FullMatch){
        cout <<"Full Match! Total Dates: "<<uniqueDatesCount<< "\tMatching Dates:"<< matchingDates << "\tMatching Counts\t" << matchingCounts<<endl;
    }else{
        cout << "Not Full match\t"<< "UnMatching Dates:"<< NotmatchingDates << "\tUnMatching Counts\t" << NotmatchingCounts<<endl;
    }
    auto AlfieEnd2Q1 = Clock::now();

    cout << "Merge Sort time: " << chrono::duration_cast<chrono::milliseconds>(AlfieEndMergeSortQ1 - AlfieStartQ1).count() << " ms\n";
    cout << "Entire Merge Sort Implementation Q1 Algorithm Time: " << chrono::duration_cast<chrono::milliseconds>(AlfieEnd2Q1 - AlfieStartQ1).count() << " ms\n\n";

    //Algorithms Here



    // Question 2: Electronics and Credit Card Analysis
    cout<<"Question 2\n";
    auto AlfieStartFullQ2 = Clock::now();
    mergeSortTransactions(transactions, 0, transactionCount - 1, 2);
    auto AlfieStartBinaryQ2 = Clock::now();
    int idx = binarySearch(transactions, transactionCount, "Electronics", 2);

    if (idx < 0) {
        cout << "\nDidn't find any Electronics transactions.\n";
        return;
    }

    int firstQ2Alfie = idx, lastQ2Alfie = idx;
    while (firstQ2Alfie > 0 && strcmp(transactions[firstQ2Alfie - 1][2], "Electronics") == 0) --firstQ2Alfie;
    while (lastQ2Alfie + 1 < transactionCount && strcmp(transactions[lastQ2Alfie + 1][2], "Electronics") == 0) ++lastQ2Alfie;
    auto AlfieEndBinaryQ2 = Clock::now();

    int credit = 0;
    for (int i = firstQ2Alfie; i <= lastQ2Alfie; ++i) {
        if (strcmp(transactions[i][5], "Credit Card") == 0)
            ++credit;
    }

    int total = lastQ2Alfie - firstQ2Alfie + 1;
    double percent = (double)credit / total * 100;
    auto AlfieEndfullQ2 = Clock::now();

    // cout << "\nList of Electronics purchases using Credit Card:\n";
    // for (int i = firstQ2Alfie; i <= lastQ2Alfie; ++i) {
    //     if (strcmp(transactions[i][5], "Credit Card") == 0) {
    //         for (int j = 0; j < TransactionFields; ++j) {
    //             cout << transactions[i][j];
    //             if (j < TransactionFields - 1) cout << ", ";
    //         }
    //         cout << "\n";
    //     }
    // }
    
    cout << "\nElectronics purchases:\n";
    cout << "Total: " << total << ", using Credit Card: " << credit << "\nPercentage = " << fixed << setprecision(2) << percent << "%\n";

    cout << "Binary Search Process for all 1 star Reviews: " << chrono::duration_cast<chrono::nanoseconds>(AlfieEndBinaryQ2 - AlfieStartBinaryQ2).count() << " nano seconds\n";
    cout << "Entire Q2 Implementation BinarySearch: " << chrono::duration_cast<chrono::milliseconds>(AlfieEndfullQ2 - AlfieStartFullQ2).count() << " ms\n\n";


    // Question 3: Word frequency in 1-star reviews
    cout<<"\n\nQuestion 3\n";
    auto startAlfieQ3 = Clock::now();

    mergeSortReviews(reviews, 0, reviewCount - 1, 2);
    int found = binarySearchReviews(reviews, reviewCount, "1", 2);

    if (found < 0) {
        cout << "No 1-star\n";
        return;
    }

    int firstQ3Alfie = found, lastQ3Alfie = found;
    while (firstQ3Alfie > 0 && strcmp(reviews[firstQ3Alfie - 1][2], "1") == 0) --firstQ3Alfie;
    while (lastQ3Alfie + 1 < reviewCount && strcmp(reviews[lastQ3Alfie + 1][2], "1") == 0) ++lastQ3Alfie;

    int total1Stars = lastQ3Alfie - firstQ3Alfie + 1;
    cout << "\n1-Star Reviews Word Count\n";
    cout << "Reviews found: " << total1Stars << "\n";

    // Use dynamic allocation for word counting
    int MAX_WORDS = total1Stars*50;
    char (*words)[FieldLength] = new char[MAX_WORDS][FieldLength];
    int *counts = new int[MAX_WORDS]();
    int unique = 0;

    for (int i = firstQ3Alfie; i <= lastQ3Alfie; ++i) {
        char tmp[FieldLength];
        strcpy(tmp, reviews[i][3]);

        for (int j = 0; tmp[j]; ++j) {
            tmp[j] = isalpha((unsigned char)tmp[j]) ? tolower((unsigned char)tmp[j]) : ' ';
        }

        for (char* token = strtok(tmp, " "); token; token = strtok(nullptr, " ")) {
            int match = -1;
            for (int w = 0; w < unique; ++w) {
                if (strcmp(words[w], token) == 0) {
                    match = w;
                    break;
                }
            }
            if (match >= 0) {
                counts[match]++;
            } else if (unique < MAX_WORDS) {
                strcpy(words[unique], token);
                counts[unique] = 1;
                ++unique;
            }
        }
    }

    // WordCount array 
    WordCount *wc = new WordCount[unique];
    for (int i = 0; i < unique; ++i) {
        strcpy(wc[i].word, words[i]);
        wc[i].count = counts[i];
    }

    WordCount* wcBuffer = new WordCount[unique];
    mergeSortWordCounts(wc, 0, unique - 1, wcBuffer);
    auto endAlfieQ3 = Clock::now();
    auto durAlfieQ3 = chrono::duration_cast<chrono::milliseconds>(endAlfieQ3 - startAlfieQ3);

    for (int i = 0; i < unique; ++i) {
        cout << wc[i].word << ": " << wc[i].count << "\n";
    }

    cout << "Q3 using merge Sort and Binary Search was completed in:" << durAlfieQ3.count() << " ms\n";

}


int main(){
    // Clean Data
    cleanTransactions("transactions.csv");
    cleanReviews    ("reviews.csv");

    int choice;

    do {
        cout << "\n===== MENU =====\n";
        cout << "1. Merge Sort + Binary Search (Alfiansyah Clark - TP075566)\n";
        cout << "2. Place 2\n";
        cout << "3. Place 3\n";
        cout << "4. Place 4\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "\nMerge Sort & Binary Search\n";
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