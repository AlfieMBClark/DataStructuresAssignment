#ifndef SEARCH_ALGORITHMS_H
#define SEARCH_ALGORITHMS_H

#include "LoadData.h"
#include "SortAlgorithms.h"
#include <iostream>
#include <iomanip>
#include <cstring>
#include <chrono>  
#include <cctype>
#include <math.h>
using namespace std;

//binary search transactions
int binarySearch(char SortedArray[][TransactionFields][FieldLength],int  size,const char* target,int  column) {
    int l=0,r=size-1;
    
    while(l<=r) {
        int m=(l+r)/2;
        int cmp=strcmp(SortedArray[m][column],target);
        if(cmp<0)      
            l=m+1;
        else if(cmp>0) 
            r=m-1;
        else           
            return m;
    }
    return -1;
}

double percentageWithCategoryAndMethod(char SortedArray[][TransactionFields][FieldLength],int  size,const char* category,const char* paymentMethod) {
    
    int idx=binarySearch(SortedArray,size,category,2);
    if(idx<0) return 0.0;
    int total=0, match=0;
    // Scan backwards
    for(int i=idx; i>=0 && strcmp(SortedArray[i][2],category)==0; --i){
        if(strcmp(SortedArray[i][5],paymentMethod)==0) 
            ++match;
            ++total;
    }
    // Scan forwards
    for(int i=idx+1; i<size && strcmp(SortedArray[i][2],category)==0; ++i){
        if(strcmp(SortedArray[i][5],paymentMethod)==0) ++match;
        ++total;
    }

    return total ? (double)match/total*100.0 : 0.0;
}

//binary search reviews
int binarySearchReviews(char SortedReviewArray[][ReviewFields][FieldLength],int  size,const char* target,int  column) {
    int l = 0, r = size - 1;
    while (l <= r) {
        int m =(l + r)/2;
        int cmp=strcmp(SortedReviewArray[m][column],target);
        if (cmp <  0) 
            l = m + 1;
        else if (cmp >  0) 
            r = m - 1;
        else               
            return m;
    }
    return -1;
}





//Linear search
int linearSearch(char** array, int n, const char* target) {
    for (int i = 0; i < n; ++i) {
        if (strcmp(array[i], target) == 0) {
            return i;
        }
    }
    return -1;
}
int linearSearch(char array[][FieldLength], int n, const char* target) {
    for (int i = 0; i < n; ++i) {
        if (strcmp(array[i], target) == 0)
            return i;
    }
    return -1;
}


//#####################################################################################################
//#####################################################################################################
// Hash search for calculating percentage of transactions in a category using a specific payment method

void hashSearchCategoryPayment(char data[][TransactionFields][FieldLength], int size) {
    const char* targetCategory = "Electronics";
    const char* targetPayment = "Credit Card";

    const int HASH_SIZE = 103;  // small prime number
    int hashTable[HASH_SIZE] = {0};
    int creditTable[HASH_SIZE] = {0};

    auto hashFunc = [](const char* str) -> int {
        unsigned long hash = 5381;
        int c;
        while ((c = *str++))
            hash = ((hash << 5) + hash) + c;
        return hash % HASH_SIZE;
    };

    for (int i = 0; i < size; ++i) {
        if (strcmp(data[i][2], targetCategory) == 0) {
            int index = hashFunc(data[i][0]); // hash by customerID just to distribute

            hashTable[index]++;
            if (strcmp(data[i][5], targetPayment) == 0) {
                creditTable[index]++;
            }
        }
    }

    int totalElectronics = 0, totalCreditCard = 0;
    for (int i = 0; i < HASH_SIZE; ++i) {
        totalElectronics += hashTable[i];
        totalCreditCard += creditTable[i];
    }

    double percentage = (totalElectronics == 0) ? 0.0 : (double)totalCreditCard / totalElectronics * 100;

    std::cout << "\n[HASH SEARCH] Electronics purchases: " << totalElectronics
              << ", using Credit Card: " << totalCreditCard
              << " (" << std::fixed << std::setprecision(2) << percentage << "%)\n";
}

// Sorting transactions by date 

