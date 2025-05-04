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
bool badrRun = false;


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









TimingResults Badr() {
    // Initialize timing results with zeros
    TimingResults badrResults = {0, 0, 0, 0, 0, 0, 0, 0};
    
    cout << "\n===== Heap Sort & Hash Search Implementation =====\n";
    
    // Load data
    Loader loader("cleaned_transactions.csv", "cleaned_reviews.csv");
    if (!loader.loadAll()) {
        cerr << "Failed to load data files. Exiting function.\n";
        return badrResults; // Return initialized struct instead of empty return
    }

    TransactionNode* transHead = loader.getTransHead();
    ReviewNode* reviewHead = loader.getRevHead();
    int transCount = loader.getTransCount();
    int revCount = loader.getRevCount();
    
    cout << "Successfully loaded " << transCount << " transactions and " 
         << revCount << " reviews.\n\n";

    // =====================================================================
    // Question 1: Sort transactions by date using Heap Sort
    // =====================================================================
    cout << "========================================================\n";
    cout << "Question 1: Sorting transactions by date using Heap Sort\n";
    cout << "========================================================\n";
    
    auto startTimeQ1 = Clock::now();
    
    // Use heap sort for transactions based on date
    heapSort(transHead);
    
    auto sortTimeQ1 = Clock::now();
    
    // Count and display transactions by date
    cout << "\nTransactions by Date:\n";
    cout << "-------------------------------------------------------\n";
    cout << "Date       | Count       ||    Date       | Count\n";
    cout << "-------------------------------------------------------\n";

    string currentDate = "";
    int dateCount = 0;
    int totalDates = 0;
    
    // Custom linked list to replace vector<pair<string, int>>
    DateCountPair* dateCountsHead = nullptr;
    DateCountPair* dateCountsTail = nullptr;

    // First collect all dates and counts
    TransactionNode* current = transHead;
    while (current != nullptr) {
        if (current->data.date != currentDate) {
            // Save previous date's count (if not the first date)
            if (!currentDate.empty()) {
                DateCountPair* newPair = new DateCountPair(currentDate, dateCount);
                
                // Add to the linked list
                if (dateCountsHead == nullptr) {
                    dateCountsHead = newPair;
                    dateCountsTail = newPair;
                } else {
                    dateCountsTail->next = newPair;
                    dateCountsTail = newPair;
                }
                
                totalDates++;
            }
            
            // Reset for new date
            currentDate = current->data.date;
            dateCount = 1;
        } else {
            dateCount++;
        }
        
        current = current->next;
    }

    // Add the last date
    if (!currentDate.empty()) {
        DateCountPair* newPair = new DateCountPair(currentDate, dateCount);
        
        if (dateCountsHead == nullptr) {
            dateCountsHead = newPair;
        } else {
            dateCountsTail->next = newPair;
            dateCountsTail = newPair;
        }
        
        totalDates++;
    }

    // Now display in two columns
    int halfSize = (totalDates + 1) / 2;
    DateCountPair* leftColumn = dateCountsHead;
    
    // Find the middle element for the right column
    DateCountPair* rightColumn = dateCountsHead;
    for (int i = 0; i < halfSize && rightColumn != nullptr; i++) {
        rightColumn = rightColumn->next;
    }
    
    // Print the two columns
    for (int i = 0; i < halfSize; i++) {
        if (leftColumn != nullptr) {
            cout << left << setw(12) << leftColumn->date << "| " 
                 << setw(10) << leftColumn->count << " ||    ";
            
            leftColumn = leftColumn->next;
        }
        
        // Right column (if it exists)
        if (rightColumn != nullptr) {
            cout << left << setw(12) << rightColumn->date << "| " 
                 << rightColumn->count;
            
            rightColumn = rightColumn->next;
        }
        cout << endl;
    }

    cout << "--------------------------------------------------------------------\n";
    
    auto endTimeQ1 = Clock::now();
    
    cout << "Total unique dates: " << totalDates << "\n";
    cout << "Total transactions: " << transCount << "\n";
    
    auto durationQ1 = chrono::duration_cast<chrono::milliseconds>(endTimeQ1 - startTimeQ1).count();
    auto sortDurationQ1 = chrono::duration_cast<chrono::milliseconds>(sortTimeQ1 - startTimeQ1).count();
    // cout << "Heap Sort completed in: " << sortDurationQ1 << " ms\n";
    cout << "Total processing time: " << durationQ1 << " ms\n\n";

    // Save timing results for Q1
    badrResults.Q1_SortTime = sortDurationQ1;
    badrResults.Q1_Full = durationQ1;

    // Cleanup dateCountsPairs linked list
    while (dateCountsHead != nullptr) {
        DateCountPair* temp = dateCountsHead;
        dateCountsHead = dateCountsHead->next;
        delete temp;
    }

    // =====================================================================
    // Question 2: Analyze Electronics purchases with Credit Card
    // =====================================================================
    cout << "===========================================================\n";
    cout << "Question 2: Electronics purchases with Credit Card analysis\n";
    cout << "===========================================================\n";
    
    auto startTimeQ2 = Clock::now();
    auto sortTimeQ2 = startTimeQ2; // Just initialize, may not be used
    
    // Use custom structure instead of map<string, vector<TransactionNode*>>
    struct CategoryMap {
        string category;
        CategoryTransaction* transactions;
        CategoryMap* next;
        
        CategoryMap(const string& cat) : category(cat), transactions(nullptr), next(nullptr) {}
    };
    
    CategoryMap* categoryMapHead = nullptr;
    int totalProcessed = 0;
    
    // Fill the map with transactions categorized by category
    current = transHead;
    while (current != nullptr) {
        // Find or create category
        CategoryMap* categoryNode = nullptr;
        
        // Search for existing category
        CategoryMap* mapIter = categoryMapHead;
        while (mapIter != nullptr) {
            if (mapIter->category == current->data.category) {
                categoryNode = mapIter;
                break;
            }
            mapIter = mapIter->next;
        }
        
        // Create new category if not found
        if (categoryNode == nullptr) {
            categoryNode = new CategoryMap(current->data.category);
            categoryNode->next = categoryMapHead;
            categoryMapHead = categoryNode;
        }
        
        // Add transaction to category
        CategoryTransaction* newTrans = new CategoryTransaction(current);
        newTrans->next = categoryNode->transactions;
        categoryNode->transactions = newTrans;
        
        current = current->next;
        totalProcessed++;
    }
    
    cout << "Processed " << totalProcessed << " transactions using hash-based categorization.\n";
    
    // Count categories
    int uniqueCategories = 0;
    CategoryMap* catIter = categoryMapHead;
    while (catIter != nullptr) {
        uniqueCategories++;
        catIter = catIter->next;
    }
    
    cout << "Found " << uniqueCategories << " unique product categories.\n\n";
    
    // Now analyze the "Electronics" category transactions
    CategoryMap* electronicsCategory = nullptr;
    
    // Find Electronics category
    catIter = categoryMapHead;
    while (catIter != nullptr) {
        if (catIter->category == "Electronics") {
            electronicsCategory = catIter;
            break;
        }
        catIter = catIter->next;
    }
    
    if (electronicsCategory != nullptr) {
        int electronicsCount = 0;
        int creditCardCount = 0;
        double totalValue = 0.0;
        
        // Count total Electronics transactions
        CategoryTransaction* transIter = electronicsCategory->transactions;
        while (transIter != nullptr) {
            electronicsCount++;
            if (transIter->transaction->data.paymentMethod == "Credit Card") {
                creditCardCount++;
                totalValue += transIter->transaction->data.price;
            }
            transIter = transIter->next;
        }
        
        // Calculate the percentage
        double percentage = 0.0;
        if (electronicsCount > 0) {
            percentage = (static_cast<double>(creditCardCount) / electronicsCount) * 100;
        }
        
        // Show distribution of other payment methods
        PaymentMethodCount* paymentMethodsHead = nullptr;
        
        // Count payment methods
        transIter = electronicsCategory->transactions;
        while (transIter != nullptr) {
            string method = transIter->transaction->data.paymentMethod;
            
            // Find or create payment method counter
            PaymentMethodCount* methodNode = nullptr;
            PaymentMethodCount* methodIter = paymentMethodsHead;
            
            while (methodIter != nullptr) {
                if (methodIter->method == method) {
                    methodNode = methodIter;
                    break;
                }
                methodIter = methodIter->next;
            }
            
            if (methodNode == nullptr) {
                methodNode = new PaymentMethodCount(method, 0);
                methodNode->next = paymentMethodsHead;
                paymentMethodsHead = methodNode;
            }
            
            methodNode->count++;
            transIter = transIter->next;
        }
        
        // Display in a nicely formatted table
        cout << "+---------------------------------------+\n";
        cout << "|      Electronics Category Analysis    |\n";
        cout << "+---------------------------------------+\n";
        cout << "| Total Electronics Purchases: " << setw(8) << electronicsCount << " |\n";
        cout << "| Purchases via Credit Card:   " << setw(8) << creditCardCount << " |\n";
        cout << "| Percentage via Credit Card:  " << setw(7) << fixed << setprecision(2) << percentage << "% |\n";
        cout << "| Total Credit Card Value:     $" << setw(9) << fixed << setprecision(2) << totalValue << " |\n";
        cout << "+---------------------------------------+\n";
        cout << "\nPayment Method Distribution:\n";
        cout << "+------------------+--------+-----------+\n";
        cout << "| Payment Method   | Count  | Percent   |\n";
        cout << "+------------------+--------+-----------+\n";

        PaymentMethodCount* methodIter = paymentMethodsHead;
        while (methodIter != nullptr) {
            double methodPercentage = (static_cast<double>(methodIter->count) / electronicsCount) * 100;
            cout << "| " << setw(16) << left << methodIter->method << " | " 
                 << setw(6) << right << methodIter->count << " | " 
                 << setw(7) << fixed << setprecision(2) << methodPercentage << "% |\n";
            
            methodIter = methodIter->next;
        }
        cout << "+------------------+--------+-----------+\n";
        
        // Cleanup payment methods
        while (paymentMethodsHead != nullptr) {
            PaymentMethodCount* temp = paymentMethodsHead;
            paymentMethodsHead = paymentMethodsHead->next;
            delete temp;
        }
    } else {
        cout << "No Electronics Purchases found in the dataset.\n";
    }
    
    auto endTimeQ2 = Clock::now();
    auto durationQ2 = chrono::duration_cast<chrono::milliseconds>(endTimeQ2 - startTimeQ2).count();
    cout << "\nHash Search completed in: " << durationQ2 << " ms\n\n";

    // Cleanup category map
    while (categoryMapHead != nullptr) {
        // Cleanup transactions in this category
        CategoryTransaction* transHead = categoryMapHead->transactions;
        while (transHead != nullptr) {
            CategoryTransaction* temp = transHead;
            transHead = transHead->next;
            delete temp;
        }
        
        CategoryMap* tempCat = categoryMapHead;
        categoryMapHead = categoryMapHead->next;
        delete tempCat;
    }

    // Save timing results for Q2
    badrResults.Q2_SortTime = 0; // No specific sort time tracked
    badrResults.Q2_SearchTime = 0; // No specific search time tracked 
    badrResults.Q2_Full = durationQ2;

    // =====================================================================
    // Question 3: Analyze word frequency in 1-star reviews
    // =====================================================================
    // I'm keeping this section as is since it doesn't use vectors
    cout << "=================================================\n";    
    cout << "Question 3: Most frequent words in 1-star reviews\n";
    cout << "=================================================\n";
    
    auto startTimeQ3 = Clock::now();
    auto searchStartTimeQ3 = startTimeQ3; // Just initialize, may not be used
    
    // Find indices of 1-star reviews
    int firstIdx = -1, lastIdx = -1;
    findOneStarReviews(firstIdx, lastIdx, revCount, reviewHead);
    
    if (firstIdx == -1 || lastIdx == -1) {
        cout << "No 1-star reviews found in the dataset.\n";
        return badrResults; // Return with timing data collected so far
    }
    
    int oneStarCount = lastIdx - firstIdx + 1;
    cout << "Found " << oneStarCount << " 1-star reviews.\n";
    
    // Allocate space for word counting (assume average of 20 unique words per review)
    WordCount* wordCounts = new WordCount[oneStarCount * 20];
    int uniqueWords = 0;
    
    // Count word frequencies
    countWordFrequencies(firstIdx, lastIdx, reviewHead, wordCounts, uniqueWords);
    
    // Convert array to linked list for sorting
    WordCountNode* wordCountHead = createWordCountList(wordCounts, uniqueWords);
    
    // Sort word counts (use merge sort for this since we have it implemented)
    MergeSortLL sortHelper;
    wordCountHead = sortHelper.sortWordCounts(wordCountHead);
    
    auto searchEndTimeQ3 = Clock::now();
    
    // Display the top 15 most frequent words with improved formatting
    cout << "\nTop 15 most frequent words in 1-star reviews:\n";
    cout << "+=======+=======================+==========+\n";
    cout << "| Rank  | Word                 | Frequency |\n";
    cout << "+=======+=======================+==========+\n";
    
    WordCountNode* wcNode = wordCountHead;
    int rank = 1;
    
    while (wcNode != nullptr && rank <= 15) {
        // Format output with table styling
        cout << "| " << setw(5) << rank << " | "
             << setw(21) << left << wcNode->data.word << " | "
             << setw(8) << right << wcNode->data.count << " |\n";
        
        wcNode = wcNode->next;
        rank++;
    }
    
    cout << "+=======+=======================+==========+\n";
    cout << "Total unique words found: " << uniqueWords << "\n";
    
    auto endTimeQ3 = Clock::now();
    auto durationQ3 = chrono::duration_cast<chrono::milliseconds>(endTimeQ3 - startTimeQ3).count();
    auto searchDurationQ3 = chrono::duration_cast<chrono::nanoseconds>(searchEndTimeQ3 - searchStartTimeQ3).count();
    
    cout << "\nWord frequency analysis completed in: " << durationQ3 << " ms\n";
    
    // Clean up allocated memory
    delete[] wordCounts;
    
    // Free the linked list
    while (wordCountHead) {
        WordCountNode* temp = wordCountHead;
        wordCountHead = wordCountHead->next;
        delete temp;
    }

    // Save timing results for Q3
    badrResults.Q3_SortTime = 0; // No specific sort time tracked
    badrResults.Q3_SearchTime = searchDurationQ3;
    badrResults.Q3_Full = durationQ3;

    // =====================================================================
    // Performance Summary
    // =====================================================================
    cout << "\n+===============================================+\n";
    cout << "|             Performance Summary              |\n";
    cout << "+===============================================+\n";
    cout << "| Q1 (Heap Sort):          " << setw(15) << durationQ1 << " ms |\n";
    cout << "| Q2 (Hash Search):        " << setw(15) << durationQ2 << " ms |\n";
    cout << "| Q3 (Word Frequency):     " << setw(15) << durationQ3 << " ms |\n";
    cout << "+-----------------------------------------------+\n";
    cout << "| Total execution time:    " << setw(15) << (durationQ1 + durationQ2 + durationQ3) << " ms |\n";
    cout << "+===============================================+\n";

    return badrResults; // Return the timing results
}






