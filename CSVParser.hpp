#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <map>
#include "Transaction.hpp"
#include "CustomDynamicArray.hpp"

extern std::map<std::string, CustomDynamicArray<Transaction>> paymentChannelBuckets;

bool isValidTransaction(const Transaction& t) {
    return !t.transaction_id.empty() && !t.amount.empty() && !t.transaction_type.empty();
}

// Enhanced CSV loading with row limit for testing
void loadFromCSV(const std::string& filename,
                 CustomDynamicArray<Transaction>& array) {
    std::ifstream file(filename);
    std::string line;
    int lineNumber = 0, validCount = 0, errorCount = 0;

    if (!file.is_open()) {
        std::cerr << "[ERROR] Failed to open file: " << filename << "\n";
        return;
    }

    std::getline(file, line);  // Skip header line

    while (std::getline(file, line)) {
        lineNumber++;

        std::stringstream ss(line);
        std::string transaction_id, timestamp, sender_account, receiver_account,
                    amount, transaction_type, merchant_category, location,
                    device_used, is_fraud, fraud_type, time_since_last_transaction,
                    spending_deviation_score, velocity_score, geo_anomaly_score,
                    payment_channel, ip_address, device_hash;

        std::getline(ss, transaction_id, ',');
        std::getline(ss, timestamp, ',');
        std::getline(ss, sender_account, ',');
        std::getline(ss, receiver_account, ',');
        std::getline(ss, amount, ',');
        std::getline(ss, transaction_type, ',');
        std::getline(ss, merchant_category, ',');
        std::getline(ss, location, ',');
        std::getline(ss, device_used, ',');
        std::getline(ss, is_fraud, ',');
        std::getline(ss, fraud_type, ',');
        std::getline(ss, time_since_last_transaction, ',');
        std::getline(ss, spending_deviation_score, ',');
        std::getline(ss, velocity_score, ',');
        std::getline(ss, geo_anomaly_score, ',');
        std::getline(ss, payment_channel, ',');
        std::getline(ss, ip_address, ',');
        std::getline(ss, device_hash, ',');

        Transaction t(transaction_id, timestamp, sender_account, receiver_account,
                      amount, transaction_type, merchant_category, location,
                      device_used, is_fraud, fraud_type, time_since_last_transaction,
                      spending_deviation_score, velocity_score, geo_anomaly_score,
                      payment_channel, ip_address, device_hash);

        if (isValidTransaction(t)) {
            array.push_back(t);
            paymentChannelBuckets[t.payment_channel].push_back(t); // Store separately
            validCount++;
        } else {
            errorCount++;
            if (errorCount <= 10)
                std::cerr << "[Line " << lineNumber << "] Invalid or missing data: " << line << "\n";
        }

        if (lineNumber % 100 == 0)
            std::cout << "Loaded lines: " << lineNumber << "\n";
    }

    std::cout << "\n✅ Finished loading.\n";
    std::cout << "Valid Transactions : " << validCount << "\n";
    std::cout << "Invalid Lines      : " << errorCount << "\n";
    std::cout << "Total Lines Read   : " << lineNumber << "\n";

    std::cout << "\n✅ Records per Payment Channel:\n";
    for (const auto& pair : paymentChannelBuckets) {
        std::cout << " - " << pair.first << ": " << pair.second.size() << " transactions\n";
    }
}

#endif