#include "LoadData.h"
#include "SortAlgorithmsLL.h"
#include "SearchAlgorithmsLL.h"

#include<iostream>
#include<iomanip>
#include<cstring>
#include<string>
#include<chrono>  
#include<cctype>
#include<sstream>

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
bool baderRun = false;


TimingResults Alfie(){
    cout << "\nMerge Sort & Binary Search\n";
    Loader loader("cleaned_transactions.csv", "cleaned_reviews.csv");
    TimingResults results; //timing struct

    if (!loader.loadTrans()||!loader.loadRev()) {
        cerr << "Failed to load files";
        return TimingResults{0, 0, 0, 0, 0, 0, 0, 0};;
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
    
    TransactionNode* transHead = loader.getTransHead();
    ReviewNode* reviewHead = loader.getRevHead();
    cout << "Transactions: " << loader.getTransCount() << "\tReviews: " << loader.getRevCount() << endl;
  
    //MergeSort object
    MergeSortLL sort;
    
    // Question 1: Count transactions per day
    TransactionNode* current = transHead;
      
    // Sort transactions by date
    cout << "Sorting transactions by date...\n";
    auto sortStartTime = Clock::now();
    transHead = sort.sortTransactions(transHead, 4);
    auto sortEndTime = Clock::now();
    cout << "\nTransactions by Date:\n";
      
    // DateCount linked list
    DateCount* dateCountHead = nullptr;
    int uniqueDatesCount = 0;
      
    if (transHead != nullptr) {
        current = transHead;
        string currentDate = current->data.date;
        int dailyCount = 1;
        current = current->next;
          
        while (current != nullptr) {
            if (current->data.date == currentDate) {
                dailyCount++;
            } else {
                // Add to Date
                DateCount* newDateNode = new DateCount(currentDate.c_str(), dailyCount);
                newDateNode->next = dateCountHead;
                dateCountHead = newDateNode;
                uniqueDatesCount++;
                  
                cout << currentDate << ":\t" << dailyCount << "\n";
            
                // Reset
                currentDate = current->data.date;
                dailyCount = 1;
            }
            current = current->next;
          }
          
        // Add the last date
        DateCount* newDateNode = new DateCount(currentDate.c_str(), dailyCount);
        newDateNode->next = dateCountHead;
        dateCountHead = newDateNode;
        uniqueDatesCount++;
          
        // last date and count
        cout << currentDate << ":\t" << dailyCount << "\n";
    }
      
    //cout << "Total unique dates: " << uniqueDatesCount << "\n";
    cout << "Total transactions: " << loader.getTransCount() << "\n";
    auto sortDuration = chrono::duration_cast<chrono::nanoseconds>(sortEndTime - sortStartTime).count();
    cout << "Merge sort time: " << sortDuration << " ns\n";
      
    // Create Transaction + Review pairs
    //cout << "\nCreating transaction-review pairs...\n";

    // Create Transaction + Review pairs
    cout << "\nCreating transaction-review pairs...";

    // TransReviewPair list
    TransReviewPair* trHead = nullptr;
    int pairCount = 0;

    // Copytransactions to TransReviewPair
    TransactionNode* transCurrent = transHead;
    while (transCurrent != nullptr) {
        TransReviewPair* newPair = new TransReviewPair(
            transCurrent->data.date.c_str(),
            transCurrent->data.customerID.c_str()
        );
        newPair->next = trHead;
        trHead = newPair;
        pairCount++;
        transCurrent = transCurrent->next;
    }

    //TrackReviews added
    struct ReviewInfo {
        char customerID[255];
        ReviewInfo* next;
        
        ReviewInfo(const char* cid) {
            strcpy(customerID, cid);
            next = nullptr;
        }
    };

    // list of reviews
    ReviewInfo* reviewInfoHead = nullptr;
    int totalReviews = 0;

    ReviewNode* revCurrent = reviewHead;
    while (revCurrent != nullptr) {
        ReviewInfo* newReviewInfo = new ReviewInfo(revCurrent->data.customerID.c_str());
        newReviewInfo->next = reviewInfoHead;
        reviewInfoHead = newReviewInfo;
        totalReviews++;
        revCurrent = revCurrent->next;
    }

    //Which Reviews Correspond
    int matchedReviews = 0;
    int unmatchedReviews = 0;
    //check if matching customer in the transaction
    ReviewInfo* currReview = reviewInfoHead;
    while (currReview != nullptr) {
        bool foundCustomer = false;
        
        // Check customer transactions
        transCurrent = transHead;
        while (transCurrent != nullptr) {
            if (strcmp(transCurrent->data.customerID.c_str(), currReview->customerID) == 0) {
                foundCustomer = true;
                break;
            }
            transCurrent = transCurrent->next;
        }
        if (foundCustomer) {
            matchedReviews++;
        } else {
            unmatchedReviews++;
        }
        currReview = currReview->next;
    }

    // Sort TransReviewPair list by date
    //cout << "Sorting transaction-review pairs by date...\n";
    trHead = sort.sortTransReviewPairs(trHead, 0);

    //transactions per date in the TransReviewPair
    DateCount* revDateCountHead = nullptr;

    if (trHead != nullptr) {
        TransReviewPair* currTR = trHead;
        char currentRevDate[255];
        strcpy(currentRevDate, currTR->date);
        int dailyRevCount = 1;
        currTR = currTR->next;
        
        while (currTR != nullptr) {
            if (strcmp(currTR->date, currentRevDate) == 0) {
                dailyRevCount++;
            } else {
                // Add to DateCount
                DateCount* newNode = new DateCount(currentRevDate, dailyRevCount);
                newNode->next = revDateCountHead;
                revDateCountHead = newNode;
                
                //date and reset
                strcpy(currentRevDate, currTR->date);
                dailyRevCount = 1;
            }
            currTR = currTR->next;
        }
        
        // Add last
        DateCount* newNode = new DateCount(currentRevDate, dailyRevCount);
        newNode->next = revDateCountHead;
        revDateCountHead = newNode;
    }

    // Comp date counts
    cout << "\nComparing Trans+Review Transactions to Transactions:\n";
    int matchingDates = 0;
    int matchingCounts = 0;
    int notMatchingDates = 0;
    int notMatchingCounts = 0;
    bool fullMatch = true;

    DateCount* DateTrans = dateCountHead;     // Og transaction counts
    DateCount* DateTranRev = revDateCountHead;  // TransReviewPair  counts

    // debug heade
    // cout << "Date\t\tTransactions\tTransReviewPairs\tMatch?\n";
    // cout << "---------------------------------------------------------------\n";

    // Check dates in trans list not in transrevlist
    while (DateTrans != nullptr) {
        bool foundMatch = false;
        DateCount* DateTranRevCurrent = revDateCountHead;
        
        while (DateTranRevCurrent != nullptr) {
            if (strcmp(DateTrans->date, DateTranRevCurrent->date) == 0) {
                // Found matching date
                foundMatch = true;
                matchingDates++;
                
                bool countsMatch = (strcmp(DateTrans->count, DateTranRevCurrent->count) == 0);
                if (countsMatch) {
                    matchingCounts++;
                } else {
                    notMatchingCounts++;
                    fullMatch = false;
                }
                //cout << DateTrans->date << "\t" << DateTrans->count << "\t\t" << dc2Iter->count << "\t\t" << (countsMatch ? "MATCH" : "MISMATCH") << "\n";
                break;
            }
            DateTranRevCurrent = DateTranRevCurrent->next;
        }
        if (!foundMatch) {
            notMatchingDates++;
            fullMatch = false;
            //cout << DateTrans->date << "\t" << DateTrans->count << "\t\tNOT FOUND\t\tMISMATCH\n";
        }
        
        DateTrans = DateTrans->next;
    }

    // Check dates in transrevlist list not in tans list
    DateTranRev = revDateCountHead;
    while (DateTranRev != nullptr) {
        bool foundMatch = false;
        DateCount* DateTransCurrent = dateCountHead;
        
        while (DateTransCurrent != nullptr) {
            if (strcmp(DateTranRev->date, DateTransCurrent->date) == 0) {
                foundMatch = true;
                break;
            }
            DateTransCurrent = DateTransCurrent->next;
        }
        if (!foundMatch) {
            notMatchingDates++;
            fullMatch = false;
            //cout << dc2->date << "\tNOT FOUND\t" << dc2->count << "\t\tMISMATCH\n";
        }
        
        DateTranRev = DateTranRev->next;
    }
    auto Q1EndTime = Clock::now();
    auto Q1Duration = chrono::duration_cast<chrono::milliseconds>(Q1EndTime - sortStartTime).count();
    
    cout << "Total transactions: " << pairCount << "\n";
    cout << "Total reviews: " << totalReviews << "\n";
    cout << "Reviews matched to customers with transactions: " << matchedReviews << "\n";
    cout << "Reviews from customers with no associated transactions: " << unmatchedReviews << "\n";
    results.Q1_SortTime=sortDuration;
    results.Q1_Full=Q1Duration;


    if (fullMatch) {
        cout << "\nFull Match! Total Dates: " << matchingDates << "\tMatching Dates: " << matchingDates << "\tMatching Counts: " << matchingCounts << "\n";
    } else {
        cout << "\nNot Full match\tNon-Matching Dates: " << notMatchingDates << "\tNon-Matching Counts: " << notMatchingCounts << "\n";
    }





    // Question 2: Electronics and Credit Card Analysis
    cout << "\n\nQuestion 2: Electronics and Credit Card Analysis\n";

    // merge sort cat
    //cout << "Sorting transactions by category...\n";
    auto startTimeQ2 = Clock::now();
    TransactionNode* sortedTransHead = sort.sortTransactions(transHead, 2); // Sort by category (column 2)
    auto MergeTimeQ2 = Clock::now();

    // binary search
    //cout << "Searching for Electronics transactions...\n";
    auto searchStartTimeQ2 = Clock::now();
    TransactionNode* foundNode = binarySearchTransactions(sortedTransHead, "Electronics", 2);

    if (foundNode == nullptr) {
        cout << "\nDidn't find any Electronics transactions.\n";
        return results;
    }

    //all Electronics leftright of node
    // backwards
    TransactionNode* firstElectNode = foundNode;
    TransactionNode* prevNode = nullptr;
    TransactionNode* tempTrans = sortedTransHead;

    //Predecessor to find node before
    while (tempTrans != nullptr) {
        if (tempTrans->next == firstElectNode) {
            prevNode = tempTrans;
            break;
        }
        tempTrans = tempTrans->next;
    }
    //back until first node
    while (prevNode != nullptr && prevNode->data.category == "Electronics") {
        firstElectNode = prevNode;
    
        //node before elect
        tempTrans = sortedTransHead;
        prevNode = nullptr;
        while (tempTrans != firstElectNode) {
            if (tempTrans->next == firstElectNode) {
                prevNode = tempTrans;
                break;
            }
            tempTrans = tempTrans->next;
        }
    }

    //Forwards
    TransactionNode* lastElectronics = foundNode;
    while (lastElectronics->next != nullptr && lastElectronics->next->data.category == "Electronics") {
        lastElectronics = lastElectronics->next;
    }

    auto searchEndTimeQ2 = Clock::now();
    //Count Electronics transactions and those using Credit Card
    //cout << "Counting Credit Card transactions...\n";
    int ElectronicsCount = 0;
    int CCElectronicsCount = 0;

    //Go through each
    TransactionNode* Q2 = firstElectNode;
    while (true) {
        ElectronicsCount++;
        if (Q2->data.paymentMethod == "Credit Card") {
            CCElectronicsCount++;
        }
        if (Q2 == lastElectronics) {
            break;
        }
        
        Q2 = Q2->next;
    }

    double percentage = 0.0;
    if (ElectronicsCount > 0) {
        percentage = (double)CCElectronicsCount / ElectronicsCount * 100.0;
    }

    auto endTimeQ2 = Clock::now();

    cout << "\nElectronics purchases analysis:\n";
    cout << "Total Electronics transactions: " << ElectronicsCount << "\n";
    cout << "Electronics transactions using Credit Card: " << CCElectronicsCount << "\n";
    cout << "Percentage: " << fixed << setprecision(2) << percentage << "%\n";

    // Performance statistics
    auto SortTimeQ2Duration = chrono::duration_cast<chrono::nanoseconds>(MergeTimeQ2 - startTimeQ2).count();
    auto SearchTimeQ2Duration = chrono::duration_cast<chrono::nanoseconds>(searchEndTimeQ2 - searchStartTimeQ2).count();
    auto Q2Duration = chrono::duration_cast<chrono::milliseconds>(endTimeQ2 - startTimeQ2).count();
    cout << "\nPerformance metrics:\n";
    cout << "Merge sort time: " << SortTimeQ2Duration << " ns\n";
    cout << "Search and boundary finding time: "<< SearchTimeQ2Duration<< " ns\n";
    cout << "Total analysis time: " <<Q2Duration << " ms\n";
    
    //To time struct
    results.Q2_SortTime=SortTimeQ2Duration;
    results.Q2_SearchTime=SearchTimeQ2Duration;
    results.Q2_Full=Q2Duration;



    //Question 3: Word Counts in 1 star Reviews
    cout << "\n\nQuestion 3: Frequent Words in 1 star Reviews";
    auto startTimeQ3 = Clock::now();
    ReviewNode* sortedRevHead = sort.sortReviews(reviewHead, 2); // Sort by rating 
    
    auto MergeTimeQ3 = Clock::now();
    auto searchStartTimeQ3 = Clock::now();
    ReviewNode* foundRevNode = binarySearchReviews(sortedRevHead, "1", 2);

    if (foundRevNode == nullptr) {
        cout << "\nNo 1 Star Reviews\n";
        return results;
    }

    // Find all 1-star reviews
    ReviewNode* firstOneStarReview = foundRevNode;
    ReviewNode* prevReview = nullptr;
    ReviewNode* tempReview = sortedRevHead;

    //Same q2
    while (tempReview != nullptr) {
        if (tempReview->next == firstOneStarReview) {
            prevReview = tempReview;
            break;
        }
        tempReview = tempReview->next;
    }

    // backward until 1
    while (prevReview != nullptr && prevReview->data.rating == 1) {
        firstOneStarReview = prevReview;
        
        //pred
        tempReview = sortedRevHead;
        prevReview = nullptr;
        while (tempReview != firstOneStarReview) {
            if (tempReview->next == firstOneStarReview) {
                prevReview = tempReview;
                break;
            }
            tempReview = tempReview->next;
        }
    }

    //last 1-star  forward
    ReviewNode* lastOneStarReview = foundRevNode;
    while (lastOneStarReview->next != nullptr && lastOneStarReview->next->data.rating == 1) {
        lastOneStarReview = lastOneStarReview->next;
    }

    auto searchEndTimeQ3 = Clock::now();

    // Count
    int totalOneStarReviews = 0;
    ReviewNode* currentReview = firstOneStarReview;
    while (true) {
        totalOneStarReviews++;
        if (currentReview == lastOneStarReview) {
            break;
        }
        currentReview = currentReview->next;
    }

    cout << "Found " << totalOneStarReviews << " 1-star reviews";

    // linked list for word counts
    WordCountNode* wordCountHead = nullptr;
    int totalWords = 0;

    //count word freq
    currentReview = firstOneStarReview;
    while (true) {
        // text to a C-string for tokenization
        char reviewText[1024];
        strncpy(reviewText, currentReview->data.reviewText.c_str(), sizeof(reviewText) - 1);
        reviewText[sizeof(reviewText) - 1] = '\0';
        
        // lowercase + replace punc with space
        for (int i = 0; reviewText[i]; ++i) {
            if (isalpha((unsigned char)reviewText[i])) {
                reviewText[i] = tolower((unsigned char)reviewText[i]);
            } else {
                reviewText[i] = ' ';
            }
        }
        
        //count words
        char* token = strtok(reviewText, " ");
        while (token) {
            // Skip empty
            if (strlen(token) == 0) {
                token = strtok(nullptr, " ");
                continue;
            }
            
            totalWords++;
            
            // Check if alrdy read
            bool wordFound = false;
            WordCountNode* wcNode = wordCountHead;
            
            while (wcNode) {
                if (strcmp(wcNode->data.word, token) == 0) {
                    wcNode->data.count++;
                    wordFound = true;
                    break;
                }
                wcNode = wcNode->next;
            }
            
            // add new
            if (!wordFound) {
                WordCount newWC;
                strcpy(newWC.word, token);
                newWC.count = 1;
                
                WordCountNode* newNode = new WordCountNode(newWC);
                newNode->next = wordCountHead;
                wordCountHead = newNode;
            }
            
            token = strtok(nullptr, " ");
        }
        
        // Move next
        if (currentReview == lastOneStarReview) {
            break;
        }
        currentReview = currentReview->next;
    }

    // Sort
    wordCountHead = sort.sortWordCounts(wordCountHead);
    cout << "\nWords found in 1-star reviews:\n";
    WordCountNode* wcDisplay = wordCountHead;
    int uniqueWordCount = 0;

    while (wcDisplay) {
        cout << wcDisplay->data.word;
        
        // Format output with proper spacing
        int tabCount = 1;
        if (strlen(wcDisplay->data.word) < 8) tabCount = 2;
        for (int t = 0; t < tabCount; t++) cout << "\t";
        
        cout << wcDisplay->data.count << "\n";
        
        wcDisplay = wcDisplay->next;
        uniqueWordCount++;
    }

    cout << "\nTotal unique words found: " << uniqueWordCount << "\n";
    cout << "Total word occurrences: " << totalWords << "\n";
    auto endTimeQ3 = Clock::now();

    auto SortTimeQ3Duration = chrono::duration_cast<chrono::nanoseconds>(MergeTimeQ3 - startTimeQ3).count();
    auto SearchTimeQ3Duration = chrono::duration_cast<chrono::nanoseconds>(searchEndTimeQ3 - searchStartTimeQ3).count();
    auto Q3Duration = chrono::duration_cast<chrono::milliseconds>(endTimeQ3 - startTimeQ3).count();

    cout << "\nPerformance metrics:\n";
    cout << "Merge sort time: " << SortTimeQ3Duration << " ns\n";
    cout << "Search and boundary finding time: " << SearchTimeQ3Duration << " ns\n";
    cout << "Total word counting and analysis time: " << Q3Duration<< " ms\n";

    results.Q3_SortTime=SortTimeQ3Duration;
    results.Q3_SearchTime=SearchTimeQ3Duration;
    results.Q3_Full=Q3Duration;

    return results;

}

TimingResults Hadi() {
    TimingResults hadiResults = {0, 0, 0, 0, 0, 0, 0, 0};
    //Question 1: Sorting transactions by date and counting them

    Loader loader("cleaned_transactions.csv", "cleaned_reviews.csv"); // Load data into linked lists
    loader.loadAll();
    TransactionNode* transHead = loader.getTransHead();
    ReviewNode* reviewHead = loader.getRevHead();

    auto bubblSortLLQ1Start = Clock::now(); // Start timer for Question 1
    bubbleSortTransactionsByDate(transHead); //Sorts linked list by date
    countTransactionsByDate(transHead, loader.getTransCount()); //Prints transaction counts by date
    auto bubbleSortLLQ1End = Clock::now();  // End timer for Question 1

    hadiResults.Q1_SortTime = chrono::duration_cast<chrono::milliseconds>(bubbleSortLLQ1End - bubblSortLLQ1Start).count();
    hadiResults.Q1_Full = chrono::duration_cast<chrono::milliseconds>(bubbleSortLLQ1End - bubblSortLLQ1Start).count();

    cout << "Bubble Sort completed in: "
         << chrono::duration_cast<chrono::milliseconds>(bubbleSortLLQ1End - bubblSortLLQ1Start).count() << " ms\n";

    // Question 2: Jump Search for Electronics purchases using Credit Card

    auto bubblSortLLQ2Start = Clock::now();
    bubbleSortTransactionsByCategory(transHead); //Sorts linked list by category
    countCreditCardPurchases(transHead);
    auto bubbleSortLLQ2End = Clock::now(); // End timer for Question 2
    cout << "Jump Search completed in: "
         << chrono::duration_cast<chrono::milliseconds>(bubbleSortLLQ2End - bubblSortLLQ2Start).count() << " ms\n";

    hadiResults.Q2_SortTime = chrono::duration_cast<chrono::milliseconds>(bubbleSortLLQ2End - bubblSortLLQ2Start).count();
    hadiResults.Q2_SearchTime = 0; //No search needed
    hadiResults.Q2_Full = chrono::duration_cast<chrono::milliseconds>(bubbleSortLLQ2End - bubblSortLLQ2Start).count();

    // Question 3: Counting words in 1-star reviews

    auto wordCountStart = Clock::now(); // Start timer for Question 3
    ReviewNode* oneStarReview = jumpSearch(reviewHead, 1);  // 1-star rating
    auto searchQ3End = Clock::now(); //End
    
    if (oneStarReview != NULL) {
        cout << "1-Star Review Found: " << oneStarReview->data.reviewText << endl;
    } else {
        cout << "No 1-Star Review Found!" << endl;
    }


    tallyWordsInReview(reviewHead);
    auto wordCountEnd = Clock::now(); // End timer for Question 3
    cout << "Word Count completed in: "
         << chrono::duration_cast<chrono::milliseconds>(wordCountEnd - wordCountStart).count() << " ms\n";

    cout << "Summary of all times:\n";
    cout << "Sorting the array: "
         << chrono::duration_cast<chrono::milliseconds>(bubbleSortLLQ1End - bubblSortLLQ1Start).count() << " ms\n";
    cout << "Searching for all electronics purchased using a credit card: "
         << chrono::duration_cast<chrono::milliseconds>(bubbleSortLLQ2End - bubblSortLLQ2Start).count() << " ms\n";
    cout << "Finding all 1-star reviews and extracting the individual words: "
         << chrono::duration_cast<chrono::milliseconds>(wordCountEnd - wordCountStart).count() << " ms\n";


    hadiResults.Q3_SortTime = 0; // No sort
    hadiResults.Q3_SearchTime = chrono::duration_cast<chrono:: nanoseconds>(searchQ3End - wordCountStart).count();
    hadiResults.Q3_Full = chrono::duration_cast<chrono::milliseconds>(wordCountEnd - wordCountStart).count();
    return hadiResults;
}







void PrintResults() {
    if (!alfiRun && !hadiRun) {
        cout << "\nNo timing data available.\n";
        return;
    }
    
    cout << "\n======  TIMINGS  ======\n";
    
    if (alfiRun) {
        cout << "\nALFIE (Merge Sort + Binary Search):\n";
        cout << "Q1 Total: " << alfiResults.Q1_Full << " ms\n";
        cout << "Q1 Merge Sort:"<< alfiResults.Q1_SortTime<<"ns\n";
        cout << "--------------------------\n";
        cout << "Q2 Total: " << alfiResults.Q2_Full << " ms\n";
        cout << "Q2 Merge Sort: "<< alfiResults.Q2_SortTime<<"ns\n";
        cout << "Q2 Binary Search: "<< alfiResults.Q2_SearchTime<<"ns\n";
        cout << "--------------------------\n";
        cout << "Q3 Total: " << alfiResults.Q3_Full << " ms\n";
        cout << "Q3 Merge Sort: "<< alfiResults.Q3_SortTime<<"ns\n";
        cout << "Q3 Binary Search: "<< alfiResults.Q3_SearchTime<<"ns\n";
        cout << "--------------------------\n";
        cout << "TOTAL: " << alfiResults.Q1_Full + alfiResults.Q2_Full + alfiResults.Q3_Full << " ms\n";
        cout << "===========================\n";
    }
    
    if (hadiRun) {
        cout << "\nHADI (Bubble Sort + Jump Search):\n";
        cout << "Q1 Total: " << hadiResults.Q1_Full << " ms\n";
        cout << "Q1 Bubble Sort: " << hadiResults.Q1_SortTime << " ns\n";
        cout << "--------------------------\n";
        cout << "Q2 Total: " << hadiResults.Q2_Full << " ms\n";
        cout << "Q2 Bubble Sort: " << hadiResults.Q2_SortTime << " ns\n";
        cout << "Q2 No Search Required\n";
        cout << "--------------------------\n";
        cout << "Q3 Total: " << hadiResults.Q3_Full << " ms\n";
        cout << "Q3 No Sort Required\n";
        cout << "Q3 Jump Search: " << hadiResults.Q3_SearchTime << " ns\n";
        cout << "--------------------------\n";
        cout << "TOTAL: " << hadiResults.Q1_Full + hadiResults.Q2_Full + hadiResults.Q3_Full << " ms\n";
        cout << "===========================\n";
    }

    // if (stanlieRun) {
    //     cout << "\nStanlie:\n";
    //     cout << "Q1 Total: " << stanlieResults.Q1_Full << " ms\n";
    //     cout << "Q1 Merge Sort:"<< stanlieResults.Q1_SortTime<<"ns\n";
    //     cout << "--------------------------\n";
    //     cout << "Q2 Total: " << stanlieResults.Q2_Full << " ms\n";
    //     cout << "Q2 Merge Sort: "<< stanlieResults.Q2_SortTime<<"ns\n";
    //     cout << "Q2 Binary Search: "<< stanlieResults.Q2_SearchTime<<"ns\n";
    //     cout << "--------------------------\n";
    //     cout << "Q3 Total: " << stanlieResults.Q3_Full << " ms\n";
    //     cout << "Q3 Merge Sort: "<< stanlieResults.Q3_SortTime<<"ns\n";
    //     cout << "Q3 Binary Search: "<< stanlieResults.Q3_SearchTime<<"ns\n";
    //     cout << "--------------------------\n";
    //     cout << "TOTAL: " << stanlieResults.Q1_Full + alfiResults.Q2_Full + alfiResults.Q3_Full << " ms\n";
    //     cout << "===========================\n";
    // }
        
    // if (baderRun) {
    //     cout << "\nBadr:\n";
    //     cout << "Q1 Total: " << badrResults.Q1_Full << " ms\n";
    //     cout << "Q1 Bubble Sort: " << badrResults.Q1_SortTime << " ns\n";
    //     cout << "--------------------------\n";
    //     cout << "Q2 Total: " << badrResults.Q2_Full << " ms\n";
    //     cout << "Q2 Bubble Sort: " << badrResults.Q2_SortTime << " ns\n";
    //     cout << "Q2 No Search Required\n";
    //     cout << "--------------------------\n";
    //     cout << "Q3 Total: " << badrResults.Q3_Full << " ms\n";
    //     cout << "Q3 No Sort Required\n";
    //     cout << "Q3 Jump Search: " << badrResults.Q3_SearchTime << " ns\n";
    //     cout << "--------------------------\n";
    //     cout << "TOTAL: " << badrResults.Q1_Full + badrResults.Q2_Full + badrResults.Q3_Full << " ms\n";
    //     cout << "===========================\n";
    // }
    
    
    cout << "==========================\n";
}

int main(){
    
    int choice;

    do {
        cout << "\n \tMENU\t\n";
        cout << "1. Merge Sort + Binary Search (Alfiansyah Clark - TP075566)\n";
        cout << "2. Place 2\n";
        cout << "3. Place 3\n";
        cout << "4. Bubble Sort + Jump Search (Abdulhadi Muhammad - TP077939)\n";
        cout << "5. Print Results\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "\nRunning Merge Sort and Binary Search\n";
                alfiResults = Alfie();
                alfiRun = true;
                break;
            case 2:
                cout << "\nStanlies sott+search\n";
                // stanlieResults = badr();
                // stanlieRun = true;
                break;
            case 3:
                cout << "\nBadrs sort + Search\n";
                // badrResults = badr();
                // badrRun = true;
                break;
            case 4:
                cout << "\nBubble Sort + Jump Search: 4\n";
                hadiResults = Hadi();
                hadiRun = true;
                break;
            case 5:
                PrintResults();
                break;
            case 6:
                cout << "\nExiting...\n";
                return 0;
                break;
            default:
                cout << "\nInvalid choice. Please try again.\n";
        }

    } while (choice != 0);
}