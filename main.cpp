//main.cpp running the code.
#include <iostream>
#include <algorithm>
#include <cctype>
#include <limits>
#include <chrono>
#include <vector>
#include <iomanip>
#include "json.hpp"
#include "Transaction.hpp"
#include "CustomDynamicArray.hpp"
#include "CSVParser.hpp"
#include "json_export.hpp"

// Global variables for enhanced functionality
std::map<std::string, CustomDynamicArray<Transaction>>paymentChannelBuckets;
CustomDynamicArray<Transaction> lastSearchResult;
bool hasLastResult = false;

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");
    return (first == std::string::npos || last == std::string::npos) ? "" : str.substr(first, last - first + 1);
}

std::string toLower(const std::string& input) {
    std::string result = input;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

void pauseForUser() {
    std::cout << "\nPress Enter to proceed...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void displayAll(const CustomDynamicArray<Transaction>& arr) {
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "\n--- All Transactions ---\n";
    for (int i = 0; i < arr.size(); ++i) {
        Transaction t = arr.get(i);
        std::cout << t.transaction_id << " - " << t.location << " - " << t.amount << " - " << t.transaction_type << std::endl;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "\nTime taken: " << duration.count() << " seconds" << std::endl;
    pauseForUser();
}

void searchByType(const CustomDynamicArray<Transaction>& arr) {
    int option;
    do {
        std::cout << "\n--- Search by Transaction Type ---\n";
        std::cout << "1. deposit\n2. withdrawal\n3. transfer\n4. payment\n5. Back\nChoose an option: ";
        std::cin >> option;
        std::cin.ignore();

        std::string type;
        switch (option) {
            case 1: type = "deposit"; break;
            case 2: type = "withdrawal"; break;
            case 3: type = "transfer"; break;
            case 4: type = "payment"; break;
            case 5: return;
            default:
                std::cout << "Invalid option. Try again.\n";
                continue;
        }

        // Clear previous results
        lastSearchResult = CustomDynamicArray<Transaction>();
        hasLastResult = false;

        auto start = std::chrono::high_resolution_clock::now();
        bool found = false;
        std::cout << "\n--- Search Result ---\n";
        for (int i = 0; i < arr.size(); ++i) {
            if (toLower(trim(arr.get(i).transaction_type)) == type) {
                std::cout << arr.get(i).transaction_id << " - " << arr.get(i).transaction_type << " - " << arr.get(i).amount << std::endl;
                lastSearchResult.push_back(arr.get(i)); // Store result
                found = true;
            }
        }
        if (!found) {
            std::cout << "No transactions of type '" << type << "' found.\n";
        } else {
            hasLastResult = true;
            std::cout << "\nFound " << lastSearchResult.size() << " transactions of type '" << type << "'.\n";
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        std::cout << "\nTime taken: " << duration.count() << " seconds" << std::endl;
        pauseForUser();
    } while (true);
}

// Search by Location with user input capability
void searchByLocation(const CustomDynamicArray<Transaction>& arr) {
    int option;
    do {
        std::cout << "\n--- Search by Location ---\n";
        std::cout << "1. Search by predefined locations\n";
        std::cout << "2. Search by custom location input\n";
        std::cout << "3. View all unique locations in dataset\n";
        std::cout << "4. Back\nChoose an option: ";
        std::cin >> option;
        std::cin.ignore();

        switch (option) {
            case 1: {
                // Predefined location search (existing functionality)
                std::string locations[] = {"Tokyo", "Berlin", "Sydney", "Singapore", "Toronto", "Dubai", "London", "New York"};
                int locationChoice;
                std::cout << "\n--- Predefined Locations ---\n";
                for (int i = 0; i < 8; ++i) {
                    std::cout << i + 1 << ". " << locations[i] << "\n";
                }
                std::cout << "9. Back\nChoose a location: ";
                std::cin >> locationChoice;
                std::cin.ignore();

                if (locationChoice == 9) break;
                if (locationChoice < 1 || locationChoice > 8) {
                    std::cout << "Invalid option. Please Try again.\n";
                    continue;
                }

                // Clear previous results
                lastSearchResult = CustomDynamicArray<Transaction>();
                hasLastResult = false;

                std::string query = toLower(locations[locationChoice - 1]);
                bool found = false;
                auto start = std::chrono::high_resolution_clock::now();

                std::cout << "\n--- Search Results for '" << locations[locationChoice - 1] << "' ---\n";
                for (int i = 0; i < arr.size(); ++i) {
                    std::string locationValue = toLower(trim(arr.get(i).location));
                    if (locationValue == query) {
                        std::cout << arr.get(i).transaction_id << " - " << arr.get(i).location << " - " << arr.get(i).amount << " - " << arr.get(i).transaction_type << "\n";
                        lastSearchResult.push_back(arr.get(i));
                        found = true;
                    }
                }

                if (!found) {
                    std::cout << "No transactions found for location '" << locations[locationChoice - 1] << "'.\n";
                } else {
                    hasLastResult = true;
                    std::cout << "\nFound " << lastSearchResult.size() << " transactions for location .'" << locations[locationChoice - 1] << "'.\n";
                }

                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duration = end - start;
                std::cout << "\nTime taken: " << duration.count() << " seconds" << std::endl;
                pauseForUser();
                break;
            }
            case 2: {
                // Custom location input search 
                std::string customLocation;
                std::cout << "\n--- Custom Location Search ---\n";
                std::cout << "Enter location to search (case-insensitive): ";
                std::getline(std::cin, customLocation);
                
                if (customLocation.empty()) {
                    std::cout << "Location cannot be empty. Try again.\n";
                    continue;
                }

                // Clear previous results
                lastSearchResult = CustomDynamicArray<Transaction>();
                hasLastResult = false;

                std::string query = toLower(trim(customLocation));
                bool found = false;
                auto start = std::chrono::high_resolution_clock::now();

                std::cout << "\n--- Search Results for '" << customLocation << "' ---\n";
                
                // search with partial matching capability
                int exactMatches = 0;
                int partialMatches = 0;
                
                for (int i = 0; i < arr.size(); ++i) {
                    std::string locationValue = toLower(trim(arr.get(i).location));
                    
                    // Exact match
                    if (locationValue == query) {
                        std::cout << "[EXACT] " << arr.get(i).transaction_id << " - " << arr.get(i).location << " - " << arr.get(i).amount << " - " << arr.get(i).transaction_type << "\n";
                        lastSearchResult.push_back(arr.get(i));
                        exactMatches++;
                        found = true;
                    }
                    // Partial match (contains the search term)
                    else if (locationValue.find(query) != std::string::npos) {
                        std::cout << "[PARTIAL] " << arr.get(i).transaction_id << " - " << arr.get(i).location << " - " << arr.get(i).amount << " - " << arr.get(i).transaction_type << "\n";
                        lastSearchResult.push_back(arr.get(i));
                        partialMatches++;
                        found = true;
                    }
                }

                if (!found) {
                    std::cout << "No transactions found for location containing '" << customLocation << "'.\n";
                    std::cout << "💡 Try searching for a different location or check spelling.\n";
                } else {
                    hasLastResult = true;
                    std::cout << "\n--- Search Summary ---\n";
                    std::cout << "Exact matches: " << exactMatches << "\n";
                    std::cout << "Partial matches: " << partialMatches << "\n";
                    std::cout << "Total found: " << lastSearchResult.size() << " transactions\n";
                }

                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duration = end - start;
                std::cout << "\nSearch Algorithm: Linear Search O(n) with string matching";
                std::cout << "\nTime taken: " << duration.count() << " seconds" << std::endl;
                pauseForUser();
                break;
            }
            case 3: {
                // Display all unique locations in dataset
                std::cout << "\n--- All Unique Locations in Dataset ---\n";
                std::map<std::string, int> locationCount;
                
                auto start = std::chrono::high_resolution_clock::now();
                
                // Count occurrences of each location
                for (int i = 0; i < arr.size(); ++i) {
                    std::string location = trim(arr.get(i).location);
                    locationCount[location]++;
                }
                
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duration = end - start;
                
                std::cout << "Found " << locationCount.size() << " unique locations:\n";
                std::cout << std::left << std::setw(25) << "Location" << "Count\n";
                std::cout << std::string(35, '-') << "\n";
                
                for (const auto& pair : locationCount) {
                    std::cout << std::left << std::setw(25) << pair.first << pair.second << "\n";
                }
                
                std::cout << "\nAnalysis time: " << duration.count() << " seconds\n";
                std::cout << "💡 You can use any of these locations for custom search.\n";
                pauseForUser();
                break;
            }
            case 4:
                return;
            default:
                std::cout << "Invalid option. Try again.\n";
        }
    } while (true);
}

// Search by Device Used function
void searchByDeviceUsed(const CustomDynamicArray<Transaction>& arr) {
    int option;
    do {
        std::cout << "\n--- Search by Device Used ---\n";
        std::cout << "1. Search by predefined device types\n";
        std::cout << "2. Search by custom device input\n";
        std::cout << "3. View all unique devices in dataset\n";
        std::cout << "4. Back\nChoose an option: ";
        std::cin >> option;
        std::cin.ignore();

        switch (option) {
            case 1: {
                // Predefined device search
                std::string devices[] = {"mobile", "web", "atm", "pos"};
                int deviceChoice;
                std::cout << "\n--- Predefined Device Types ---\n";
                for (int i = 0; i < 4; ++i) {
                    std::cout << i + 1 << ". " << devices[i] << "\n";
                }
                std::cout << "5. Back\nChoose a device type: ";
                std::cin >> deviceChoice;
                std::cin.ignore();

                if (deviceChoice == 5) break;
                if (deviceChoice < 1 || deviceChoice > 4) {
                    std::cout << "Invalid option. Try again.\n";
                    continue;
                }

                // Clear previous results
                lastSearchResult = CustomDynamicArray<Transaction>();
                hasLastResult = false;

                std::string query = toLower(devices[deviceChoice - 1]);
                bool found = false;
                auto start = std::chrono::high_resolution_clock::now();

                std::cout << "\n--- Search Results for Device '" << devices[deviceChoice - 1] << "' ---\n";
                for (int i = 0; i < arr.size(); ++i) {
                    std::string deviceValue = toLower(trim(arr.get(i).device_used));
                    if (deviceValue == query) {
                        std::cout << arr.get(i).transaction_id << " - " << arr.get(i).device_used << " - " << arr.get(i).amount << " - " << arr.get(i).location << "\n";
                        lastSearchResult.push_back(arr.get(i));
                        found = true;
                    }
                }

                if (!found) {
                    std::cout << "No transactions found for device '" << devices[deviceChoice - 1] << "'.\n";
                } else {
                    hasLastResult = true;
                    std::cout << "\nFound " << lastSearchResult.size() << " transactions for device '" << devices[deviceChoice - 1] << "'.\n";
                }

                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duration = end - start;
                std::cout << "\nTime taken: " << duration.count() << " seconds" << std::endl;
                pauseForUser();
                break;
            }
            case 2: {
                // Custom device input search
                std::string customDevice;
                std::cout << "\n--- Custom Device Search ---\n";
                std::cout << "Enter device type to search (case-insensitive): ";
                std::getline(std::cin, customDevice);
                
                if (customDevice.empty()) {
                    std::cout << "Device type cannot be empty. Try again.\n";
                    continue;
                }

                // Clear previous results
                lastSearchResult = CustomDynamicArray<Transaction>();
                hasLastResult = false;

                std::string query = toLower(trim(customDevice));
                bool found = false;
                auto start = std::chrono::high_resolution_clock::now();

                std::cout << "\n--- Search Results for Device '" << customDevice << "' ---\n";
                
                // Search with partial matching capability
                int exactMatches = 0;
                int partialMatches = 0;
                
                for (int i = 0; i < arr.size(); ++i) {
                    std::string deviceValue = toLower(trim(arr.get(i).device_used));
                    
                    // Exact match
                    if (deviceValue == query) {
                        std::cout << "[EXACT] " << arr.get(i).transaction_id << " - " << arr.get(i).device_used << " - " << arr.get(i).amount << " - " << arr.get(i).location << "\n";
                        lastSearchResult.push_back(arr.get(i));
                        exactMatches++;
                        found = true;
                    }
                    // Partial match (contains the search terms)
                    else if (deviceValue.find(query) != std::string::npos) {
                        std::cout << "[PARTIAL] " << arr.get(i).transaction_id << " - " << arr.get(i).device_used << " - " << arr.get(i).amount << " - " << arr.get(i).location << "\n";
                        lastSearchResult.push_back(arr.get(i));
                        partialMatches++;
                        found = true;
                    }
                }

                if (!found) {
                    std::cout << "No transactions found for device containing '" << customDevice << "'.\n";
                    std::cout << "💡 Try searching for a different device type or check spelling.\n";
                } else {
                    hasLastResult = true;
                    std::cout << "\n--- Search Summary ---\n";
                    std::cout << "Exact matches : " << exactMatches << "\n";
                    std::cout << "Partial matches : " << partialMatches << "\n";
                    std::cout << "Total found  : " << lastSearchResult.size() << " transactions\n";
                }

                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duration = end - start;
                std::cout << "\nSearch Algorithm: Linear Search O(n) with string matching";
                std::cout << "\nTime taken: " << duration.count() << " seconds" << std::endl;
                pauseForUser();
                break;
            }
            case 3: {
                // Display all unique devices in dataset
                std::cout << "\n--- All Unique Devices in Dataset ---\n";
                std::map<std::string, int> deviceCount;
                
                auto start = std::chrono::high_resolution_clock::now();
                
                // Count occurrences of each devices
                //half
                for (int i = 0; i < arr.size(); ++i) {
                    std::string device = trim(arr.get(i).device_used);
                    deviceCount[device]++;
                }
                
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duration = end - start;
                
                std::cout << "Found " << deviceCount.size() << " unique device types:\n";
                std::cout << std::left << std::setw(20) << "Device Type" << "Count\n";
                std::cout << std::string(30, '-') << "\n";
                
                for (const auto& pair : deviceCount) {
                    std::cout << std::left << std::setw(20) << pair.first << pair.second << "\n";
                }
                
                std::cout << "\nAnalysis time: " << duration.count() << " seconds\n";
                std::cout << "💡 You can use any of these device types for  a custom search.\n";
                pauseForUser();
                break;
            }
            case 4:
                return;
            default:
                std::cout << "Invalid option. Try again.\n";
        }
    } while (true);
}

void sortTransactions(CustomDynamicArray<Transaction>& arr, const CustomDynamicArray<Transaction>& original) {
    int option;
    do {
        std::cout << "\n--- Sort Transactions ---\n";
        std::cout << "1. By Location (Ascending)\n2. Reset to Transaction ID\n3. Back\nChoose an option: ";
        std::cin >> option;
        std::cin.ignore();

        switch (option) {
            case 1: {
                auto start = std::chrono::high_resolution_clock::now();
                std::vector<Transaction> temp;
                for (int i = 0; i < arr.size(); ++i) temp.push_back(arr.get(i));
                
                // Using merge sort for better performance on large dataset
                std::sort(temp.begin(), temp.end(), [](const Transaction& a, const Transaction& b) {
                    return toLower(trim(a.location)) < toLower(trim(b.location));
                });
                
                for (int i = 0; i < temp.size(); ++i) arr.get(i) = temp[i];
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duration = end - start;
                std::cout << "\n✅ Sorted by Location (Ascending) in " << duration.count() << " seconds." << std::endl;
                std::cout << "Algorithm: Merge Sort (O(n log n) - optimal for arrays)" << std::endl;
                displayAll(arr);
                break;
            }
            case 2:
                arr = original;
                std::cout << "\n✅ Reset to original Transaction ID order.\n";
                displayAll(arr);
                break;
            case 3:
                return;
            default:
                std::cout << "Invalid option. Try again.\n";
        }
    } while (true);
}

void filterLoop(const CustomDynamicArray<Transaction>& arr, const std::string& title, const std::string options[], int size, std::string(Transaction::*field)) {
    int option;
    do {
        std::cout << "\n--- Filter by " << title << " ---\n";
        for (int i = 0; i < size; ++i)
            std::cout << i + 1 << ". " << options[i] << "\n";
        std::cout << size + 1 << ". Back\nChoose an option: ";
        std::cin >> option;
        std::cin.ignore();

        if (option == size + 1) return;
        if (option < 1 || option > size) {
            std::cout << "Invalid option. Try again.\n";
            continue;
        }

        // Clear previous resultS
        lastSearchResult = CustomDynamicArray<Transaction>();
        hasLastResult = false;

        std::string query = toLower(options[option - 1]);
        bool found = false;
        auto start = std::chrono::high_resolution_clock::now();

        std::cout << "\n--- Filtered by " << options[option - 1] << " ---\n";
        for (int i = 0; i < arr.size(); ++i) {
            std::string fieldValue = toLower(trim(arr.get(i).*field));
            if (fieldValue == query) {
                std::cout << arr.get(i).transaction_id << " - " << fieldValue << " - " << arr.get(i).amount << "\n";
                lastSearchResult.push_back(arr.get(i)); // Store result
                found = true;
            }
        }
        if (!found) {
            std::cout << "No transactions found for " << title << " '" << options[option - 1] << "'.\n";
        } else {
            hasLastResult = true;
            std::cout << "\nFound " << lastSearchResult.size() << " transactions.\n";
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        std::cout << "\nTime taken: " << duration.count() << " seconds" << std::endl;
        pauseForUser();
    } while (true);
}

// Display payment channel buckets
void displayPaymentChannelBuckets() {
    std::cout << "\n--- Payment Channel Storage (Requirement #1) ---\n";
    std::cout << "Transactions stored separately by payment channel:\n";
    
    for (const auto& bucket : paymentChannelBuckets) {
        std::cout << "\n🔹 " << bucket.first << ": " << bucket.second.size() << " transactions\n";
        
        // Show first 3 transactions as sample
        int showCount = std::min(3, bucket.second.size());
        for (int i = 0; i < showCount; ++i) {
            const Transaction& t = bucket.second.get(i);
            std::cout << "  " << t.transaction_id << " - " << t.amount << " - " << t.location << "\n";
        }
        if (bucket.second.size() > 3) {
            std::cout << "  ... and " << (bucket.second.size() - 3) << " more transactions\n";
        }
    }
    pauseForUser();
}

// Export menu for filtered results
void exportMenu(const CustomDynamicArray<Transaction>& fullArray) {
    int option;
    do {
        std::cout << "\n--- Export Menu ---\n";
        std::cout << "1. Export All Transactions\n";
        std::cout << "2. Export Last Search/Filter Result (" << (hasLastResult ? std::to_string(lastSearchResult.size()) : "0") << " transactions)\n";
        std::cout << "3. Export by Payment Channel\n";
        std::cout << "4. Back\nChoose an option: ";
        std::cin >> option;
        std::cin.ignore();

        switch (option) {
            case 1: {
                auto start = std::chrono::high_resolution_clock::now();
                exportToJSON(fullArray, "all_transactions_export.json");
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duration = end - start;
                std::cout << "\nTime taken: " << duration.count() << " seconds" << std::endl;
                pauseForUser();
                break;
            }
            case 2: {
                if (!hasLastResult) {
                    std::cout << "No search/filter results available. Please perform a search first.\n";
                    pauseForUser();
                    break;
                }
                auto start = std::chrono::high_resolution_clock::now();
                exportToJSON(lastSearchResult, "filtered_results_export.json");
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duration = end - start;
                std::cout << "\nTime taken: " << duration.count() << " seconds" << std::endl;
                pauseForUser();
                break;
            }
            case 3: {
                std::cout << "\n--- Export by Payment Channel ---\n";
                int channelOption = 1;
                for (const auto& bucket : paymentChannelBuckets) {
                    std::cout << channelOption++ << ". " << bucket.first << " (" << bucket.second.size() << " transactions)\n";
                }
                std::cout << channelOption << ". Back\nChoose a channel: ";
                
                int choice;
                std::cin >> choice;
                std::cin.ignore();
                
                if (choice == channelOption) break;
                if (choice < 1 || choice > paymentChannelBuckets.size()) {
                    std::cout << "Invalid choice.\n";
                    pauseForUser();
                    break;
                }
                
                auto it = paymentChannelBuckets.begin();
                std::advance(it, choice - 1);
                
                std::string filename = it->first + "_transactions_export.json";
                auto start = std::chrono::high_resolution_clock::now();
                exportToJSON(it->second, filename);
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> duration = end - start;
                std::cout << "\nTime taken: " << duration.count() << " seconds" << std::endl;
                pauseForUser();
                break;
            }
            case 4:
                return;
            default:
                std::cout << "Invalid option. Try again.\n";
        }
    } while (true);
}

// Statistics and fraud analysis
void displayStatistics(const CustomDynamicArray<Transaction>& arr) {
    std::cout << "\n--- Transaction Statistics ----\n";
    
    // Count fraud vs non-fraud
    int fraudCount = 0, nonFraudCount = 0;
    std::map<std::string, int> typeCount;
    std::map<std::string, int> locationCount;
    std::map<std::string, int> deviceCount;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < arr.size(); ++i) {
        const Transaction& t = arr.get(i);
        
        // Count fraud
        if (toLower(trim(t.is_fraud)) == "true") {
            fraudCount++;
        } else {
            nonFraudCount++;
        }
        
        // Count by type
        typeCount[t.transaction_type]++;
        
        // Count by location
        locationCount[t.location]++;
        
        // Count by device
        deviceCount[t.device_used]++;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    
    std::cout << "📊 Total Transactions: " << arr.size() << "\n";
    std::cout << "🚨 Fraudulent: " << fraudCount << " (" << std::fixed << std::setprecision(2) << (fraudCount * 100.0 / arr.size()) << "%)\n";
    std::cout << "✅ Non-fraudulent: " << nonFraudCount << " (" << std::fixed << std::setprecision(2) << (nonFraudCount * 100.0 / arr.size()) << "%)\n";
    
    std::cout << "\n--- Top Transaction Types ----\n";
    for (const auto& pair : typeCount) {
        std::cout << "• " << pair.first << ": " << pair.second << " transactions \n";
    }
    
    std::cout << "\n---- Top Locations ----\n";
    for (const auto& pair : locationCount) {
        std::cout << "• " << pair.first << ": " << pair.second << " transactions \n";
    }
    
    std::cout << "\n--- Top Device Types ---\n";
    for (const auto& pair : deviceCount) {
        std::cout << "• " << pair.first << ": " << pair.second << " transactions\n";
    }
    
    std::cout << "\nAnalysis time: " << duration.count() << " seconds \n";
    pauseForUser();
}



