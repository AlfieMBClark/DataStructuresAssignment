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
    // Load data
    loadTransactions("cleaned_transactions.csv");
    loadReviews     ("cleaned_reviews.csv");
    
    cout << "Transactions:"<< transactionCount<< "\tReviews: "<<reviewCount<<endl;


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
    
    cout << uIDCount << " unique customers.\n\n";
    

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
        // Skip cust with more reviews than transactions
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

    //Count words
    int maxWords = total1Stars * 50;
    char (*words)[FieldLength] = new char[maxWords][FieldLength];
    int* counts = new int[maxWords]();
    int foundWords = 0;

    // Count
    for (int i = firstQ3Alfie; i <= lastQ3Alfie; ++i) {
        char temp[FieldLength];
        strcpy(temp, reviews[i][3]);

        //lowercase, punct - space
        for (int j = 0; temp[j]; ++j) {
            if (isalpha((unsigned char)temp[j])) {
                temp[j] = tolower((unsigned char)temp[j]);
            } else {
                temp[j] = ' ';
            }
        }

        // split space and count
        char* word = strtok(temp, " ");
        while (word) {
            int existing = -1;

            for (int w = 0; w < foundWords; ++w) {
                if (strcmp(words[w], word) == 0) {
                    existing = w;
                    break;
                }
            }

            if (existing != -1) {
                counts[existing]++;
            } else if (foundWords < maxWords) {
                strcpy(words[foundWords], word);
                counts[foundWords] = 1;
                foundWords++;
            }

            word = strtok(nullptr, " ");
        }
    }

    //WordCount struct
    WordCount* wc = new WordCount[foundWords];
    for (int i = 0; i < foundWords; ++i) {
        strcpy(wc[i].word, words[i]);
        wc[i].count = counts[i];
    }

    WordCount* wcBuffer = new WordCount[foundWords];
    mergeSortWordCounts(wc, 0, foundWords - 1, wcBuffer);
    auto endAlfieQ3 = Clock::now();
    auto durAlfieQ3 = chrono::duration_cast<chrono::milliseconds>(endAlfieQ3 - startAlfieQ3);

    for (int i = 0; i < foundWords; ++i) {
        cout << wc[i].word << ": " << wc[i].count << "\n";
    }

    cout << "Q3 using merge Sort and Binary Search was completed in:" << durAlfieQ3.count() << " ms\n";

}

