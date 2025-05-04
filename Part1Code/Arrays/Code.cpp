#include "DataCleaning.h"
#include "LoadData.h"
#include "SortAlgorithms.h" // Ensure this header defines the MergeSort class
#include "SearchAlgorithms.h"

#include <iostream>
#include <iomanip>
#include <cstring>
#include <chrono>  
#include <cctype>

using namespace std;
using Clock = chrono::high_resolution_clock;
// Struct fro timing
struct TimingResults {
    //Q1
    long long Q1_SortTime;
    long long Q1_Full; 
    //Q2
    long long Q2_SortTime; 
    long long Q2_SearchTime; 
    long long Q2_Full;
    //Q3
    long long Q3_SortTime; 
    long long Q3_SearchTime;
    long long Q3_Full;
};

TimingResults alfiResults;
TimingResults hadiResults;
TimingResults stanlieResults;
TimingResults badrResults;
bool alfiRun = false;
bool hadiRun = false;
bool stanlieRun = false;
bool badrRun = false;



TimingResults Alfie(){
    // Load data
    loadTransactions("cleaned_transactions.csv");
    loadReviews     ("cleaned_reviews.csv");
    
    cout << "Transactions:"<< transactionCount<< "\tReviews: "<<reviewCount<<endl;

     // object
     TimingResults results = {0, 0, 0, 0, 0, 0, 0, 0};
     MergeSort sort; // MergeSort class

    // Question 1: Count transactions per day
    // Unique customer ID's
    auto AlfieStartQ1 = Clock::now();
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
    auto AlfieStartMergeQ1 = Clock::now();
    sort.sortTransactions(transactions, 0, transactionCount - 1, 4, transactionCount);
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

    //cout << TransReviewArray[1][0]<<" AND "<<TransReviewArray[1][1];
    cout << "\nTR " << totalReviewsTR <<"  TT "<< totalTransactionsTR;

    // Sort by date
    sort.sortTransRevArray(TransReviewArray, 0, pairCount - 1, 0, transactionCount);

    // for (int i=0; i<pairCount; i++){
    //     cout << TransReviewArray[i][0] << "And" <<TransReviewArray[i][1]<<endl;
    // }

    
    //  2D array for storing dates and counts
    char (*revDateCounts)[2][FieldLength] = new char[uniqueDatesCount][2][FieldLength];

    //Count Trans per day in TransReview
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
    auto AlfieEndQ1 = Clock::now();
    
    auto Q1Sort = chrono::duration_cast<chrono::milliseconds>(AlfieEndMergeSortQ1 - AlfieStartMergeQ1).count();
    auto Q1Duration = chrono::duration_cast<chrono::milliseconds>(AlfieEndQ1 - AlfieStartQ1).count();


    cout << "Merge Sort time: " << Q1Sort << " ms\n";
    cout << "Entire Merge Sort Implementation Q1 Algorithm Time: "<< Q1Duration << " ms\n\n";


    results.Q1_SortTime=Q1Sort;
    results.Q1_Full=Q1Duration;





    // Question 2: Electronics and Credit Card Analysis
    cout<<"Question 2\n";
    auto AlfieStartFullQ2 = Clock::now();
    sort.sortTransactions(transactions, 0, transactionCount - 1, 4, transactionCount);

    //search
    auto AlfieStartBinaryQ2 = Clock::now();
    int idx = binarySearch(transactions, transactionCount, "Electronics", 2);
    if (idx < 0) {
        cout << "\nDidn't find any Electronics transactions.\n";
        return results;
    }
    int firstQ2Alfie = idx, lastQ2Alfie = idx;
    //blocks
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


    auto MergeSortTimeQ2 =chrono::duration_cast<chrono::milliseconds>(AlfieStartBinaryQ2 - AlfieStartFullQ2).count();
    auto BinarySearchQ2 =chrono::duration_cast<chrono::nanoseconds>(AlfieEndBinaryQ2 - AlfieStartBinaryQ2).count();
    auto Q2Duration = chrono::duration_cast<chrono::milliseconds>(AlfieEndfullQ2 - AlfieStartFullQ2).count();
    cout << "Binary Search Process for all 1 star Reviews: " << BinarySearchQ2 << " nano seconds\n";
    cout << "Entire Q2 Implementation BinarySearch: " << Q2Duration << " ms\n\n";
   
    //To time struct
    results.Q2_SortTime=MergeSortTimeQ2;
    results.Q2_SearchTime=BinarySearchQ2;
    results.Q2_Full=Q2Duration;




    

    // Question 3: Word frequency in 1-star reviews
    cout<<"\n\nQuestion 3\n";
    auto startAlfieQ3 = Clock::now();

    //sort rating
    sort.sortReviews(reviews, 0, reviewCount - 1, 2, reviewCount);
    auto endSortQ3 = Clock::now();

    //search
    int found = binarySearchReviews(reviews, reviewCount, "1", 2);

    if (found < 0) {
        cout << "No 1-star\n";
        return results;
    }
    //block
    int firstQ3Alfie = found, lastQ3Alfie = found;
    while (firstQ3Alfie > 0 && strcmp(reviews[firstQ3Alfie - 1][2], "1") == 0) --firstQ3Alfie;
    while (lastQ3Alfie + 1 < reviewCount && strcmp(reviews[lastQ3Alfie + 1][2], "1") == 0) ++lastQ3Alfie;
    auto endSearchQ3 = Clock::now();


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

        //lowercase, punct = space
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

    //WordCount struc array
    WordCount* wc = new WordCount[foundWords];
    for (int i = 0; i < foundWords; ++i) {
        strcpy(wc[i].word, words[i]);
        wc[i].count = counts[i];
    }

    //sort counts
    WordCount* wcBuffer = new WordCount[foundWords];
    sort.sortWordCounts(wc, 0, foundWords - 1, wcBuffer);
    auto endAlfieQ3 = Clock::now();

    for (int i = 0; i < foundWords; ++i) {
        cout << wc[i].word << ": " << wc[i].count << "\n";
    }

    auto SortTimeQ3Duration =chrono::duration_cast<chrono::milliseconds>(endSortQ3 - startAlfieQ3).count();
    auto SearchTimeQ3Duration = chrono::duration_cast<chrono::milliseconds>(endSearchQ3- endSortQ3).count();
    auto durAlfieQ3 = chrono::duration_cast<chrono::milliseconds>(endAlfieQ3 - startAlfieQ3).count();
    cout << "Q3 using merge Sort and Binary Search was completed in:" << durAlfieQ3 << " ms\n";

    results.Q3_Full=durAlfieQ3;
    results.Q3_SortTime=SortTimeQ3Duration;
    results.Q3_SearchTime=SearchTimeQ3Duration;
    
    return results;

}

