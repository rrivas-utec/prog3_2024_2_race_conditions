//
// Created by rrivas on 31/10/2024.
//

#ifndef PROG3_2024_2_RACE_CONDITIONS_CLIENT_H
#define PROG3_2024_2_RACE_CONDITIONS_CLIENT_H

#include <iostream>

class Client {
//    std::atomic<double> balance = 0;
    double balance = 0;
    std::mutex mtx;
public:
    Client() = default;
    explicit Client(double balance): balance(balance) {}
    void debit(double amount) {
        std::lock_guard lg(mtx);
        balance += amount;
    }
    void withdraw(double amount) {
        std::lock_guard lg(mtx);
        balance -= amount;
    }

    friend std::ostream& operator<<(std::ostream& out, const Client& c) {
        out << c.balance;
        return out;
    }
};

#endif //PROG3_2024_2_RACE_CONDITIONS_CLIENT_H