void Stanlie() {

    loadTransactions("cleaned_transactions.csv");
    loadReviews     ("cleaned_reviews.csv");

    char (*categoryTarget)[FieldLength] = new char[1][FieldLength];
    char (*paymentTarget)[FieldLength] = new char[1][FieldLength];

    strcpy(categoryTarget[0], "Electronics");
    strcpy(paymentTarget[0], "Credit Card");

    int electronicsTotal = 0;
    int creditCardCount = 0;

    auto StanlieStartQ2 = Clock::now();
    for (int i = 0; i < transactionCount; ++i) {
        // Use linearSearch with dynamically allocated arrays
        if (linearSearch(categoryTarget, 1, transactions[i][2]) >= 0) {
            ++electronicsTotal;

            if (linearSearch(paymentTarget, 1, transactions[i][5]) >= 0) {
                ++creditCardCount;
            }
        }
    }
    auto StanlieEndQ2 = Clock::now();

    if (electronicsTotal == 0) {
        cout << "\nNo Electronics transactions found.\n";
    } else {
        double percentage = (double)creditCardCount / electronicsTotal * 100;
        cout << "Linear Search\n";
        cout << "Total Electronics purchases: " << electronicsTotal << "\n";
        cout << "Paid with Credit Card: " << creditCardCount << "\n";
        cout << "Percentage: " << fixed << setprecision(2) << percentage << "%\n";
    }

    cout <<"Linear Search Time: "<<chrono::duration_cast<chrono::nanoseconds>(StanlieEndQ2 - StanlieStartQ2).count()<<"ns\n";
    // Clean up dynamic memory
    delete[] categoryTarget;
    delete[] paymentTarget;

        //Question 3
    char** wordList = nullptr;
    int* wordCounts = nullptr;
    int capacity = 10;
    int uniqueWordCount = 0;

    wordList = new char*[capacity];
    wordCounts = new int[capacity];
    auto StanlieStartQ3 = Clock::now();
    for (int i = 0; i < reviewCount; ++i) {
        if (strcmp(reviews[i][2], "1") == 0) {
            char buffer[FieldLength];
            strcpy(buffer, reviews[i][3]);

            for (int j = 0; buffer[j]; ++j)
                buffer[j] = isalpha((unsigned char)buffer[j]) ? tolower((unsigned char)buffer[j]) : ' ';

            
            for (char* token = strtok(buffer, " "); token; token = strtok(nullptr, " ")) {
                int idx = linearSearch(wordList, uniqueWordCount, token);

                if (idx >= 0) {
                    wordCounts[idx]++;
                } else {
                    if (uniqueWordCount == capacity) {
                        int newCap = capacity * 2;

                        char** newWordList = new char*[newCap];
                        for (int k = 0; k < capacity; ++k) newWordList[k] = wordList[k];
                        delete[] wordList;
                        wordList = newWordList;

                        int* newWordCounts = new int[newCap];
                        for (int k = 0; k < capacity; ++k) newWordCounts[k] = wordCounts[k];
                        delete[] wordCounts;
                        wordCounts = newWordCounts;

                        capacity = newCap;
                    }

                    wordList[uniqueWordCount] = new char[FieldLength];
                    strcpy(wordList[uniqueWordCount], token);
                    wordCounts[uniqueWordCount] = 1;
                    uniqueWordCount++;
                }
            }
        }
    }

    auto StanlieEndQ3 = Clock::now();
    // Create WordCount array dynamically
    WordCount* wordFreq = new WordCount[uniqueWordCount];
    for (int i = 0; i < uniqueWordCount; ++i) {
        strcpy(wordFreq[i].word, wordList[i]);
        wordFreq[i].count = wordCounts[i];
    }

    // Insertion sort (descending)
    insertionSort(wordFreq, uniqueWordCount);

    // Output
    cout << "\nTop Words in 1-Star Reviews:\n";
    for (int i = 0; i < uniqueWordCount; ++i) {
        cout << wordFreq[i].word << ": " << wordFreq[i].count << "\n";
    }

    cout <<"Linear Search Time: "<<chrono::duration_cast<chrono::milliseconds>(StanlieEndQ3 - StanlieStartQ3).count()<<"ms\n";
    auto StanlieEnd1Q3 = Clock::now();
    cout <<"Question 3 timer: "<<chrono::duration_cast<chrono::milliseconds>(StanlieEnd1Q3 - StanlieStartQ3).count()<<"ms\n";
    
    return;
    // Cleanup
    for (int i = 0; i < uniqueWordCount; ++i)
    {
    delete[] wordList[i];
    delete[] wordList;
    delete[] wordCounts;
    delete[] wordFreq;
    }
   
}