TimingResults Stanlie() {

    TimingResults StanResults = {0, 0, 0, 0, 0, 0, 0, 0}; //timing struct
    loadTransactions("cleaned_transactions.csv");
    loadReviews     ("cleaned_reviews.csv");

    // Question 1: Sort transactions by date and count
    auto StanlieStartQ1 = Clock::now();  // Start full Q1 timer

// Sort by date (column index 4)
auto StanlieStartQ1_Sort = Clock::now();
cout << "\nQuestion 1 sort transactions by dates";
insertionSort(transactions, transactionCount, 4); 
auto StanlieEndQ1_Sort = Clock::now();

char (*dateTransactions)[2][FieldLength] = new char[transactionCount][2][FieldLength]; // max rows = all transactions
int dateIndex = 0;

char currentDate[FieldLength];
strcpy(currentDate, transactions[0][4]);
int dailyCount = 1;

// Count per date
for (int i = 1; i < transactionCount; ++i) {
    if (strcmp(transactions[i][4], currentDate) == 0) {
        ++dailyCount;
    } else {
        // Save previous date and its count
        strcpy(dateTransactions[dateIndex][0], currentDate);
        sprintf(dateTransactions[dateIndex][1], "%d", dailyCount);
        ++dateIndex;

        // Move to next date
        strcpy(currentDate, transactions[i][4]);
        dailyCount = 1;
    }
}

// Add last date
strcpy(dateTransactions[dateIndex][0], currentDate);
sprintf(dateTransactions[dateIndex][1], "%d", dailyCount);
++dateIndex;

// Output
cout << "\n=== Transactions Per Day ===\n";
for (int i = 0; i < dateIndex; ++i) {
    cout << dateTransactions[i][0] << ":" << dateTransactions[i][1] << " transactions\n";
}
cout << "Total transactions: " << transactionCount << endl;

auto StanlieEndQ1 = Clock::now();  // End full Q1 timer

// Timings
cout << "Insertion Sort Time: " << chrono::duration_cast<chrono::milliseconds>(StanlieEndQ1_Sort - StanlieStartQ1_Sort).count() << " ms\n";

StanResults.Q1_SortTime = chrono::duration_cast<chrono::milliseconds>(StanlieEndQ1_Sort - StanlieStartQ1_Sort).count();
StanResults.Q1_Full     = chrono::duration_cast<chrono::milliseconds>(StanlieEndQ1 - StanlieStartQ1).count();

// Cleanup
delete[] dateTransactions;

    // Question 2: Electronics & Credit Card Analysis
    auto StanlieStartQ2 = Clock::now();
    cout << "\nQuestion 2: Electronics & Credit Card Analysis";
    auto StanlieSortStartQ2 = Clock::now();
    insertionSort(transactions, transactionCount, 5); // Sort by payment method first
    insertionSort(transactions, transactionCount, 2); // Then by category
    auto StanlieSortEndQ2 = Clock::now();
    
    char (*categoryTarget)[FieldLength] = new char[1][FieldLength];
    char (*paymentTarget)[FieldLength] = new char[1][FieldLength];
    strcpy(categoryTarget[0], "Electronics");
    strcpy(paymentTarget[0], "Credit Card");
    
    int electronicsTotal = 0;
    int creditCardCount = 0;
    
    auto StanlieStartSearchQ2 = Clock::now();
    for (int i = 0; i < transactionCount; ++i) {
        if (linearSearch(categoryTarget, 1, transactions[i][2]) >= 0) {
            ++electronicsTotal;
            if (linearSearch(paymentTarget, 1, transactions[i][5]) >= 0) {
                ++creditCardCount;
            }
        }
    }
    auto StanlieEndSearchQ2 = Clock::now();
    
    // Display result
    if (electronicsTotal == 0) {
        cout << "\nNo Electronics transactions found.\n";
    } else {
        double percentage = (double)creditCardCount / electronicsTotal * 100;
        cout << "Total Electronics purchases: " << electronicsTotal << "\n";
        cout << "Paid with Credit Card: " << creditCardCount << "\n";
        cout << "Percentage: " << fixed << setprecision(2) << percentage << "%\n";
    }
    auto StanlieEndQ2 = Clock::now();

    // Show timings
    cout << "Insertion Sort Time: " << chrono::duration_cast<chrono::milliseconds>(StanlieSortEndQ2 - StanlieSortStartQ2).count() << " ms\n";
    cout << "Linear Search Time: " << chrono::duration_cast<chrono::nanoseconds>(StanlieEndSearchQ2 - StanlieStartSearchQ2).count() << " ns\n";
    
    // Store timing results
    StanResults.Q2_SortTime   = chrono::duration_cast<chrono::milliseconds>(StanlieSortEndQ2 - StanlieSortStartQ2).count();
    StanResults.Q2_SearchTime = chrono::duration_cast<chrono::nanoseconds>(StanlieEndSearchQ2 - StanlieStartSearchQ2).count();
    StanResults.Q2_Full       = chrono::duration_cast<chrono::milliseconds>(StanlieEndQ2 - StanlieSortStartQ2).count();
    
    // Clean up
    delete[] categoryTarget;
    delete[] paymentTarget;

    // Question 3: Most frequent words in 1-star reviews
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
    cout << "\nQuestion 3: Words frequently used in product reviews rated 1-star";
    // Create WordCount array dynamically
    WordCount* wordFreq = new WordCount[uniqueWordCount];
    for (int i = 0; i < uniqueWordCount; ++i) {
        strcpy(wordFreq[i].word, wordList[i]);
        wordFreq[i].count = wordCounts[i];
    }

    auto StanlieSortQ3 = Clock::now();
    // Insertion sort (descending)
    insertionSort(wordFreq, uniqueWordCount);
    
    auto StanlieEndSortQ3 = Clock::now();

    // Output
    cout << "\nTop Words in 1-Star Reviews:\n";
    for (int i = 0; i < uniqueWordCount; ++i) {
        cout << wordFreq[i].word << ": " << wordFreq[i].count << "\n";
    }

    cout <<"Linear Search Time: "<<chrono::duration_cast<chrono::nanoseconds>(StanlieEndQ3 - StanlieStartQ3).count()<<"ns\n";
    auto StanlieEnd1Q3 = Clock::now();
    cout << "Sort Time: " << chrono::duration_cast<chrono::nanoseconds>(StanlieEndSortQ3 - StanlieSortQ3).count() << " ns\n";

    // Store Q3 timing
    StanResults.Q3_SortTime = chrono::duration_cast<chrono::nanoseconds>(StanlieEndSortQ3 - StanlieSortQ3).count();
    StanResults.Q3_SearchTime = chrono::duration_cast<chrono::nanoseconds>(StanlieEndQ3 - StanlieStartQ3).count();
    StanResults.Q3_Full = chrono::duration_cast<chrono::milliseconds>(StanlieEnd1Q3 - StanlieStartQ3).count();

// ────────────────────────────────────────────────
// Final Summary for Stanlie's Results
// ────────────────────────────────────────────────
cout << "\nSummary of all times:\n";
cout << "Q1 : " << StanResults.Q1_Full << " ms\n";
cout << "Q2 : "  << StanResults.Q2_Full << " ms\n";
cout << "Q3 : " << StanResults.Q3_Full << " ms\n";
    return StanResults;

    // Cleanup
    for (int i = 0; i < uniqueWordCount; ++i)
    {
    delete[] wordList[i];
    delete[] wordList;
    delete[] wordCounts;
    delete[] wordFreq;
    }
   
}


