#ifndef SEARCH_ALGORITHMS_LL_H
#define SEARCH_ALGORITHMS_LL_H

#include "LoadData.h"
#include <string>
#include <math.h>
#include <map>
#include <sstream>
#include <cstring>
#include <vector>
#include <algorithm>

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

//--------------------------------------------------------------------

//Jump Search

TransactionNode* jumpSearchElectronics(TransactionNode* head, const string& targetCategory) {
    int n = 0;
    TransactionNode* current = head;

    // Find the length of the linked list
    while (current != NULL) {
        n++;
        current = current->next;
    }

    // Set the step size as sqrt(n)
    int step = sqrt(n);
    current = head;

    // Jump forward by 'step' and then linear search within the block
    for (int i = 0; i < n; i += step) {
        if (current->data.category == targetCategory) {
            return current;  // Found a match for electronics, return the node
        }
        for (int j = 0; j < step && current != NULL; ++j) {
            if (current->data.category == targetCategory) {
                return current;  // Found a match, return the node
            }
            current = current->next;
        }
    }

    return NULL;  // If no match found
}

// Function to count credit card purchases for electronics
void countCreditCardPurchases(TransactionNode* head) {
    int electronicsCount = 0;
    int creditCardCount = 0;

    TransactionNode* current = head;

    while (current != NULL) {
        if (current->data.category == "Electronics") {  // Check if it's an electronics purchase
            electronicsCount++;
            if (current->data.paymentMethod == "Credit Card") {
                creditCardCount++;  // Count credit card payments
            }
        }
        current = current->next;
    }

    // Output the counts
    cout << "Total Electronics Purchases: " << electronicsCount << endl;
    cout << "Total Electronics Purchases via Credit Card: " << creditCardCount << endl;

    // Calculate and print the percentage of credit card purchases
    if (electronicsCount > 0) {
        double percentage = (static_cast<double>(creditCardCount) / electronicsCount) * 100;
        cout << "Percentage of Electronics Purchases via Credit Card: " << percentage << "%" << endl;
    } else {
        cout << "No Electronics Purchases found." << endl;
    }
}

// Function to count reviews with a specific rating
ReviewNode* jumpSearch(ReviewNode* head, int targetRating) {
    int n = 0;
    ReviewNode* current = head;

    // Find the length of the linked list
    while (current != NULL) {
        n++;
        current = current->next;
    }

    // Set the step size as sqrt(n)
    int step = sqrt(n);
    current = head;

    // Jump forward by 'step' and then linear search within the block
    for (int i = 0; i < n; i += step) {
        if (current->data.rating == targetRating) {
            return current;  // Found a match, return the node
        }
        for (int j = 0; j < step && current != NULL; ++j) {
            if (current->data.rating == targetRating) {
                return current;  // Found a match, return the node
            }
            current = current->next;
        }
    }

    return NULL;  // If no match found
}

void tallyWordsInReview(ReviewNode* reviewHead) {
    map<string, int> wordCount;
    ReviewNode* current = reviewHead;

    while (current != NULL) {
        if (current->data.rating == 1) {
            stringstream ss(current->data.reviewText);
            string word;

            while (ss >> word) {
                // Remove non-alphabetic characters
                word.erase(remove_if(word.begin(), word.end(), [](char c) { return !isalpha(c); }), word.end());

                // Convert to lowercase
                transform(word.begin(), word.end(), word.begin(), ::tolower);

                if (!word.empty()) {
                    wordCount[word]++;
                }
            }
        }
        current = current->next;
    }

    // Move map data into vector for sorting
    vector<pair<string, int>> sortedWords(wordCount.begin(), wordCount.end());

    // Sort by frequency (descending), then alphabetically
    sort(sortedWords.begin(), sortedWords.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
        return a.second != b.second ? a.second > b.second : a.first < b.first;
    });

    // Print sorted results
    for (const auto& entry : sortedWords) {
        cout << entry.first << ": " << entry.second << endl;
    }
}
#endif 