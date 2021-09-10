//
// Created by zhenkai on 2021/9/8.
//

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

int main(void) {
    std::condition_variable cv;
    std::mutex mtx;
    std::atomic<bool> ready = {false};

    std::queue<int> queue;

    auto producter = [&]() {
        for (int i = 0; i < 20; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::unique_lock<std::mutex> lock(mtx);
            queue.push(i);
            std::cout << "producter : " << i << std::endl;
            ready = true;
            lock.unlock();
            //            cv.notify_one();
            cv.notify_all();
        }
    };

    auto customer = [&]() {
        while (true) {
            std::unique_lock<std::mutex> lock(mtx);
            //            while (!ready) {
            while (queue.empty()) {
                cv.wait(lock);
            }
            // 短暂取消锁，使得生产者有机会在消费者消费空前继续生产
            lock.unlock();

            std::this_thread::sleep_for(std::chrono::seconds(1));

            lock.lock();
            //            while (!queue.empty()) {
            if (!queue.empty()) {
                std::cout << " id : " << std::this_thread::get_id()
                          << " val: " << queue.front() << std::endl;
                queue.pop();
            }
            //            ready = false;
        }
    };

    int n = 10;
    std::thread ct[n];
    for (int i = 0; i < n; ++i) {
        ct[i] = std::thread(customer);
    }

    std::thread pr(producter);

    for (int j = 0; j < n; ++j) {
        ct[j].join();
    }

    pr.join();

    return 0;
}