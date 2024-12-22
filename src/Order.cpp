// This is an order class implementation. 

#include <string>
#include <chrono>
#include "../include/Order.h"

using std::string;
using std::time_t;

// Constructor definition
Order::Order(time_t orderTimestamp, double orderPrice, int orderQuantity, string orderSide) : timestamp(orderTimestamp), price(orderPrice), quantity(orderQuantity), side(orderSide) {}
