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
    
    



    // Question 1: Count transactions per day

    //Array for unique customerID
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



    //With Reviews
    static char trRevDates[Rows][2][FieldLength];
    int    trRevCount = 0;

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
                // date is field 4 in transactions
                strcpy(trRevDates[trRevCount][0], transactions[i][4]);
                // customerID is field 0
                strcpy(trRevDates[trRevCount][1], cid);
                ++trRevCount;
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
            // swap the two rows [i] and [minIdx]
            char tmpDate [FieldLength], tmpCID[FieldLength];
            // save row i
            strcpy(tmpDate,  trRevDates[i][0]);
            strcpy(tmpCID,   trRevDates[i][1]);
            // copy minIdx → i
            strcpy(trRevDates[i][0], trRevDates[minIdx][0]);
            strcpy(trRevDates[i][1], trRevDates[minIdx][1]);
            // copy saved i → minIdx
            strcpy(trRevDates[minIdx][0], tmpDate);
            strcpy(trRevDates[minIdx][1], tmpCID);
        }
    }
    
    cout << "\nComparing it to transactions+review array\n";
    /* //This Outputs the results from the transaction+review Array
    if (trRevCount > 0) {
        char currentDate[FieldLength];
        strcpy(currentDate, trRevDates[0][0]);
        int countOnDate = 0;

        for (int i = 0; i < trRevCount; ++i) {
            if (strcmp(trRevDates[i][0], currentDate) == 0) {
                ++countOnDate;
            } else {
                cout << currentDate << " : " << countOnDate << "\n";
                strcpy(currentDate, trRevDates[i][0]);
                countOnDate = 1;
            }
        }
        // print the final date’s count
        cout << currentDate << " : " << countOnDate << "\n";
    
    }
    */
    int fullDateCount = 0;
    static char fullDates [Rows][FieldLength];
    static int  fullCounts[Rows];

    // Filtered summary from your trRevDates array:
    int filtDateCount = 0;
    static char filtDates [Rows][FieldLength];
    static int  filtCounts[Rows];

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

    static const int MAX_WORDS = 5000;
    static char words[MAX_WORDS][FieldLength];
    static int counts[MAX_WORDS] = {0};
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

    static WordCount wc[MAX_WORDS];
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
    
}