void PrintResults() {
    if (!alfiRun && !hadiRun && !badrRun) {
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
        
    if (badrRun) {
        cout << "\nBadr:\n";
        cout << "Q1 Total: " << badrResults.Q1_Full << " ms\n";
        cout << "Q1 Heap Sort: " << badrResults.Q1_SortTime << " ns\n";
        cout << "--------------------------\n";
        cout << "Q2 Total: " << badrResults.Q2_Full << " ms\n";
        cout << "Q2 Heap Sort: " << badrResults.Q2_SortTime << " ns\n";
        cout << "Q2 No Search Required\n";
        cout << "--------------------------\n";
        cout << "Q3 Total: " << badrResults.Q3_Full << " ms\n";
        cout << "Q3 No Sort Required\n";
        cout << "Q3 Hashing Search: " << badrResults.Q3_SearchTime << " ns\n";
        cout << "--------------------------\n";
        cout << "TOTAL: " << badrResults.Q1_Full + badrResults.Q2_Full + badrResults.Q3_Full << " ms\n";
        cout << "===========================\n";
    }
    
    
    cout << "==========================\n";
}

int main(){
    
    int choice;

    do {
        cout << "\n \tMENU\t\n";
        cout << "1. Merge Sort + Binary Search (Alfiansyah Clark - TP075566)\n";
        cout << "2. Place 2\n";
        cout << "3. Heap Sort + Hashing Search (Badr Abduldaim - TP074644)\n";
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
                cout << "\nHeap Sort & Hashing Search\n";
                badrResults = Badr();
                badrRun = true;
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