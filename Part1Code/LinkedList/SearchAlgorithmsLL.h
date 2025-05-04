#ifndef SEARCH_ALGORITHMS_LL_H
#define SEARCH_ALGORITHMS_LL_H

#include "LoadData.h"
#include <string>
#include <math.h>
#include <map>
#include <sstream>
#include <cstring>
#include <cctype>
#include <vector>
#include <algorithm>
#include <iomanip>

TransactionNode* binarySearchTransactions(TransactionNode* head, const char* target, int field) {
    if (head == nullptr) {
        return nullptr;
    }
    
    //total node
    int count = 0;
    TransactionNode* current = head;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    
    // mid
    TransactionNode* middle = head;
    for (int i = 0; i < count / 2; i++) {
        middle = middle->next;
    }
    
    // value mid
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
    
    // Comp targ
    int comparison = strcmp(value, target);
    
    if (comparison == 0) {
        //match
        return middle;
    } else if (comparison > 0) {
        //left half search
        if (count <= 1) {
            return nullptr;
        }
        
        //left half lis
        TransactionNode* left = head;
        TransactionNode* temp = left;
        for (int i = 0; i < count / 2 - 1; i++) {
            temp = temp->next;
        }
        
        TransactionNode* origNext = temp->next;
        temp->next = nullptr; 

        //serch left half
        TransactionNode* result = binarySearchTransactions(left, target, field);
        
        //Restore
        temp->next = origNext;
        
        return result;
    } else {
        // Target right half
        if (middle->next == nullptr) {
            return nullptr; 
        }
        
        return binarySearchTransactions(middle->next, target, field);
    }
}


