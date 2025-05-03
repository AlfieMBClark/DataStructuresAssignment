#ifndef SORT_ALGORITHMS_H
#define SORT_ALGORITHMS_H


#include "LoadData.h"
#include <cstring>
#include <iostream>

#include "LoadData.h"
#include <cstring>
using namespace std;

//Alfie Stuff
// WordCount struct 
struct WordCount {
    char word[255];
    int count;
    WordCount() {
        word[0] = '\0';
        count = 0;
    }
};

// WordCount node
struct WordCountNode {
    WordCount data;
    WordCountNode* next;
    
    WordCountNode(const WordCount& wc) {
        strcpy(data.word, wc.word);
        data.count = wc.count;
        next = NULL;
    }
};

// Date and count pair - transactions per date
struct DateCount {
    char date[255];
    char count[20];
    DateCount* next;
    
    DateCount(const char* d, int c) {
        strcpy(date, d);
        sprintf(count, "%d", c);
        next = NULL;
    }
};

// TransRev pair 
struct TransReviewPair {
    char date[255];
    char customerID[255];
    TransReviewPair* next;
    
    TransReviewPair(const char* d, const char* cid) {
        strcpy(date, d);
        strcpy(customerID, cid);
        next = NULL;
    }
};

//MergeSort Class
class MergeSortLL {
private:
    // Split the linked list in two halves - helper for all merge sorts
    template<typename NodeType>
    NodeType* splitList(NodeType* head) {
        if (!head || !head->next) return NULL;
        
        NodeType* slow = head;
        NodeType* fast = head->next;
        
        // Use fast/slow pointer technique (tortoise and hare) to find middle
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        
        NodeType* mid = slow->next;
        slow->next = NULL; // Split the list
        return mid;
    }

public:
    // Merge sort for transaction linked list
    TransactionNode* sortTransactions(TransactionNode* head, int sortBy = 4) {
        // Base case: empty list or single element
        if (!head || !head->next) return head;
        
        // Split the list into two halves
        TransactionNode* mid = splitList(head);
        
        // Recursively sort both halves
        head = sortTransactions(head, sortBy);
        mid = sortTransactions(mid, sortBy);
        
        // Merge the sorted halves
        TransactionNode* result = NULL;
        TransactionNode** current = &result;
        
        while (head && mid) {
            bool useHead = false;
            
            // Compare sortBy field
            switch (sortBy) {
                case 0: 
                    useHead = (head->data.customerID <= mid->data.customerID);
                    break;
                case 1: 
                    useHead = (head->data.product <= mid->data.product);
                    break;
                case 2: 
                    useHead = (head->data.category <= mid->data.category);
                    break;
                case 3: 
                    useHead = (head->data.price <= mid->data.price);
                    break;
                case 4: 
                    useHead = (head->data.date <= mid->data.date);
                    break;
                case 5: 
                    useHead = (head->data.paymentMethod <= mid->data.paymentMethod);
                    break;
                default: // default to date
                    useHead = (head->data.date <= mid->data.date);
            }
            
            if (useHead) {
                *current = head;
                head = head->next;
            } else {
                *current = mid;
                mid = mid->next;
            }
            
            current = &((*current)->next);
        }
        
        // Attach remaining list
        *current = (head) ? head : mid;
        
        return result;
    }
    