TimingResults Badr() {
    TimingResults BadrResults = {0, 0, 0, 0, 0, 0, 0, 0};
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
    auto Q1FullEnd = Clock::now();

    cout << "Heap Sort and Counting Transactions completed in "
         << chrono::duration_cast<chrono::milliseconds>(Q1End - Q1Start).count() << " ms\n";


    // Store Q1 timing
    BadrResults.Q1_SortTime = chrono::duration_cast<chrono::milliseconds>(Q1End - Q1Start).count();
    BadrResults.Q1_Full = chrono::duration_cast<chrono::milliseconds>(Q1FullEnd - Q1Start).count();
    
    // =====================================================================================
    // Question 2: Hash Search for Electronics purchases using Credit Card
    // =====================================================================================
    auto Q2Start = Clock::now(); // Start timer for Question 2
    hashSearchCategoryPayment(transactions, transactionCount);  // Call Hash Search
    auto Q2End = Clock::now();  // End timer for Question 2

    cout << "Hash Search for Electronics using Credit Card completed in "
         << chrono::duration_cast<chrono::milliseconds>(Q2End - Q2Start).count() << " ms\n";
    auto Q2FullEnd = Clock::now();

    // Store Q2 timing
    BadrResults.Q2_SortTime = 0; // No additional sorting in Q2
    BadrResults.Q2_SearchTime = chrono::duration_cast<chrono::nanoseconds>(Q2End - Q2Start).count();
    BadrResults.Q2_Full = chrono::duration_cast<chrono::milliseconds>(Q2FullEnd - Q2Start).count();

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
        return BadrResults;
    }

    int total1Stars = last - first + 1;
    cout << "Total 1-Star Reviews: " << total1Stars << "\n";

    WordCount* wc = new WordCount[total1Stars * 50];  // Maximum words
    int unique = 0;
    countWordFrequencies(first, last, reviews, wc, unique);  // Count word frequencies

    auto Q3SortStart = Clock::now();
    heapSort(wc, unique);  // Sort word frequencies using Heap Sort
    auto Q3SortEnd = Clock::now();

    displayTopWords(wc, unique, 10);  // Display top 10 words

    auto Q3End = Clock::now();  // End timer for Question 3
    cout << "Word Frequency Analysis for 1-Star Reviews completed in "
         << chrono::duration_cast<chrono::milliseconds>(Q3End - Q3Start).count() << " ms\n";


     // Store Q3 timing
     BadrResults.Q3_SortTime = chrono::duration_cast<chrono::nanoseconds>(Q3SortEnd - Q3SortStart).count();
     BadrResults.Q3_SearchTime = 0; 
     BadrResults.Q3_Full = chrono::duration_cast<chrono::milliseconds>(Q3End - Q3Start).count();

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

