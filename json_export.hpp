// json_export.hpp
#ifndef JSON_EXPORT_HPP
#define JSON_EXPORT_HPP

#include "Transaction.hpp"
#include "CustomDynamicArray.hpp"
#include <fstream>
#include "json.hpp"


using json = nlohmann::json;

void exportToJSON(const CustomDynamicArray<Transaction>& arr, const std::string& filename) {
    json j_array = json::array();

    for (int i = 0; i < arr.size(); ++i) {
        const Transaction& t = arr.get(i);
        json j = {
            {"transaction_id", t.transaction_id},
            {"timestamp", t.timestamp},
            {"sender_account", t.sender_account},
            {"receiver_account", t.receiver_account},
            {"amount", t.amount},
            {"transaction_type", t.transaction_type},
            {"merchant_category", t.merchant_category},
            {"location", t.location},
            {"device_used", t.device_used},
            {"is_fraud", t.is_fraud},
            {"fraud_type", t.fraud_type},
            {"time_since_last_transaction", t.time_since_last_transaction},
            {"spending_deviation_score", t.spending_deviation_score},
            {"velocity_score", t.velocity_score},
            {"geo_anomaly_score", t.geo_anomaly_score},
            {"payment_channel", t.payment_channel},
            {"ip_address", t.ip_address},
            {"device_hash", t.device_hash}
        };
        j_array.push_back(j);
    }

    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Failed to open file for writing JSON: " << filename << "\n";
        return;
    }
    outFile << std::setw(4) << j_array << std::endl;
    std::cout << "\n\u2705 Exported " << arr.size() << " transactions to " << filename << "\n";
}

#endif
