#include <iostream>
#include "../include/Order.h"
#include "../include/OrderBook.h"


using std::cout;
using std::endl;
using std::cin;

int main() {

    // std::cout << "__cplusplus: " << __cplusplus << std::endl;

    // Order Book Time
    OrderBook orderbook;
    cout << "\033[1;34m";
    cout << "Welcome to the C++ Orderbook\n" << endl;
    cout << "Initializing Orderbook\n" << endl;
    orderbook.init("BUY", "../init_data/buy_orders.csv");
    orderbook.init("SELL", "../init_data/sell_orders.csv");
    

    while(true) {
        cout << "\n\033[1;34mOptions: \n 1. Print Orderbook\n 2. Enter Order\n 3. Exit\033[0m" << endl;
        cout << "\nEnter Option: ";
        int option;
        cin >> option;
        if(option == 1) {
            orderbook.printOrders();
        } else if(option == 2) {
            //cout << "\n\033[1;34mOptions\n1.Market Order\n2.Limit Order\033[0m" << endl;
            cout << "\n\033[1;34mEnter Order Type: \n 1. Market Order\n 2. Limit Order\033[0m" << endl;

            int orderChoice;
            cout << "\nEnter Choice: ";
            cin >> orderChoice;

            cout << "\n\033[1;34mEnter Side:\n 1. BUY\n 2. SELL\033[0m" << endl;

            cout << "\nEnter Choice: ";
            int sideChoice;
            cin >> sideChoice;

            cout << "Enter order quantity: ";

            int quantity;
            cin >> quantity;
            cout << "\n";

            if(orderChoice == 1) {
                if(sideChoice == 1) {
                    orderbook.enterOrder("BUY", quantity);
                } else if(sideChoice == 2) {
                    orderbook.enterOrder("SELL", quantity);
                }
            }
            // } else if(orderChoice == 2) {
            //     cout << "Enter Price: ";
            //     double price;
            //     cin >> price;
            //     if(sideChoice == 1) {
            //         orderbook.enterOrder("BUY", price, quantity);
            //     } else if(sideChoice == 2) {
            //         orderbook.enterOrder("SELL", price, quantity);
            //     }
            // }

            // cout << "Enter Side: ";
            // string side;
            // cin >> side;
            // cout << "Enter Price: ";
            // double price;
            // cin >> price;
            // cout << "Enter Quantity: ";
            // int quantity;
            // cin >> quantity;
            // cout << "Enter Order Type: ";
            // string orderType;
            // cin >> orderType;
            // orderbook.enterOrder(side, price, quantity, orderType);
        } else if(option == 3) {
            break;
        }
    }

    return 0;

}