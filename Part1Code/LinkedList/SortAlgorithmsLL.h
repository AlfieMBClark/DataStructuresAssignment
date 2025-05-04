#ifndef SORT_ALGORITHMS_H
#define SORT_ALGORITHMS_H


#include "LoadData.h"
#include <cstring>
#include <string>
#include <iostream>

#include "LoadData.h"
#include <cstring>
using namespace std;


//Alfie Stuff
//Word + freq
struct WordCount {
    char word[255];
    int count;
    WordCount() {
        word[0] = '\0';
        count = 0;
    }
};

// node
struct WordCountNode {
    WordCount data;
    WordCountNode* next;
    
    WordCountNode(const WordCount& wc) {
        strcpy(data.word, wc.word);
        data.count = wc.count;
        next = NULL;
    }
};

//transactions per date
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

class MergeSortLL {
private:
    //Trans split
    TransactionNode* splitTransactionList(TransactionNode* head) {
        if (!head || !head->next) return NULL;
        
        TransactionNode* slow = head;
        TransactionNode* fast = head->next;
        
        // Find mid
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        
        TransactionNode* mid = slow->next;
        slow->next = NULL; // Splits list
        return mid;
    }
    
    //Review split
    ReviewNode* splitReviewList(ReviewNode* head) {
        if (!head || !head->next) return NULL;
        
        ReviewNode* slow = head;
        ReviewNode* fast = head->next;
        
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        
        ReviewNode* mid = slow->next;
        slow->next = NULL;
        return mid;
    }
    
    //TransRevPair split
    TransReviewPair* splitTransReviewPairList(TransReviewPair* head) {
        if (!head || !head->next) return NULL;
        
        TransReviewPair* slow = head;
        TransReviewPair* fast = head->next;
        
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        
        TransReviewPair* mid = slow->next;
        slow->next = NULL;
        return mid;
    }
    
    //WordCount split
    WordCountNode* splitWordCountList(WordCountNode* head) {
        if (!head || !head->next) return NULL;
        
        WordCountNode* slow = head;
        WordCountNode* fast = head->next;
        
        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }
        
        WordCountNode* mid = slow->next;
        slow->next = NULL; 
        return mid;
    }

