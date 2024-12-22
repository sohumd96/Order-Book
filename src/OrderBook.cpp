// This is an order book class

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>
#include <stdexcept>
#include <algorithm>

#include "../include/OrderBook.h"

using std::cout;
using std::cerr;
using std::endl;

using std::string;
using std::vector;
using std::map;
using std::ifstream;


void OrderBook::enterOrder(const string& side, int quantity) {
    cout << "Submitting " << side << " market order for " << quantity << " units..." << endl;
    auto now = std::chrono::system_clock::now();
    std::time_t timestamp = std::chrono::system_clock::to_time_t(now);
    auto start = std::chrono::high_resolution_clock::now();
    int originalQuantity = quantity;
    double totalCost = 0;

    if (side == "SELL") {
        for (auto it = buyOrders.rbegin(); it != buyOrders.rend(); ++it) {
            double price = it->first;
            vector<Order>& orders = it->second;
            vector<Order>::iterator orderIter = orders.begin();
            while(orderIter != orders.end()) {
                Order& order = *orderIter;
                if (order.quantity <= quantity) {
                    cout << "Matched order at price: " << price << " for " << order.quantity << " units" << endl;
                    quantity -= order.quantity;
                    totalCost += order.quantity * price;
                    order.quantity = 0;
                    orderIter = orders.erase(orderIter);
                } else {
                    cout << "Matched order at price: " << price << " for " << quantity << " units" << endl;
                    order.quantity -= quantity;
                    totalCost += quantity * price;
                    quantity = 0;
                    break;
                }
            }
            if (quantity == 0) {
                break;
            }
            
        }

        // Iterate through the map and erase entries with empty vectors
        for (auto it = buyOrders.begin(); it != buyOrders.end();) {
            if (it->second.empty()) {
                // Erase the current element and move the iterator to the next
                it = buyOrders.erase(it);  // erase returns the next iterator
            } else {
                // Move to the next element
                ++it;
            }
        }

        double avgPrice = totalCost/(originalQuantity - quantity);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        std::cout << "Sold " << (originalQuantity - quantity) << "/" << originalQuantity << " units @ $" << std::fixed << std::setprecision(2) << avgPrice <<" average price. Time taken: " << duration << " nanoseconds." << std::endl;

    } else if (side == "BUY") {
        for (auto it = sellOrders.begin(); it != sellOrders.end(); ++it) {
            double price = it->first;
            vector<Order>& orders = it->second;
            vector<Order>::iterator orderIter = orders.begin();
            while(orderIter != orders.end()) {
                Order& order = *orderIter;
                if (order.quantity <= quantity) {
                    cout << "Matched order at price: " << price << " for " << order.quantity << " units" << endl;
                    quantity -= order.quantity;
                    totalCost += order.quantity * price;
                    order.quantity = 0;
                    orderIter = orders.erase(orderIter);
                } else {
                    cout << "Matched order at price: " << price << " for " << quantity << " units" << endl;
                    order.quantity -= quantity;
                    totalCost += quantity * price;
                    quantity = 0;
                    break;
                }
            }
            if (quantity == 0) {
                break;
            }
            
        }

        // Iterate through the map and erase entries with empty vectors
        for (auto it = sellOrders.begin(); it != sellOrders.end();) {
            if (it->second.empty()) {
                // Erase the current element and move the iterator to the next
                it = sellOrders.erase(it);  // erase returns the next iterator
            } else {
                // Move to the next element
                ++it;
            }
        }

        double avgPrice = totalCost/(originalQuantity - quantity);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        std::cout << "Filled " << (originalQuantity - quantity) << "/" << originalQuantity << " units @ $" << std::fixed << std::setprecision(2) << avgPrice <<" average price. Time taken: " << duration << " nanoseconds." << std::endl;

    } else {
        throw std::invalid_argument("Invalid trade action.");
    }
    

    //cout << "Order entered successfully" << endl;
}

