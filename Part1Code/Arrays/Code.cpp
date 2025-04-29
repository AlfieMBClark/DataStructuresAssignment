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

int main(){
    // Clean Data
    cleanTransactions("transactions.csv");
    cleanReviews    ("reviews.csv");

    // Load data into dynamically sized arrays
    loadTransactions("cleaned_transactions.csv");
    loadReviews     ("cleaned_reviews.csv");
    
    cout << "Working with " << transactionCount << " transactions and " << reviewCount << " reviews\n";

    // Question 1: Count transactions per day

    // Count unique customer IDs and create the array in one pass
    char (*tempIDs)[FieldLength] = new char[transactionCount][FieldLength];
    int uniqueIds = 0;

    // First identify all unique IDs
    for (int i = 0; i < transactionCount; ++i) {
        char* currentID = transactions[i][0];
        bool exists = false;
        
        // Check if we've seen this ID before
        for (int j = 0; j < uniqueIds; ++j) {
            if (strcmp(tempIDs[j], currentID) == 0) {
                exists = true;
                break;
            }
        }
        
        if (!exists) {
            // Add to our temporary tracking array
            strncpy(tempIDs[uniqueIds], currentID, FieldLength-1);
            tempIDs[uniqueIds][FieldLength-1] = '\0';
            ++uniqueIds;
        }
    }

    // Now create the final array with the exact size we need
    char (*uniqueCustomerIDs)[FieldLength] = new char[uniqueIds][FieldLength];

    // Copy from temp array to final array
    for (int i = 0; i < uniqueIds; ++i) {
        strcpy(uniqueCustomerIDs[i], tempIDs[i]);
    }
    
    cout << "Found " << uniqueIds << " unique customers:\n\n";
    

    //Merge Sort
    auto start = Clock::now();
    mergeSort(transactions, 0, transactionCount - 1, 4);
    auto end = Clock::now();

    cout << "Sorted by date in " 
        << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms\n";

    if (transactionCount == 0) {
        cout << "No transactions available.\n";
        return 0;
    }

    string currentDate = transactions[0][4];
    int count = 0;

    cout << "\nTransactions by Date:\n";

    for (int i = 0; i < transactionCount; ++i) {
        string date = transactions[i][4];
        if (date == currentDate) {
            count++;
        } else {
            cout << currentDate << ":\t" << count << "\n";
            currentDate = date;
            count = 1;
        }
    }
    cout << currentDate << ":\t" << count << "\n";
    cout << "Total: " << transactionCount << "\n";



    //With Reviews - now using dynamic allocation
    char (*trRevDates)[2][FieldLength] = nullptr;
    int trRevCount = 0;

    // Allocate memory - can't be larger than transactions
    trRevDates = new char[transactionCount][2][FieldLength];

    for (int c = 0; c < uniqueIds; ++c) {
        char* cid = uniqueCustomerIDs[c];

        //count transaction
        int trCount = 0;
        for (int i = 0; i < transactionCount; ++i) {
            if (strcmp(transactions[i][0], cid) == 0)
                ++trCount;
        }

        //count Reviews
        int rvCount = 0;
        for (int i = 0; i < reviewCount; ++i) {
            if (strcmp(reviews[i][1], cid) == 0)
                ++rvCount;
        }

        // if they reviewed more than they transacted skip
        if (rvCount > trCount) continue;

        // add *all* their transaction dates + ID
        for (int i = 0; i < transactionCount; ++i) {
            if (strcmp(transactions[i][0], cid) == 0) {
                if (trRevCount < transactionCount) { // Safety check
                    // date is field 4 in transactions
                    strcpy(trRevDates[trRevCount][0], transactions[i][4]);
                    // customerID is field 0
                    strcpy(trRevDates[trRevCount][1], cid);
                    ++trRevCount;
                }
            }
        }
    }
    for (int i = 0; i < trRevCount - 1; ++i) {
        int minIdx = i;
        for (int j = i + 1; j < trRevCount; ++j) {
            if (strcmp(trRevDates[j][0], trRevDates[minIdx][0]) < 0) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            char tmpDate [FieldLength], tmpCID[FieldLength];
            strcpy(tmpDate,  trRevDates[i][0]);
            strcpy(tmpCID,   trRevDates[i][1]);
            strcpy(trRevDates[i][0], trRevDates[minIdx][0]);
            strcpy(trRevDates[i][1], trRevDates[minIdx][1]);
            strcpy(trRevDates[minIdx][0], tmpDate);
            strcpy(trRevDates[minIdx][1], tmpCID);
        }
    }
    
    cout << "\nComparing it to transactions+review array\n";

    //date analysis
    //Transaction Dates
    int fullDateCount = 0;
    char (*fullDates)[FieldLength] = nullptr;
    int *fullCounts = nullptr;

    // trRevDates
    int filtDateCount = 0;
    char (*filtDates)[FieldLength] = nullptr;
    int *filtCounts = nullptr;

    fullDates = new char[transactionCount][FieldLength];
    fullCounts = new int[transactionCount];
    
    filtDates = new char[transactionCount][FieldLength];
    filtCounts = new int[transactionCount];

    //Compare
    bool match = (fullDateCount == filtDateCount);
    if (match) {
        for (int i = 0; i < fullDateCount; ++i) {
            if (strcmp(fullDates[i], filtDates[i]) != 0 ||
                fullCounts[i] != filtCounts[i])
            {
                match = false;
                break;
            }
        }
    }

    if (match) {
        cout << "Outputs Match\n";
    } else {
        cout << "Outputs Do Not Match\n";
    }


    //Algorithms Here
    // Question 2: Electronics and Credit Card Analysis
    mergeSort(transactions, 0, transactionCount - 1, 2);
    int idx = binarySearch(transactions, transactionCount, "Electronics", 2);

    if (idx < 0) {
        cout << "\nDidn't find any Electronics transactions.\n";
        return 0;
    }

    int first = idx, last = idx;
    while (first > 0 && strcmp(transactions[first - 1][2], "Electronics") == 0) --first;
    while (last + 1 < transactionCount && strcmp(transactions[last + 1][2], "Electronics") == 0) ++last;

    int credit = 0;
    for (int i = first; i <= last; ++i) {
        if (strcmp(transactions[i][5], "Credit Card") == 0)
            ++credit;
    }

    int total = last - first + 1;
    double percent = (double)credit / total * 100;

    cout << "\nElectronics purchases:\n";
    cout << "Total: " << total << ", using Credit Card: " << credit << " (" << fixed << setprecision(2) << percent << "%)\n";

    //Algorithms Here

    // Question 3: Word frequency in 1-star reviews
    auto startQ3 = Clock::now();

    mergeSortReviews(reviews, 0, reviewCount - 1, 2);
    int found = binarySearchReviews(reviews, reviewCount, "1", 2);

    if (found < 0) {
        cout << "No 1-star reviews found.\n";
        return 0;
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

    // Allocate WordCount array dynamically
    WordCount *wc = new WordCount[unique];
    for (int i = 0; i < unique; ++i) {
        strcpy(wc[i].word, words[i]);
        wc[i].count = counts[i];
    }

    mergeSortWordCounts(wc, 0, unique - 1);
    auto endQ3 = Clock::now();
    auto durQ3 = chrono::duration_cast<chrono::milliseconds>(endQ3 - startQ3);

    for (int i = 0; i < unique; ++i) {
        cout << wc[i].word << ": " << wc[i].count << "\n";
    }

    cout << "Done in " << durQ3.count() << " ms\n";
    
    return 0;
}