TimingResults Hadi() {
    TimingResults HadiResults = {0, 0, 0, 0, 0, 0, 0, 0};
    //Question 1: Sorting transactions by date and counting them

    // Load data into dynamically sized arrays
    loadTransactions("cleaned_transactions.csv");
    loadReviews("cleaned_reviews.csv");

    cout << "Working with " << transactionCount << " transactions and " << reviewCount << " reviews\n";

    auto bubblSortQ1Start = Clock::now(); // Start timer for Question 1
    bubbleSort(transactions, transactionCount, 4);  // Sorting transactions by the date
    auto bubbleSortQ1End = Clock::now();  // End timer for Question 1

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
    auto FullQ1End = Clock::now();
    cout << "Bubble Sort completed in: "
         << chrono::duration_cast<chrono::milliseconds>(bubbleSortQ1End - bubblSortQ1Start).count() << " ms\n";
    

    // Store Q1 timing
    HadiResults.Q1_SortTime = chrono::duration_cast<chrono::milliseconds>(bubbleSortQ1End - bubblSortQ1Start).count();
    HadiResults.Q1_Full = chrono::duration_cast<chrono::milliseconds>(FullQ1End - bubblSortQ1Start).count();


    // Question 2: Jump Search for Electronics purchases using Credit Card

    auto bubblSortQ2Start = Clock::now();
    bubbleSort(transactions, transactionCount, 2);  // Re-sort transactions by category
    auto bubbleSortQ2End = Clock::now();
    findElectronicsAndCreditCardPurchases(transactions, transactionCount);
    auto FullQ2End = Clock::now();

    cout << "Jump Search for Electronics using Credit Card completed in "
         << chrono::duration_cast<chrono::milliseconds>(FullQ2End - bubblSortQ2Start).count() << " ms\n";


    // Store Q2 timing
    HadiResults.Q2_SortTime = chrono::duration_cast<chrono::milliseconds>(bubbleSortQ2End - bubblSortQ2Start).count();
    HadiResults.Q2_SearchTime = chrono::duration_cast<chrono::milliseconds>(FullQ2End - bubbleSortQ2End).count(); 
    HadiResults.Q2_Full = chrono::duration_cast<chrono::milliseconds>(FullQ2End - bubblSortQ2Start).count();

    // Question 3: Word Frequency in 1-Star Reviews

    auto bubbleSortQ3Start = Clock::now(); // Start timer for Question 3
    reviewsBubbleSort(reviews, reviewCount, 2); // Sort reviews by rating
    auto bubbleSortQ3End = Clock::now();
    analyzeOneStarReviews(reviews, reviewCount);
    auto FullQ3End = Clock::now();  // End timer for Question 3

    cout << "Word Frequency Analysis for 1-Star Reviews using bubble sort and jump search completed in "
         << chrono::duration_cast<chrono::milliseconds>(bubbleSortQ3End - bubbleSortQ3Start).count() << " ms\n";


    //Final Summary

    cout << "Summary of all times:\n";
    cout << "Sorting the array: "
         << chrono::duration_cast<chrono::milliseconds>(bubbleSortQ1End - bubblSortQ1Start).count() << " ms\n";
    cout << "Searching for all electronics purchased using a credit card: "
         << chrono::duration_cast<chrono::milliseconds>(bubbleSortQ2End - bubblSortQ2Start).count() << " ms\n";
    cout << "Finding all 1-star reviews and extracting the individual words: "
         << chrono::duration_cast<chrono::milliseconds>(bubbleSortQ3End - bubbleSortQ3Start).count() << " ms\n";


    // Store Q3 timing
    HadiResults.Q3_SortTime = chrono::duration_cast<chrono::milliseconds>(bubbleSortQ3End - bubbleSortQ3Start).count();
    HadiResults.Q3_SearchTime = chrono::duration_cast<chrono::milliseconds>(FullQ3End - bubbleSortQ3End).count();
    HadiResults.Q3_Full = chrono::duration_cast<chrono::milliseconds>(bubbleSortQ3End - bubbleSortQ3Start).count();

    return HadiResults;
}