// Function to find 1-star reviews
void findOneStarReviews(int& first, int& last, int reviewCount, char reviews[][ReviewFields][FieldLength]) {
    first = -1;
    last = -1;
    for (int i = 0; i < reviewCount; ++i) {
        if (strcmp(reviews[i][2], "1") == 0) {  // Assuming the rating is in column 2 (index 2)
            if (first == -1) first = i;  // Set first 1-star review index
            last = i;  // Set last 1-star review index
        }
    }
}

// Function to count word frequencies in 1-star reviews
void countWordFrequencies(int first, int last, char reviews[][ReviewFields][FieldLength], WordCount* wc, int& unique) {
    int MAX_WORDS = (last - first + 1) * 50;  // Maximum words (adjust as needed)
    char (*words)[FieldLength] = new char[MAX_WORDS][FieldLength];
    int* counts = new int[MAX_WORDS]();
    
    // Process each 1-star review
    for (int i = first; i <= last; ++i) {
        char tmp[FieldLength];
        strcpy(tmp, reviews[i][3]);  // Review text is at index 3

        // Convert to lowercase and remove non-alphabetical characters
        for (int j = 0; tmp[j]; ++j) {
            tmp[j] = isalpha((unsigned char)tmp[j]) ? tolower((unsigned char)tmp[j]) : ' ';
        }

        // Tokenize the review text into words
        for (char* token = strtok(tmp, " "); token; token = strtok(nullptr, " ")) {
            int match = -1;
            for (int w = 0; w < unique; ++w) {
                if (strcmp(words[w], token) == 0) {
                    match = w;
                    break;
                }
            }
            if (match >= 0) {
                counts[match]++;  // Increment count if word found
            } else if (unique < MAX_WORDS) {
                strcpy(words[unique], token);
                counts[unique] = 1;
                ++unique;
            }
        }
    }

    // Store word counts in WordCount structure
    for (int i = 0; i < unique; ++i) {
        strcpy(wc[i].word, words[i]);
        wc[i].count = counts[i];
    }
}

// Function to display top N frequent words (sorted)
void displayTopWords(WordCount* wc, int unique, int N) {
    for (int i = 0; i < unique && i < N; ++i) {
        std::cout << wc[i].word << ": " << wc[i].count << "\n";  // Show top N words
    }
}

//#####################################################################################################
//#####################################################################################################

//Basic jump search algorithm

int jumpSearch(char arr[][TransactionFields][FieldLength], int n, const string& target, int column) {
    int step = sqrt(n);  // Determine block size
    int prev = 0;

    // Jump in blocks to find the block containing the target
    while (strcmp(arr[min(step, n) - 1][column], target.c_str()) < 0) {
        prev = step;
        step += sqrt(n);
        if (prev >= n) return -1;  // Target not found
    }

    // Linear search within the identified block
    for (int i = prev; i < min(step, n); ++i) {
        if (strcmp(arr[i][column], target.c_str()) == 0) {
            return i;  // Return index of the first match
        }
    }

    return -1;  // Not found
}

// Jump search for finding electrnics purchases with credit cards

void findElectronicsAndCreditCardPurchases(char arr[][TransactionFields][FieldLength], int n) {
    const string category = "Electronics";
    const string paymentMethod = "Credit Card";

    // Step 1: Use Jump Search to find first occurrence of "Electronics" in column 2
    int startIndex = jumpSearch(arr, n, category, 2);  // 2 = category column
    if (startIndex == -1) {
        cout << "No Electronics purchases found.\n";
        return;
    }

    // Step 2: Count Electronics purchases and those made with Credit Card
    int electronicsCount = 0;
    int creditCardCount = 0;

    for (int i = startIndex; i < n; ++i) {
        if (strcmp(arr[i][2], category.c_str()) != 0) break;  // Stop if category changes (data is sorted)
        electronicsCount++;

        if (strcmp(arr[i][5], paymentMethod.c_str()) == 0) {
            creditCardCount++;
        }
    }

    // Step 3: Display results
    cout << "\n===== Electronics Purchase Analysis =====\n";
    cout << "Total Electronics Purchases: " << electronicsCount << "\n";
    cout << "Purchases with Credit Card: " << creditCardCount << "\n";

    if (electronicsCount > 0) {
        float percentage = (creditCardCount * 100.0f) / electronicsCount;
        cout << "Percentage using Credit Card: " << percentage << "%\n";
    }
}

#endif