ReviewNode* binarySearchReviews(ReviewNode* head, const char* target, int field) {
    if (head == nullptr) {
        return nullptr;
    }
    
    //otal nodes for half
    int count = 0;
    ReviewNode* current = head;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    
    //mid node
    ReviewNode* middle = head;
    for (int i = 0; i < count / 2; i++) {
        middle = middle->next;
    }
    
    //value from mid
    const char* value = nullptr;
    switch (field) {
        case 0: value = middle->data.productID.c_str(); break;
        case 1: value = middle->data.customerID.c_str(); break;
        case 2: value = std::to_string(middle->data.rating).c_str(); break;
        case 3: value = middle->data.reviewText.c_str(); break;
        default: value = middle->data.productID.c_str(); break;
    }
    
    // Comp to target
    int comparison = strcmp(value, target);
    
    if (comparison == 0) {
        //match found
        return middle;
    } else if (comparison > 0) {
        // Target left half - sublist
        if (count <= 1) {
            return nullptr; // Not found
        }
        
        //tmp list left
        ReviewNode* left = head;
        ReviewNode* temp = left;
        for (int i = 0; i < count / 2 - 1; i++) {
            temp = temp->next;
        }
        
        ReviewNode* origNext = temp->next;
        temp->next = nullptr; 
        
        //search left
        ReviewNode* result = binarySearchReviews(left, target, field);
        
        // Restore the list
        temp->next = origNext;
        
        return result;
    } else {
        // Target right half
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


//#############################################################################
//################################################################################

// function declarations at the top of the file
void hashSearchCategoryPayment(TransactionNode* head, int transCount);
void findOneStarReviews(int& first, int& last, int revCount, ReviewNode* reviewHead);
void countWordFrequencies(int first, int last, ReviewNode* reviewHead, WordCount* wc, int& unique);
WordCountNode* createWordCountList(WordCount* wc, int unique);
void displayTopWords(WordCountNode* head, int unique, int topN);




//implementations at the end of the file
// Define a node for storing Transaction pointers in a linked list
struct TransactionListNode {
    TransactionNode* transaction;
    TransactionListNode* next;
    
    TransactionListNode(TransactionNode* trans) : transaction(trans), next(nullptr) {}
};

// Define a category entry to replace map entries
struct CategoryEntry {
    string category;
    TransactionListNode* transactions;
    int count;
    CategoryEntry* next;
    
    CategoryEntry(const string& cat) : category(cat), transactions(nullptr), count(0), next(nullptr) {}
    
    // Add a transaction to this category
    void addTransaction(TransactionNode* trans) {
        TransactionListNode* newNode = new TransactionListNode(trans);
        newNode->next = transactions;
        transactions = newNode;
        count++;
    }
    
    // Clean up the transaction list
    ~CategoryEntry() {
        while (transactions) {
            TransactionListNode* temp = transactions;
            transactions = transactions->next;
            delete temp;
        }
    }
};

// Define a payment method counter node
struct PaymentMethodNode {
    string method;
    int count;
    PaymentMethodNode* next;
    
    PaymentMethodNode(const string& m) : method(m), count(1), next(nullptr) {}
};

// Function to implement hash-based search without vectors
void hashSearchCategoryPayment(TransactionNode* head, int transCount) {
    // Create a custom hash table using linked lists instead of map<string, vector>
    CategoryEntry* categoryTable = nullptr;
    
    // Track total transactions processed
    int totalProcessed = 0;
    int uniqueCategories = 0;
    
    // Fill our custom structure with transactions categorized by category
    TransactionNode* current = head;
    while (current != nullptr) {
        // Find if category already exists
        CategoryEntry* categoryNode = nullptr;
        CategoryEntry* tempCat = categoryTable;
        
        while (tempCat != nullptr) {
            if (tempCat->category == current->data.category) {
                categoryNode = tempCat;
                break;
            }
            tempCat = tempCat->next;
        }
        
        // If category doesn't exist, create it
        if (categoryNode == nullptr) {
            categoryNode = new CategoryEntry(current->data.category);
            categoryNode->next = categoryTable;
            categoryTable = categoryNode;
            uniqueCategories++;
        }
        
        // Add transaction to the category
        categoryNode->addTransaction(current);
        
        current = current->next;
        totalProcessed++;
    }
    
    cout << "Processed " << totalProcessed << " transactions using hash-based categorization.\n";
    cout << "Found " << uniqueCategories << " unique product categories.\n\n";
    
    // Find "Electronics" category
    CategoryEntry* electronicsCategory = nullptr;
    CategoryEntry* tempCat = categoryTable;
    
    while (tempCat != nullptr) {
        if (tempCat->category == "Electronics") {
            electronicsCategory = tempCat;
            break;
        }
        tempCat = tempCat->next;
    }
    
    // Now analyze the "Electronics" category transactions
    if (electronicsCategory != nullptr) {
        int electronicsCount = electronicsCategory->count;
        int creditCardCount = 0;
        double totalValue = 0.0;
        
        // Count credit card payments and total value
        TransactionListNode* trans = electronicsCategory->transactions;
        
        // Create payment method counter
        PaymentMethodNode* paymentMethods = nullptr;
        
        while (trans != nullptr) {
            // Check if it's a credit card transaction
            if (trans->transaction->data.paymentMethod == "Credit Card") {
                creditCardCount++;
                totalValue += trans->transaction->data.price;
            }
            
            // Find or add payment method in our counter
            PaymentMethodNode* pmNode = nullptr;
            PaymentMethodNode* pmTemp = paymentMethods;
            
            while (pmTemp != nullptr) {
                if (pmTemp->method == trans->transaction->data.paymentMethod) {
                    pmNode = pmTemp;
                    break;
                }
                pmTemp = pmTemp->next;
            }
            
            if (pmNode != nullptr) {
                // Payment method found, increment counter
                pmNode->count++;
            } else {
                // New payment method, add to list
                pmNode = new PaymentMethodNode(trans->transaction->data.paymentMethod);
                pmNode->next = paymentMethods;
                paymentMethods = pmNode;
            }
            
            trans = trans->next;
        }
        
        cout << "Electronics Category Analysis:\n";
        cout << "-----------------------------\n";
        cout << "Total Electronics Purchases: " << electronicsCount << "\n";
        cout << "Electronics Purchases via Credit Card: " << creditCardCount << "\n";
        
        // Calculate the percentage
        double percentage = 0.0;
        if (electronicsCount > 0) {
            percentage = (static_cast<double>(creditCardCount) / electronicsCount) * 100;
            cout << "Percentage of Electronics Purchases via Credit Card: " 
                 << fixed << setprecision(2) << percentage << "%\n";
            cout << "Total value of Electronics purchases via Credit Card: $" 
                 << fixed << setprecision(2) << totalValue << "\n";
        }
        
        cout << "\nPayment Method Distribution for Electronics:\n";
        PaymentMethodNode* method = paymentMethods;
        
        while (method != nullptr) {
            double methodPercentage = (static_cast<double>(method->count) / electronicsCount) * 100;
            cout << method->method << ": " << method->count << " (" 
                 << fixed << setprecision(2) << methodPercentage << "%)\n";
            
            method = method->next;
        }
        
        // Clean up payment methods
        while (paymentMethods != nullptr) {
            PaymentMethodNode* temp = paymentMethods;
            paymentMethods = paymentMethods->next;
            delete temp;
        }
    } else {
        cout << "No Electronics Purchases found in the dataset.\n";
    }
    
    // Clean up category table
    while (categoryTable != nullptr) {
        CategoryEntry* temp = categoryTable;
        categoryTable = categoryTable->next;
        delete temp; // This calls the destructor which cleans up transaction list
    }
}

// Function to find the range of 1-star reviews
void findOneStarReviews(int& first, int& last, int revCount, ReviewNode* reviewHead) {
    first = -1;
    last = -1;
    int index = 0;
    ReviewNode* current = reviewHead;
    
    while (current != nullptr) {
        if (current->data.rating == 1) {
            if (first == -1) first = index;
            last = index;
        }
        current = current->next;
        index++;
    }
}

// Function to count word frequencies in 1-star reviews
void countWordFrequencies(int first, int last, ReviewNode* reviewHead, WordCount* wc, int& unique) {
    // Common stop words to filter out
    const char* stopWords[] = {"the", "a", "an", "and", "or", "but", "is", "are", "was", "were", 
                             "has", "have", "had", "be", "been", "being", "in", "on", "at", "to", 
                             "for", "with", "by", "about", "of", "that", "this", "these", "those"};
    const int stopWordsCount = sizeof(stopWords) / sizeof(stopWords[0]);
    
    int index = 0;
    ReviewNode* current = reviewHead;
    unique = 0;
    
    while (current != nullptr) {
        if (index >= first && index <= last) {
            stringstream ss(current->data.reviewText);
            string word;
            
            while (ss >> word) {
                // Remove non-alphabetic characters
                string cleanWord = "";
                for (char c : word) {
                    if (isalpha(c)) {
                        cleanWord += tolower(c);
                    }
                }
                
                // Skip empty words or words that are too short
                if (cleanWord.length() <= 1) {
                    continue;
                }
                
                // Skip common stop words
                bool isStopWord = false;
                for (int i = 0; i < stopWordsCount; i++) {
                    if (cleanWord == stopWords[i]) {
                        isStopWord = true;
                        break;
                    }
                }
                
                if (isStopWord) {
                    continue;
                }
                
                // Check if word is already counted
                bool found = false;
                for (int i = 0; i < unique; i++) {
                    if (strcmp(wc[i].word, cleanWord.c_str()) == 0) {
                        wc[i].count++;
                        found = true;
                        break;
                    }
                }
                
                // Add new word if not found
                if (!found && unique < 1000) { // Limit to prevent overflow
                    if (cleanWord.length() < 255) {  // Ensure word fits in our buffer
                        strncpy(wc[unique].word, cleanWord.c_str(), 254);
                        wc[unique].word[254] = '\0';  // Ensure null termination
                        wc[unique].count = 1;
                        unique++;
                    }
                }
            }
        }
        
        current = current->next;
        index++;
    }
    
    cout << "Processed " << (last - first + 1) << " reviews with 1-star ratings.\n";
    cout << "Found " << unique << " unique words after filtering stop words.\n";
}

// Function to create a linked list of word counts
WordCountNode* createWordCountList(WordCount* wc, int unique) {
    WordCountNode* head = nullptr;
    for (int i = 0; i < unique; i++) {
        WordCountNode* newNode = new WordCountNode(wc[i]);
        newNode->next = head;
        head = newNode;
    }
    return head;
}

// Function to display top frequent words
void displayTopWords(WordCountNode* head, int unique, int topN) {
    if (head == nullptr || topN <= 0) {
        cout << "No words to display.\n";
        return;
    }
    
    cout << "\n----------------------------------------\n";
    cout << "Rank | Word                | Frequency \n";
    cout << "----------------------------------------\n";
    
    WordCountNode* current = head;
    int rank = 1;
    
    while (current != nullptr && rank <= topN) {
        // Format output for better readability
        cout << setw(4) << rank << " | "
             << setw(20) << left << current->data.word << " | "
             << setw(9) << right << current->data.count << "\n";
        
        current = current->next;
        rank++;
    }
    
    cout << "----------------------------------------\n";
    cout << "Total unique words found: " << unique << "\n";
}

//Stanlie

void LinearSearch(ReviewNode* head, FrequentWord*& freqHead) {
    freqHead = nullptr;

    while (head) {
        if (head->data.rating == 1) {
            std::string review = head->data.reviewText;

            for (char& ch : review)
                ch = std::isalpha(ch) ? std::tolower(ch) : ' ';

            std::stringstream ss(review);
            std::string token;

            while (ss >> token) {
                FrequentWord* current = freqHead;
                bool found = false;

                while (current) {
                    if (current->word == token) {
                        current->count++;
                        found = true;
                        break;
                    }
                    current = current->next;
                }

                if (!found) {
                    FrequentWord* newNode = new FrequentWord(token);
                    newNode->next = freqHead;
                    freqHead = newNode;
                }
            }
        }
        head = head->next;
    }
}

void LinearSearchElectronics(TransactionNode* head, int& totalElectronics, int& creditCardCount) {
    totalElectronics = 0;
    creditCardCount = 0;

    while (head) {
        // Use .find to allow for small variations like leading/trailing spaces
        if (head->data.category.find("Electronics") != std::string::npos) {
            totalElectronics++;
            if (head->data.paymentMethod.find("Credit Card") != std::string::npos) {
                creditCardCount++;
            }
        }
        head = head->next;
    }
}














#endif 