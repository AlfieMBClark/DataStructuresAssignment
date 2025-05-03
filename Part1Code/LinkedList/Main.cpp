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


void Alfie(){
    cout << "\nMerge Sort & Binary Search\n";
    Loader loader("transactions_cleaned.csv", "reviews_cleaned.csv");

    if (!loader.loadTrans()||!loader.loadRev()) {
        cerr << "Failed to load files";
        return;
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
                // Add to DateCount linked list
                DateCount* newNode = new DateCount(currentDate.c_str(), dailyCount);
                newNode->next = dateCountHead;
                dateCountHead = newNode;
                uniqueDatesCount++;
                  
                  // Print the date and count
                cout << currentDate << ":\t" << dailyCount << "\n";
                  
                // Reset for next date
                currentDate = current->data.date;
                dailyCount = 1;
            }
            current = current->next;
          }
          
        // Add the last date
        DateCount* newNode = new DateCount(currentDate.c_str(), dailyCount);
        newNode->next = dateCountHead;
        dateCountHead = newNode;
        uniqueDatesCount++;
          
        // Print the last date and count
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

    // Simply create a TransReviewPair list with ALL transactions
    TransReviewPair* trHead = nullptr;
    int pairCount = 0;

    // Copy all transactions directly to TransReviewPair list
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

    // If you want to track reviews as well, create a separate data structure
    struct ReviewInfo {
        char customerID[255];
        ReviewInfo* next;
        
        ReviewInfo(const char* cid) {
            strcpy(customerID, cid);
            next = nullptr;
        }
    };

    // Create a list of reviews
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

    // Now analyze which reviews correspond to transactions
    int matchedReviews = 0;
    int unmatchedReviews = 0;

    // For each review, check if there's a matching customer in the transactions
    ReviewInfo* currReview = reviewInfoHead;
    while (currReview != nullptr) {
        bool foundCustomer = false;
        
        // Check if this customer has any transactions
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

    // Count transactions per date in the TransReviewPair list
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
                // Add to DateCount linked list
                DateCount* newNode = new DateCount(currentRevDate, dailyRevCount);
                newNode->next = revDateCountHead;
                revDateCountHead = newNode;
                
                // Update current date and reset count
                strcpy(currentRevDate, currTR->date);
                dailyRevCount = 1;
            }
            currTR = currTR->next;
        }
        
        // Add the last date
        DateCount* newNode = new DateCount(currentRevDate, dailyRevCount);
        newNode->next = revDateCountHead;
        revDateCountHead = newNode;
    }

    // Compare date counts
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
    
    cout << "Total transactions: " << pairCount << "\n";
    cout << "Total reviews: " << totalReviews << "\n";
    cout << "Reviews matched to customers with transactions: " << matchedReviews << "\n";
    cout << "Reviews from customers with no associated transactions: " << unmatchedReviews << "\n";


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
        return;
    }

    //all Electronics leftright of node
    // backwards
    TransactionNode* firstElectronics = foundNode;
    TransactionNode* prev = nullptr;
    TransactionNode* temp = sortedTransHead;

    //Predecessor map to find the node before any given node
    while (temp != nullptr) {
        if (temp->next == firstElectronics) {
            prev = temp;
            break;
        }
        temp = temp->next;
    }
    //back until first node
    while (prev != nullptr && prev->data.category == "Electronics") {
        firstElectronics = prev;
    
        //node before elect
        temp = sortedTransHead;
        prev = nullptr;
        while (temp != firstElectronics) {
            if (temp->next == firstElectronics) {
                prev = temp;
                break;
            }
            temp = temp->next;
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
    int totalElectronics = 0;
    int creditCardElectronics = 0;

    //Go through each
    TransactionNode* Q2 = firstElectronics;
    while (true) {
        totalElectronics++;
        if (Q2->data.paymentMethod == "Credit Card") {
            creditCardElectronics++;
        }
        if (Q2 == lastElectronics) {
            break;
        }
        
        Q2 = Q2->next;
    }

    double percentage = 0.0;
    if (totalElectronics > 0) {
        percentage = (double)creditCardElectronics / totalElectronics * 100.0;
    }

    auto endTimeQ2 = Clock::now();

    cout << "\nElectronics purchases analysis:\n";
    cout << "Total Electronics transactions: " << totalElectronics << "\n";
    cout << "Electronics transactions using Credit Card: " << creditCardElectronics << "\n";
    cout << "Percentage: " << fixed << setprecision(2) << percentage << "%\n";

    // Performance statistics
    cout << "\nPerformance metrics:\n";
    cout << "Merge sort time: " << chrono::duration_cast<chrono::milliseconds>(MergeTimeQ2 - startTimeQ2).count() << " ms\n";
    cout << "Search and boundary finding time: " << chrono::duration_cast<chrono::nanoseconds>(searchEndTimeQ2 - searchStartTimeQ2).count() << " ns\n";
    cout << "Total analysis time: " << chrono::duration_cast<chrono::milliseconds>(endTimeQ2 - startTimeQ2).count() << " ms\n";
    

}

void Hadi() {

    //Question 1: Sorting transactions by date and counting them

    Loader loader("cleaned_transactions.csv", "cleaned_reviews.csv"); // Load data into linked lists
    loader.loadAll();
    TransactionNode* transHead = loader.getTransHead();
    ReviewNode* reviewHead = loader.getRevHead();

    auto bubblSortLLQ1Start = Clock::now(); // Start timer for Question 1
    bubbleSortTransactionsByDate(transHead); //Sorts linked list by date
    countTransactionsByDate(transHead, loader.getTransCount()); //Prints transaction counts by date
    auto bubbleSortLLQ1End = Clock::now();  // End timer for Question 1
    cout << "Bubble Sort completed in: "
         << chrono::duration_cast<chrono::milliseconds>(bubbleSortLLQ1End - bubblSortLLQ1Start).count() << " ms\n";

    // Question 2: Jump Search for Electronics purchases using Credit Card

    auto bubblSortLLQ2Start = Clock::now();
    bubbleSortTransactionsByCategory(transHead); //Sorts linked list by category
    countCreditCardPurchases(transHead);
    auto bubbleSortLLQ2End = Clock::now(); // End timer for Question 2
    cout << "Jump Search completed in: "
         << chrono::duration_cast<chrono::milliseconds>(bubbleSortLLQ2End - bubblSortLLQ2Start).count() << " ms\n";

    // Question 3: Counting words in 1-star reviews

    auto wordCountStart = Clock::now(); // Start timer for Question 3
    ReviewNode* oneStarReview = jumpSearch(reviewHead, 1);  // 1-star rating
    
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
    }
    


int main(){
    
    int choice;

    do {
        cout << "\n \tMENU\t\n";
        cout << "1. Merge Sort + Binary Search (Alfiansyah Clark - TP075566)\n";
        cout << "2. Place 2\n";
        cout << "3. Place 3\n";
        cout << "4. Bubble Sort + Jump Search (Abdulhadi Muhammad - TP077939)\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "\nRunning Merge Sort and Binary Search\n";
                Alfie();
                break;
            case 2:
                cout << "\nPlace 2\n";
                break;
            case 3:
                cout << "\nPlace 3\n";
                break;
            case 4:
                cout << "\nBubble Sort + Jump Search: 4\n";
                Hadi();
                break;
            case 5:
                cout << "\nExiting...\n";
                return 0;
                break;
            default:
                cout << "\nInvalid choice. Please try again.\n";
        }

    } while (choice != 0);

}