public:
    // Merge sort transactions
    TransactionNode* sortTransactions(TransactionNode* head, int sortBy = 4) {
        if (!head || !head->next) return head;
        
        // Split into half
        TransactionNode* mid = splitTransactionList(head);
        
        //sort halves
        head = sortTransactions(head, sortBy);
        mid = sortTransactions(mid, sortBy);
        
        //Merge halves
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
                default: //def date
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
        
        // Attach remaining
        *current = (head) ? head : mid;
        
        return result;
    }
    
    // Merge sort Review - Same
    ReviewNode* sortReviews(ReviewNode* head, int sortBy = 0) {
        if (!head || !head->next) return head;
        
        ReviewNode* mid = splitReviewList(head);
    
        head = sortReviews(head, sortBy);
        mid = sortReviews(mid, sortBy);
    
        ReviewNode* result = NULL;
        ReviewNode** current = &result;
        
        while (head && mid) {
            bool useHead = false;
            
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
        
        *current = (head) ? head : mid;
        return result;
    }
    
    // Mergesort TransReviewPair - wsame
    TransReviewPair* sortTransReviewPairs(TransReviewPair* head, int sortBy = 0) {
        if (!head || !head->next) return head;
        
        TransReviewPair* mid = splitTransReviewPairList(head);
        
        head = sortTransReviewPairs(head, sortBy);
        mid = sortTransReviewPairs(mid, sortBy);
        
        TransReviewPair* result = NULL;
        TransReviewPair** current = &result;
        
        while (head && mid) {
            bool useHead = false;
        
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
        
        *current = (head) ? head : mid;
        return result;
    }
    
    // Merge sort WordCount -same
    WordCountNode* sortWordCounts(WordCountNode* head) {
        if (!head || !head->next) return head;
        
        WordCountNode* mid = splitWordCountList(head);
        
        //sort 
        head = sortWordCounts(head);
        mid = sortWordCounts(mid);
        
        // Merge
        WordCountNode* result = NULL;
        WordCountNode** current = &result;
        
        while (head && mid) {
            // Sort by count
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

void bubbleSortTransactionsByCategory(TransactionNode*& head) {
    if (!head || !head->next) return; // No need to sort if list has 0 or 1 element

    bool swapped;
    TransactionNode* ptr;
    TransactionNode* lptr = nullptr;

    do {
        swapped = false;
        ptr = head;

        // Traverse through the list and swap nodes if needed
        while (ptr->next != lptr) {
            if (ptr->data.category > ptr->next->data.category) {
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




//#######################################################################
//######################################################################

// Helper function to heapify the tree
void heapify(TransactionNode* arr[], int n, int i) {
    int largest = i;  // Initialize largest as root
    int left = 2 * i + 1;  // Left child
    int right = 2 * i + 2;  // Right child

    // If left child is larger than root
    if (left < n && arr[left]->data.date > arr[largest]->data.date)
        largest = left;

    // If right child is larger than root
    if (right < n && arr[right]->data.date > arr[largest]->data.date)
        largest = right;

    // If largest is not root
    if (largest != i) {
        swap(arr[i], arr[largest]);
        // Recursively heapify the affected sub-tree
        heapify(arr, n, largest);
    }
}

// Function to perform heap sort
void heapSort(TransactionNode*& head) {
    // Count total nodes in the list
    int n = 0;
    TransactionNode* temp = head;
    while (temp) {
        n++;
        temp = temp->next;
    }

    // If the list is empty or has only one element, no sorting needed
    if (n <= 1) return;

    // Convert linked list to array for heap sort
    TransactionNode** arr = new TransactionNode*[n];
    temp = head;
    for (int i = 0; i < n; i++) {
        arr[i] = temp;
        temp = temp->next;
    }

    // Build a max heap
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }

    // Extract elements from the heap and rebuild it
    for (int i = n - 1; i > 0; i--) {
        // Move the current root to the end
        swap(arr[0], arr[i]);
        
        // Call heapify on the reduced heap
        heapify(arr, i, 0);
    }

    // Rebuild the sorted list from the heapified array
    for (int i = 0; i < n - 1; i++) {
        arr[i]->next = arr[i + 1];
    }
    arr[n - 1]->next = nullptr;  // Last node should point to nullptr

    head = arr[0];  // The new head of the sorted list
    
    // Clean up the array
    delete[] arr;
}

//Stanlie's private domicile
// Make a copy of the transaction node
TransactionNode* DeepCopyTransactionList(TransactionNode* head) {
    if (!head) return nullptr;

    TransactionNode* newHead = new TransactionNode(head->data);
    TransactionNode* currentNew = newHead;
    TransactionNode* currentOld = head->next;

    while (currentOld) {
        currentNew->next = new TransactionNode(currentOld->data);
        currentNew = currentNew->next;
        currentOld = currentOld->next;
    }

    return newHead;
}
//Stanlie sorts
void InsertionSortWordFreqDescending(FrequentWord*& head) {
    FrequentWord* sorted = nullptr;

    while (head) {
        FrequentWord* node = head;
        head = head->next;

        if (!sorted || node->count > sorted->count) {
            node->next = sorted;
            sorted = node;
        } else {
            FrequentWord* temp = sorted;
            while (temp->next && temp->next->count >= node->count)
                temp = temp->next;

            node->next = temp->next;
            temp->next = node;
        }
    }

    head = sorted;
}

TransactionNode* InsertionSortByDate(TransactionNode* head) {
    if (!head || !head->next) return head;

    TransactionNode* sorted = nullptr;

    while (head) {
        TransactionNode* node = head;
        head = head->next;
        node->next = nullptr;

        if (!sorted || node->data.date < sorted->data.date) {
            node->next = sorted;
            sorted = node;
        } else {
            TransactionNode* current = sorted;
            while (current->next && current->next->data.date <= node->data.date) {
                current = current->next;
            }
            node->next = current->next;
            current->next = node;
        }
    }

    return sorted;
}

TransactionNode* InsertionSortByCategory(TransactionNode* head) {
    if (!head || !head->next) return head;

    TransactionNode* sorted = nullptr;  // new sorted list

    while (head) {
        TransactionNode* current = head;
        head = head->next;
        current->next = nullptr;

        if (!sorted || current->data.category < sorted->data.category) {
            current->next = sorted;
            sorted = current;
        } else {
            TransactionNode* temp = sorted;
            while (temp->next && temp->next->data.category < current->data.category) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
    }

    return sorted;
}





// Define a date-count pair structure for Question 1
struct DateCountPair {
    string date;
    int count;
    DateCountPair* next;
    
    DateCountPair(const string& d, int c) : date(d), count(c), next(nullptr) {}
};

// Define a category-transactions structure for Question 2
struct CategoryTransaction {
    TransactionNode* transaction;
    CategoryTransaction* next;
    
    CategoryTransaction(TransactionNode* trans) : transaction(trans), next(nullptr) {}
};

// Simple container for payment method counts
struct PaymentMethodCount {
    string method;
    int count;
    PaymentMethodCount* next;
    
    PaymentMethodCount(const string& m, int c) : method(m), count(c), next(nullptr) {}
};
















#endif