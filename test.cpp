//
// Created by zhenkai on 2021/9/9.
//

#include <chrono>
#include <iostream>
#include <thread>
#include <threadpool.h>
#include <unistd.h>

class A {
public:
    A(int a = 10) : m_a(a) { std::cout << "construct a :" << a << std::endl; }
    A(const A &a) {
        std::cout << "copy construct a :" << a.m_a << std::endl;
        this->m_a = a.m_a;
    }
    A &operator=(const A &a) {
        std::cout << "copy assign a :" << a.m_a << std::endl;
        this->m_a = a.m_a;
        return *this;
    }
    A(const A &&a) {
        std::cout << "move construct a :" << a.m_a << std::endl;
        this->m_a = a.m_a;
    }
    A &operator=(const A &&a) {
        std::cout << "move assign a :" << a.m_a << std::endl;
        this->m_a = a.m_a;
        return *this;
    }
    ~A() { std::cout << "destruct a :" << m_a << std::endl; }
    void run() { std::cout << "run a : " << m_a << std::endl; }

public:
    int m_a;
};

int main() {
    ThreadPool<A> pool(5, 1024);

    pool.Run();

    int m, n;
    m = n = 1;
    std::thread t[n];
    while (n--) {
        t[n] = std::thread([&pool, n]() {
          pool.Push(std::move(A(n)));
          std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        });
    }

    while (m--) {
        t[m].join();
    }

//    t.join();

    pool.Stop();

//    std::thread t1([&pool]() {
//        pool.Push(A(2));
//        pool.Push(A(3));
//        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//    });
//
//    t1.join();

    sleep(2);

    return 0;
}