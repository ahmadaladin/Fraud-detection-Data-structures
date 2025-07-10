#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include <string>

class Transaction {
public:
    std::string transaction_id;
    std::string timestamp;
    std::string sender_account;
    std::string receiver_account;
    std::string amount;
    std::string transaction_type;
    std::string merchant_category;
    std::string location;
    std::string device_used;
    std::string is_fraud;
    std::string fraud_type;
    std::string time_since_last_transaction;
    std::string spending_deviation_score;
    std::string velocity_score;
    std::string geo_anomaly_score;
    std::string payment_channel;
    std::string ip_address;
    std::string device_hash;

    Transaction() = default;

    Transaction(const std::string& transaction_id,
                const std::string& timestamp,
                const std::string& sender_account,
                const std::string& receiver_account,
                const std::string& amount,
                const std::string& transaction_type,
                const std::string& merchant_category,
                const std::string& location,
                const std::string& device_used,
                const std::string& is_fraud,
                const std::string& fraud_type,
                const std::string& time_since_last_transaction,
                const std::string& spending_deviation_score,
                const std::string& velocity_score,
                const std::string& geo_anomaly_score,
                const std::string& payment_channel,
                const std::string& ip_address,
                const std::string& device_hash)
        : transaction_id(transaction_id), timestamp(timestamp), sender_account(sender_account),
          receiver_account(receiver_account), amount(amount), transaction_type(transaction_type),
          merchant_category(merchant_category), location(location), device_used(device_used),
          is_fraud(is_fraud), fraud_type(fraud_type), time_since_last_transaction(time_since_last_transaction),
          spending_deviation_score(spending_deviation_score), velocity_score(velocity_score),
          geo_anomaly_score(geo_anomaly_score), payment_channel(payment_channel),
          ip_address(ip_address), device_hash(device_hash) {}
};

#endif