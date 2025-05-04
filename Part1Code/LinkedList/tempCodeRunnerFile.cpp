TimingResults Stanlie() {
    cout << "\nInsertion Sort & Linear Search\n";

    Loader loader("cleaned_transactions.csv", "cleaned_reviews.csv");
    TimingResults results = {0, 0, 0, 0, 0, 0, 0, 0};  // Initialize to 0

    if (!loader.loadAll()) {
        cout << "Failed to load data.\n";
        return results;
    }

    TransactionNode* originalHead = loader.getTransHead();
    TransactionNode* copyForQ1 = DeepCopyTransactionList(originalHead);


// Question 1: Sort transactions by date and count
cout << "\n\nQuestion 1: Sort transactions by date and total number of transactions\n";
auto StanlieStartQ1 = Clock::now();
TransactionNode* sortedHead = InsertionSortByDate(copyForQ1);
auto StanlieEndQ1 = Clock::now();
cout << "Transactions by Date:\n";
TransactionNode* current = sortedHead;
if (current) {
    string currentDate = current->data.date;
    int dailyCount = 1;
    current = current->next;

    while (current) {
        if (current->data.date == currentDate) {
            dailyCount++;
        } else {
            cout << currentDate << ": " << dailyCount << " transaction\n";
            currentDate = current->data.date;
            dailyCount = 1;
        }
        current = current->next;
    }
    cout << currentDate << ": " << dailyCount << " transaction\n";
}

results.Q1_SortTime = chrono::duration_cast<chrono::milliseconds>(StanlieEndQ1 - StanlieStartQ1).count();
results.Q1_Full = results.Q1_SortTime;

cout << "Insertion Sort Time: " << results.Q1_SortTime << " ms\n";


// Question 2: Electronics & Credit Card Analysis
cout << "\n\nQuestion 2: Electronics and Credit Card Analysis\n";
auto StanlieStartQ2 = Clock::now();

// Copy original list to avoid affecting other questions
TransactionNode* copyForQ2 = DeepCopyTransactionList(originalHead);

// Sort the copy by category (or any relevant field)
auto StanlieQ2_SortStart = Clock::now();
TransactionNode* sortedQ2Head = InsertionSortByCategory(copyForQ2);  // You implement this
auto StanlieQ2_SortEnd = Clock::now();

// Perform linear search on the sorted list
int totalElectronics = 0, creditCardCount = 0;
auto StanlieQ2_SearchStart = Clock::now();
LinearSearchElectronics(sortedQ2Head, totalElectronics, creditCardCount);
auto StanlieQ2_SearchEnd = Clock::now();

// Timing results
results.Q2_SortTime = chrono::duration_cast<chrono::milliseconds>(StanlieQ2_SortEnd - StanlieQ2_SortStart).count();
results.Q2_SearchTime = chrono::duration_cast<chrono::nanoseconds>(StanlieQ2_SearchEnd - StanlieQ2_SearchStart).count();
results.Q2_Full = chrono::duration_cast<chrono::milliseconds>(StanlieQ2_SearchEnd - StanlieStartQ2).count();

// Output
cout << "Total Electronics purchases: " << totalElectronics << endl;
cout << "Paid with Credit Card: " << creditCardCount << endl;

if (totalElectronics > 0) {
    double percentage = (static_cast<double>(creditCardCount) / totalElectronics) * 100.0;
    cout << "Percentage: " << fixed << setprecision(2) << percentage << "%\n";
} else {
    cout << "Percentage: 0.00%\n";
}

cout << "Insertion Sort Time: " << results.Q2_SortTime << " ms\n";
cout << "Linear Search Time: " << results.Q2_SearchTime << " ns\n";

// Question 3: Most frequent words in 1-star reviews
cout << "\n\nQuestion 3: Words frequently used in product reviews rated 1-star\n";
auto StanlieStartQ3 = Clock::now();

FrequentWord* freqHead = nullptr;

auto StanlieQ3_SearchStart = Clock::now();
LinearSearch(loader.getRevHead(), freqHead);
auto StanlieQ3_SearchEnd = Clock::now();

auto StanlieQ3_SortStart = Clock::now();
InsertionSortWordFreqDescending(freqHead);
auto StanlieQ3_SortEnd = Clock::now();

results.Q3_SearchTime = chrono::duration_cast<chrono::milliseconds>(StanlieQ3_SearchEnd - StanlieQ3_SearchStart).count();
results.Q3_SortTime   = chrono::duration_cast<chrono::nanoseconds>(StanlieQ3_SortEnd - StanlieQ3_SortStart).count();

cout << "Top Words in 1-Star Reviews:\n";
FrequentWord* freqCurrent = freqHead;
while (freqCurrent) {
    cout << left << setw(15) << freqCurrent->word << ": " << freqCurrent->count << endl;
    freqCurrent = freqCurrent->next;
}

cout << "Linear Search Time: " << results.Q3_SearchTime << " ms\n";
cout << "Insertion Sort Time: " << results.Q3_SortTime << " ns\n";

auto StanlieEndQ3 = Clock::now();
results.Q3_Full = chrono::duration_cast<chrono::milliseconds>(StanlieEndQ3 - StanlieStartQ3).count();

// Clean up
while (freqHead) {
    FrequentWord* temp = freqHead;
    freqHead = freqHead->next;
    delete temp;
}

// ────────────────────────────────────────────────
// Final Summary for Stanlie's Results
// ────────────────────────────────────────────────
cout << "\nSummary of all times:\n";
cout << "Q1 : " << results.Q1_Full << " ms\n";
cout << "Q2 : "  << results.Q2_Full << " ms\n";
cout << "Q3 : " << results.Q3_Full << " ms\n";

return results;
}