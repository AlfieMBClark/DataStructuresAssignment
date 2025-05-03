#ifndef SEARCH_ALGORITHMS_LL_H
#define SEARCH_ALGORITHMS_LL_H

#include "LoadData.h"
#include <string>

TransactionNode* binarySearchTransactions(TransactionNode* head, const char* target, int field) {
    // Base case: empty list
    if (head == nullptr) {
        return nullptr;
    }
    
    // Count total nodes for halving the list
    int count = 0;
    TransactionNode* current = head;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    
    // Find the middle node
    TransactionNode* middle = head;
    for (int i = 0; i < count / 2; i++) {
        middle = middle->next;
    }
    
    // Get field value from middle node
    const char* value = nullptr;
    switch (field) {
        case 0: value = middle->data.customerID.c_str(); break;
        case 1: value = middle->data.product.c_str(); break;
        case 2: value = middle->data.category.c_str(); break;
        case 3: value = std::to_string(middle->data.price).c_str(); break;
        case 4: value = middle->data.date.c_str(); break;
        case 5: value = middle->data.paymentMethod.c_str(); break;
        default: value = middle->data.customerID.c_str(); break;
    }
    
    // Compare with target
    int comparison = strcmp(value, target);
    
    if (comparison == 0) {
        // Found a match
        return middle;
    } else if (comparison > 0) {
        // Target is in left half - create a sublist and search
        if (count <= 1) {
            return nullptr; // Not found
        }
        
        // Create a temporary list of the left half
        TransactionNode* left = head;
        TransactionNode* temp = left;
        for (int i = 0; i < count / 2 - 1; i++) {
            temp = temp->next;
        }
        
        TransactionNode* origNext = temp->next;
        temp->next = nullptr; // Temporarily break the list
        
        // Recursive search on left half
        TransactionNode* result = binarySearchTransactions(left, target, field);
        
        // Restore the list
        temp->next = origNext;
        
        return result;
    } else {
        // Target is in right half - search in second half
        if (middle->next == nullptr) {
            return nullptr; // Not found
        }
        
        return binarySearchTransactions(middle->next, target, field);
    }
}


ReviewNode* binarySearchReviews(ReviewNode* head, const char* target, int field) {
    // Base case: empty list
    if (head == nullptr) {
        return nullptr;
    }
    
    // Count total nodes for halving the list
    int count = 0;
    ReviewNode* current = head;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    
    // Find the middle node
    ReviewNode* middle = head;
    for (int i = 0; i < count / 2; i++) {
        middle = middle->next;
    }
    
    // Get field value from middle node
    const char* value = nullptr;
    switch (field) {
        case 0: value = middle->data.productID.c_str(); break;
        case 1: value = middle->data.customerID.c_str(); break;
        case 2: value = std::to_string(middle->data.rating).c_str(); break;
        case 3: value = middle->data.reviewText.c_str(); break;
        default: value = middle->data.productID.c_str(); break;
    }
    
    // Compare with target
    int comparison = strcmp(value, target);
    
    if (comparison == 0) {
        // Found a match
        return middle;
    } else if (comparison > 0) {
        // Target is in left half - create a sublist and search
        if (count <= 1) {
            return nullptr; // Not found
        }
        
        // Create a temporary list of the left half
        ReviewNode* left = head;
        ReviewNode* temp = left;
        for (int i = 0; i < count / 2 - 1; i++) {
            temp = temp->next;
        }
        
        ReviewNode* origNext = temp->next;
        temp->next = nullptr; // Temporarily break the list
        
        // Recursive search on left half
        ReviewNode* result = binarySearchReviews(left, target, field);
        
        // Restore the list
        temp->next = origNext;
        
        return result;
    } else {
        // Target is in right half - search in second half
        if (middle->next == nullptr) {
            return nullptr; // Not found
        }
        
        return binarySearchReviews(middle->next, target, field);
    }
}


#endif 