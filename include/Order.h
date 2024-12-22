#ifndef ORDER_H // Include guard to prevent multiple inclusion
#define ORDER_H

#include <string>

class Order {
public:
    int timestamp;
    double price;
    int quantity;
    std::string side;

    // Constructor
    Order(time_t orderTimestamp, double orderPrice, int orderQuantity, std::string orderSide);
};

#endif // ORDER_H