    // Merge sort Review
    ReviewNode* sortReviews(ReviewNode* head, int sortBy = 0) {
        // Base case: empty list or single element
        if (!head || !head->next) return head;
        
        // Split two halves
        ReviewNode* mid = splitList(head);
        
        //sort both halves
        head = sortReviews(head, sortBy);
        mid = sortReviews(mid, sortBy);
        
        // Merge halves
        ReviewNode* result = NULL;
        ReviewNode** current = &result;
        
        while (head && mid) {
            bool useHead = false;
            
            // Compare sortBy
            switch (sortBy) {
                case 0: 
                    useHead = (head->data.productID <= mid->data.productID);
                    break;
                case 1:
                    useHead = (head->data.customerID <= mid->data.customerID);
                    break;
                case 2:
                    useHead = (head->data.rating <= mid->data.rating);
                    break;
                case 3:
                    useHead = (head->data.reviewText <= mid->data.reviewText);
                    break;
                default:
                    useHead = (head->data.productID <= mid->data.productID);
            }
            
            if (useHead) {
                *current = head;
                head = head->next;
            } else {
                *current = mid;
                mid = mid->next;
            }
            
            current = &((*current)->next);
        }
        
        // Attach remaining list
        *current = (head) ? head : mid;
        
        return result;
    }
    
    // Mergesort TransReviewPair
    TransReviewPair* sortTransReviewPairs(TransReviewPair* head, int sortBy = 0) {
        // Base case: empty list or single element
        if (!head || !head->next) return head;
        
        //two halves
        TransReviewPair* mid = splitList(head);
        
        // sort halves
        head = sortTransReviewPairs(head, sortBy);
        mid = sortTransReviewPairs(mid, sortBy);
        
        // Merge halves
        TransReviewPair* result = NULL;
        TransReviewPair** current = &result;
        
        while (head && mid) {
            bool useHead = false;
            
            // Compare
            if (sortBy == 0) {
                useHead = (strcmp(head->date, mid->date) <= 0);
            } else {
                useHead = (strcmp(head->customerID, mid->customerID) <= 0);
            }
            
            if (useHead) {
                *current = head;
                head = head->next;
            } else {
                *current = mid;
                mid = mid->next;
            }
            
            current = &((*current)->next);
        }
        
        // Attach remaining list
        *current = (head) ? head : mid;
        
        return result;
    }
    
    // Merge sort WordCount 
    WordCountNode* sortWordCounts(WordCountNode* head) {
        // Base case: empty list or single element
        if (!head || !head->next) return head;
        
        // Split halves
        WordCountNode* mid = splitList(head);
        
        //sort 
        head = sortWordCounts(head);
        mid = sortWordCounts(mid);
        
        // Merge
        WordCountNode* result = NULL;
        WordCountNode** current = &result;
        
        while (head && mid) {
            // Sort by count)
            if (head->data.count >= mid->data.count) {
                *current = head;
                head = head->next;
            } else {
                *current = mid;
                mid = mid->next;
            }
            
            current = &((*current)->next);
        }
        
        // Attach remaining list
        *current = (head) ? head : mid;
        
        return result;
    }
};



//----------------------------------------------------------------------------

// Bubble Sort for Linked List

void bubbleSortTransactionsByDate(TransactionNode*& head) {
    if (!head || !head->next) return; // 0 or 1 item

    bool swapped;
    TransactionNode* ptr;
    TransactionNode* lptr = nullptr;

    do {
        swapped = false;
        ptr = head;

        while (ptr->next != lptr) {
            if (ptr->data.date > ptr->next->data.date) {
                // Swap entire Transaction data (not nodes)
                Transaction temp = ptr->data;
                ptr->data = ptr->next->data;
                ptr->next->data = temp;
                swapped = true;
            }
            ptr = ptr->next;
        }

        lptr = ptr; // Mark the last sorted node
    } while (swapped);
}

void countTransactionsByDate(TransactionNode* head, int transCount) {
    if (!head) return;

    cout << "\n===== Transactions by Date =====\n";
    
    string currentDate = head->data.date;
    int count = 1;

    TransactionNode* current = head->next;

    while (current) {
        if (current->data.date == currentDate) {
            count++;
        } else {
            // Output count for the current date
            cout << currentDate << ":\t" << count << " transactions\n";
            currentDate = current->data.date; // Update to the next date
            count = 1; // Reset count
        }
        current = current->next;
    }

    // Output the last date's count
    cout << currentDate << ":\t" << count << " transactions\n";
    cout << "Total Transactions: " << transCount << "\n";
}

#endif