#include <iostream>
#include <thread>
#include <vector>
#include <cmath>
#include <mutex>
#include "Client.h"

void task_accumulate_cos(double radian, double& total) {
    total += std::cos(radian);
}

void ejercicio_3() {
    double total = 0;
    double interval = 0.01;
    int n = 100;
    for (int i = 0; i < n; ++i) {
        total += std::cos(i*interval);
    }
    std::cout << total << std::endl;

    total = 0;
    std::vector<std::thread> vh;
    vh.reserve(n);
    for (int i = 0; i < n; ++i)
        vh.emplace_back(task_accumulate_cos, i*interval, std::ref(total));
    for (int i = 0; i < n; ++i)
        vh[i].join();
    std::cout << total << std::endl;
}

int incrementar_total_nc(int n, int inc) {
    int total = 0;
    for (int i = 0; i < n; ++i)
        total += inc;
    return total;
}

std::mutex mtx;

class guardian_de_lock {
    std::mutex& mtx;
public:
    explicit guardian_de_lock(std::mutex& mtx): mtx(mtx) { mtx.lock();}
    ~guardian_de_lock() { mtx.unlock();}
};

void task_inc(int inc, int& total) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
//    mtx.lock();
//    std::lock_guard lg(mtx);
    std::unique_lock ul(mtx);
    total += inc;
    ul.unlock();
//    mtx.unlock();
}

int incrementar_total(int n, int inc) {
    int total = 0;
    std::vector<std::thread> vh(n);
    for (auto& item: vh) {
        item = std::thread(task_inc, inc, std::ref(total));
    }
    for (auto& item: vh) {
        item.join();
    }
    return total;
}

int incrementar_total_atomic(int n, int inc) {
//    std::atomic<int> total = 0;
    int total = 0;
    std::vector<std::thread> vh(n);
    for (auto& item: vh) {
        item = std::thread(
                [inc, &total]() {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                    total += inc;
                }
                );
    }
    for (auto& item: vh) {
        item.join();
    }
    return total;
}

void ejemplo() {
    std::cout << incrementar_total_nc(100, 1) << std::endl;
    std::cout << incrementar_total(100, 1) << std::endl;

    int m = 100;
    for (int i = 0; i < m; ++i) {
        std::cout << incrementar_total(100, 1) << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < m; ++i) {
        std::cout << incrementar_total_atomic(100, 1) << " ";
    }
}

void ejemplo_2() {
    Client c1;
    int nd = 100;
    int nr = 100;
    for (int i = 0; i < nd; ++i)
        c1.debit(2);
    for (int i = 0; i < nr; ++i)
        c1.withdraw(1);
    std::cout << c1 << std::endl;
}

void ejemplo_3() {
    Client c1;
    int nd = 100;
    int nr = 100;
    int vd = 2;
    int vr = 1;

    std::vector<std::thread> vhd(nd);
    std::vector<std::thread> vhr(nr);

    using namespace std::chrono_literals;
    for (auto& hd: vhd) {
        hd = std::thread (
                [&c1, vd](){
                    std::this_thread::sleep_for(10ms);
                    c1.debit(vd);
                }
        );
    }
    for (auto& hr: vhr) {
        hr = std::thread (
                [&c1, vr](){ c1.withdraw(vr);
                }
        );
    }

    for (auto& hd: vhd) hd.join();
    for (auto& hr: vhr) hr.join();

    std::cout << c1 << std::endl;
}

int main() {
//    ejemplo();
    ejemplo_2();
    ejemplo_3();
    return 0;
}