void PrintResults() {
    if (!alfiRun && !hadiRun && !stanlieRun && !badrRun) {
        cout << "\nNo timing data available.\n";
        return;
    }
    
    cout << "\n====  TIMING  ====\n";
    
    if (alfiRun) {
        cout << "\nALFIE (Merge Sort + Binary Search):\n";
        cout << "Q1 Total: " << alfiResults.Q1_Full << " ms\n";
        cout << "Q1 Merge Sort: " << alfiResults.Q1_SortTime << " ms\n";
        cout << "--------------------------\n";
        cout << "Q2 Total: " << alfiResults.Q2_Full << " ms\n";
        cout << "Q2 Merge Sort: " << alfiResults.Q2_SortTime << " ms\n";
        cout << "Q2 Binary Search: " << alfiResults.Q2_SearchTime << " ns\n";
        cout << "--------------------------\n";
        cout << "Q3 Total: " << alfiResults.Q3_Full << " ms\n";
        cout << "Q3 Merge Sort: " << alfiResults.Q3_SortTime << " ms\n";
        cout << "Q3 Binary Search: " << alfiResults.Q3_SearchTime << " ns\n";
        cout << "--------------------------\n";
        cout << "TOTAL: " << alfiResults.Q1_Full + alfiResults.Q2_Full + alfiResults.Q3_Full << " ms\n";
        cout << "===========================\n";
    }
    
    if (hadiRun) {
        cout << "\nHADI (Bubble Sort + Jump Search):\n";
        cout << "Q1 Total: " << hadiResults.Q1_Full << " ms\n";
        cout << "Q1 Bubble Sort: " << hadiResults.Q1_SortTime << " ms\n";
        cout << "--------------------------\n";
        cout << "Q2 Total: " << hadiResults.Q2_Full << " ms\n";
        cout << "Q2 Bubble Sort: " << hadiResults.Q2_SortTime << " ms\n";
        cout << "Q2 Jump Search: "<<hadiResults.Q2_SortTime<<"ms\n";
        cout << "--------------------------\n";
        cout << "Q3 Total: " << hadiResults.Q3_Full << " ms\n";
        cout << "Q3 Bubble Sort: " << hadiResults.Q3_SortTime << "ms\n";
        cout << "Q3 Jump Search: " << hadiResults.Q3_SearchTime << " ms\n";
        cout << "--------------------------\n";
        cout << "TOTAL: " << hadiResults.Q1_Full + hadiResults.Q2_Full + hadiResults.Q3_Full << " ms\n";
        cout << "===========================\n";
    }
    if (stanlieRun) {
        cout << "\nStanlie:\n";
        cout << "Q1 Total: " << stanlieResults.Q1_Full << " ms\n";
        cout << "Q1 Insertion Sort: "<< stanlieResults.Q1_SortTime<<"ms\n";
        cout << "--------------------------\n";
        cout << "Q2 Total: " << stanlieResults.Q2_Full << " ms\n";
        cout << "Q2 Insertion Sort: " << stanlieResults.Q2_SortTime << " ms\n";
        cout << "Q2 Linear Search: "<< stanlieResults.Q2_SearchTime<<"ns\n";
        cout << "--------------------------\n";
        cout << "Q3 Total: " << stanlieResults.Q3_Full << " ms\n";
        cout << "Q3 Insertion Sort: "<< stanlieResults.Q3_SortTime<<"ns\n";
        cout << "Q3 Linear Search: "<< stanlieResults.Q3_SearchTime<<"ns\n";
        cout << "--------------------------\n";
        cout << "TOTAL: " << stanlieResults.Q1_Full + stanlieResults.Q2_Full + stanlieResults.Q3_Full << " ms\n";
        cout << "===========================\n";
    }
        
    if (badrRun) {
        cout << "\nBadr:\n";
        cout << "Q1 Total: " << badrResults.Q1_Full << " ms\n";
        cout << "Q1 Heap Sort: " << badrResults.Q1_SortTime << " ms\n";
        cout << "--------------------------\n";
        cout << "Q2 Total: " << badrResults.Q2_Full << " ms\n";
        cout << "Q2 No Sort Used.\n";
        cout << "Q2 Hashing Search: " << badrResults.Q2_SearchTime<< "ns\n";
        cout << "--------------------------\n";
        cout << "Q3 Total: " << badrResults.Q3_Full << " ms\n";
        cout << "Q3 No Sort Required\n";
        cout << "Q3 Hashing Search: " << badrResults.Q3_SearchTime << " ns\n";
        cout << "--------------------------\n";
        cout << "TOTAL: " << badrResults.Q1_Full + badrResults.Q2_Full + badrResults.Q3_Full << " ms\n";
        cout << "===========================\n";
    }
    
    
    cout << "=========================\n";
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
        cout << "4. Bubble Sort + Jump Search (Abdulhadi Muhammad TP077939)\n";
        cout << "5. Print Results\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "\nMerge Sort & Binary Search\n";
                alfiResults = Alfie();
                alfiRun = true;
                break;
            case 2:
                cout << "\nInsertion Sort + Linear Search\n";
                stanlieResults = Stanlie();
                stanlieRun = true;
                break;
            case 3:
                cout << "\nHeap Sort & Hashing Search\n";
                badrResults = Badr();
                badrRun = true;
                break;
            case 4:
                cout << "\nBubble Sort + Jump Search\n";
                hadiResults = Hadi();
                hadiRun = true;
                break;
            case 5:
                PrintResults();
                break;
            case 6:
                cout << "\nExiting...\n";
                return 0;
            default:
                cout << "\nInvalid choice. Please try again.\n";
        }

    } while (choice != 0);
}
