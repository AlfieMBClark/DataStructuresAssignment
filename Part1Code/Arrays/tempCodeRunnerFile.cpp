 for (int i = 1; i < transactionCount; ++i) {
        if (strcmp(transactions[i][4], currentDate) == 0) {
            ++dailyCount;