double OrderBook::calculateBPS() {
    double minSellPrice = sellOrders.begin()->first; 
    double maxBuyPrice = buyOrders.rbegin()->first;
    double bps = (minSellPrice - maxBuyPrice)/maxBuyPrice * 10000;
    return bps;
}


void OrderBook::printOrders() {
    //cout << "Printing all orders" << endl;

    

    cout << "\033[1;31m\nSELL ORDERS\n\033[0m" << endl;
    
    for (auto it = sellOrders.rbegin(); it != sellOrders.rend(); ++it) {
        // cout << "Price: " << sellOrder.first << endl;
        int currentSellQuantity = 0;
        for (auto& order : it->second) {
            //cout << "Timestamp: " << std::time_t(&order.timestamp) << "Price: " << order.price << "Quantity: " << order.quantity << "Side: " << order.side << endl;
            currentSellQuantity += order.quantity;
        }
        cout << "\033[1;31mPrice: " << std::fixed << std::setprecision(2) << it->first << "\tQuantity: " << currentSellQuantity << "\033[0m" << endl;
    }

    double bps = calculateBPS();

    cout << "\033[1;37m\nBPS: " << std::fixed << std::setprecision(2) << bps << "\033[0m" << endl;

    cout << "\n\033[1;32mBUY ORDERS:\n\033[0m" << endl;
    
    for (auto it = buyOrders.rbegin(); it != buyOrders.rend(); ++it) {
        //cout << "Price: " << buyOrder.first << endl;
        int currentBuyQuantity = 0;
        for (auto& order : it->second) {
            //cout << "Timestamp: " << std::ctime(&order.timestamp) << "Price: " << order.price << "Quantity: " << order.quantity << "Side: " << order.side << endl;
            currentBuyQuantity += order.quantity;
        }
        cout << "\033[1;32mPrice: " << std::fixed << std::setprecision(2) << it->first << "\tQuantity: " << currentBuyQuantity << "\033[0m" << endl;

    }

}


void OrderBook::init(const string& side, const string& file_name) {

    ifstream file(file_name);

    if(!file.is_open()) {
        cerr << "Failed to open order data" << endl;
        return;
    }

    // To store each line of the file
    std::string line; 

    // Skip the header row, if any
    std::getline(file, line);

    double price;
    double quantity;

    while (std::getline(file, line)) {
        std::stringstream ss(line); // Create a stringstream for the current line
        std::string value;
    

        // Read and convert the first column (double)
        std::getline(ss, value, ','); // Split by comma
        price = std::stod(value); // Convert string to double

        // Read and convert the second column (int)
        std::getline(ss, value, ','); // Split by comma
        quantity = std::stoi(value); // Convert string to int

        // Get the current time point (timestamp)
        auto now = std::chrono::system_clock::now();
        
        // Convert to time_t for easy conversion to human-readable format
        std::time_t timestamp = std::chrono::system_clock::to_time_t(now);

        if (side == "BUY") {
            if (buyOrders.find(price) != buyOrders.end()) {
                buyOrders[price].push_back(Order(timestamp, price, quantity, side));
                buyOrderQuantity[price] += quantity;
            } else{
                buyOrders[price] = {Order(timestamp, price, quantity, side)};
                buyOrderQuantity[price] = quantity;
            }
        } else if (side == "SELL") {
            if (sellOrders.find(price) != sellOrders.end()) {
                sellOrders[price].push_back(Order(timestamp, price, quantity, side));
                sellOrderQuantity[price] += quantity;
            } else{
                sellOrders[price] = {Order(timestamp, price, quantity, side)};
                sellOrderQuantity[price] = quantity;
            }
        } else {
            throw std::invalid_argument("Invalid trade action.");
        }

        //std::cout << "Row: Column1 = " << price << ", Column2 = " << quantity << std::endl;

    }

    // Close the file
    file.close();

    //std::cout << "All " << side << " orders have been loaded into the order book" << std::endl;

}