void Badr() {
    // Load data into dynamically sized arrays
    loadTransactions("cleaned_transactions.csv");
    loadReviews("cleaned_reviews.csv");

    cout << "Working with " << transactionCount << " transactions and " << reviewCount << " reviews\n";

    // =====================================================================================
    // Question 1: Heap Sort and Counting Transactions by Date
    // =====================================================================================
    auto Q1Start = Clock::now(); // Start timer for Question 1
    heapSort(transactions, transactionCount, 4);  // Sorting transactions by the 'date' column (index 4)
    auto Q1End = Clock::now();  // End timer for Question 1

    // Count transactions per date
    string currentDate = transactions[0][4];
    int count = 1;
    cout << "\n===== Transactions by Date =====\n";
    
    for (int i = 1; i < transactionCount; ++i) {
        if (strcmp(transactions[i][4], currentDate.c_str()) == 0) {
            count++;
        } else {
            cout << currentDate << ":\t" << count << "\n";
            currentDate = transactions[i][4];
            count = 1;
        }
    }
    cout << currentDate << ":\t" << count << "\n";  // Output last date count
    cout << "Total Transactions: " << transactionCount << "\n";

    cout << "Heap Sort and Counting Transactions completed in "
         << chrono::duration_cast<chrono::milliseconds>(Q1End - Q1Start).count() << " ms\n";

    // =====================================================================================
    // Question 2: Hash Search for Electronics purchases using Credit Card
    // =====================================================================================
    auto Q2Start = Clock::now(); // Start timer for Question 2
    hashSearchCategoryPayment(transactions, transactionCount);  // Call Hash Search
    auto Q2End = Clock::now();  // End timer for Question 2

    cout << "Hash Search for Electronics using Credit Card completed in "
         << chrono::duration_cast<chrono::milliseconds>(Q2End - Q2Start).count() << " ms\n";

    // =====================================================================================
    // Question 3: Word Frequency in 1-Star Reviews
    // =====================================================================================
    auto Q3Start = Clock::now(); // Start timer for Question 3
    cout << "\n===== 1-Star Reviews Word Frequency =====\n";

    // Find the 1-star reviews and count word frequencies
    int first = -1, last = -1;
    findOneStarReviews(first, last, reviewCount, reviews);
    
    if (first == -1) {
        cout << "No 1-star reviews found.\n";
        return;
    }

    int total1Stars = last - first + 1;
    cout << "Total 1-Star Reviews: " << total1Stars << "\n";

    WordCount* wc = new WordCount[total1Stars * 50];  // Maximum words
    int unique = 0;
    countWordFrequencies(first, last, reviews, wc, unique);  // Count word frequencies

    heapSort(wc, unique);  // Sort word frequencies using Heap Sort

    displayTopWords(wc, unique, 10);  // Display top 10 words

    auto Q3End = Clock::now();  // End timer for Question 3
    cout << "Word Frequency Analysis for 1-Star Reviews completed in "
         << chrono::duration_cast<chrono::milliseconds>(Q3End - Q3Start).count() << " ms\n";


    // After all the questions have been processed, calculate the time taken for each question
    long long Q1Time = chrono::duration_cast<chrono::milliseconds>(Q1End - Q1Start).count();
    long long Q2Time = chrono::duration_cast<chrono::milliseconds>(Q2End - Q2Start).count();
    long long Q3Time = chrono::duration_cast<chrono::milliseconds>(Q3End - Q3Start).count();

    // Calculate the total time as the sum of individual question times
    long long totalTime = Q1Time + Q2Time + Q3Time;

    // =====================================================================================
    // Time Summary for All Questions
    // =====================================================================================
    cout << "\n===== Time Summary =====\n";
    cout << "Question 1 (Heap Sort + Counting Transactions): "
         << chrono::duration_cast<chrono::milliseconds>(Q1End - Q1Start).count() << " ms\n";
    cout << "Question 2 (Hash Search for Electronics + Credit Card): "
         << chrono::duration_cast<chrono::milliseconds>(Q2End - Q2Start).count() << " ms\n";
    cout << "Question 3 (Word Frequency in 1-Star Reviews): "
         << chrono::duration_cast<chrono::milliseconds>(Q3End - Q3Start).count() << " ms\n";
    cout << "Total Time: " << totalTime << " ms\n";
}


int main(){
    // Clean Data
    cleanTransactions("transactions.csv");
    cleanReviews    ("reviews.csv");

    int choice;

    do {
        cout << "\n===== MENU =====\n";
        cout << "1. Merge Sort + Binary Search (Alfiansyah Clark - TP075566)\n";
        cout << "2. Insertion Sort + Linear Search (Stanlie Lin - TP073945)\n";
        cout << "3. Heap Sort + Hashing Search (Badr Abduldaim - TP074644)\n";
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
                cout << "\nInsertion Sort + Linear Search\n";
                Stanlie();
                break;
            case 3:
                cout << "\nHeap Sort & Hashing Search\n";
                Badr();
                break;
            case 4:
                cout << "\nHadi\n";
                break;
            case 5:
                cout << "\nExiting...\n";
                return 0;
            default:
                cout << "\nInvalid choice. Please try again.\n";
        }

    } while (choice != 0);

}