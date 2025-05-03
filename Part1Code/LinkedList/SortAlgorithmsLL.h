#ifndef SORT_ALGORITHMS_H
#define SORT_ALGORITHMS_H


#include "LoadData.h"
#include <cstring>
#include <iostream>





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