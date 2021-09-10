//
// Created by zhenkai on 2021/9/8.
//

#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

template <class T> class ThreadPool {
public:
    explicit ThreadPool(int worker_num = 5, int queue_size = 1024)
        : m_worker_num(worker_num), m_queue_size(queue_size), m_stop(false) {
    };
    ~ThreadPool() = default;

public:
    void Stop() { m_stop = true; }

    void Run() {
        for (int i = 0; i < m_worker_num; ++i) {
            std::thread t([this, i]() {
              while (!m_stop) {
                  auto t = Pop();
                  std::cout << "thread num: " << i << " get" << std::endl;
                  t.run();
              }
              std::cout << "stop thread : " << i << std::endl;
            });
            t.detach();
        }
    }

    void Push(T &&ele) {
        std::unique_lock<std::mutex> lock(m_mtx);
        while (m_queue.size() > m_queue_size) {
            m_not_empty.wait(lock);
        }
        m_queue.emplace(std::forward<T>(ele));
//        m_queue.emplace(std::move(ele));
        lock.unlock();
        m_not_full.notify_all();
    }

private:
    T Pop() {
        std::unique_lock<std::mutex> lock(m_mtx);
        while (m_queue.empty()) {
            m_not_full.wait(lock);
        }
        T t = std::move(m_queue.front());
        m_queue.pop();
        lock.unlock();
        m_not_empty.notify_all();

        return t;
    }

private:
    int m_worker_num;
    int m_queue_size;
    std::atomic<bool> m_stop;
    std::condition_variable m_not_full;
    std::condition_variable m_not_empty;
    std::mutex m_mtx;
    std::queue<T> m_queue;
};