#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <vector>
#include <map>
#include <string>
#include "Order.h" // Include the Order class

class OrderBook {

    private:
        std::map<double, std::vector<Order>> buyOrders;
        std::map<double, std::vector<Order>> sellOrders;
        std::map<double, int> buyOrderQuantity;
        std::map<double, int> sellOrderQuantity;

    public:
        void init(const std::string& side, const std::string& file_name);
        void enterOrder(const std::string& side, int quantity);
        double calculateBPS();
        //void enterOrder(const std::string& side, double price, int quantity);
        void printOrders();
};

#endif // ORDERBOOK_H
