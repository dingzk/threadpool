//
// Created by zhenkai on 2021/9/7.
//

#include "cv.h"

#include <iostream>
#include <atomic>
#include <pthread.h>
#include <queue>
#include <thread>
#include <unistd.h>

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int main(void) {

    static std::queue<int> queue;
    static int i = 0;

    auto coustumer = []() {
      auto id = std::this_thread::get_id();
        while (true) {
            pthread_mutex_lock(&mutex);
            while (queue.size() > 0) {
                std::cout << "customer: id " << id << " v : " << queue.front() << std::endl;
                queue.pop();
            }
            if (queue.size() == 0) {
                pthread_cond_wait(&cond, &mutex);
            }
            pthread_mutex_unlock(&mutex);
        }
    };

    auto producter = []() {
        pthread_mutex_lock(&mutex);
        while (queue.size() <= 10) {
            queue.push(i++);
            std::cout << "productor : " << i - 1 << std::endl;
        }
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond);
    };

    std::thread c1(coustumer);
    std::thread c2(coustumer);
    std::thread p(producter);

    c1.detach();
    c2.detach();
    p.detach();


    sleep(1);
    std::thread p1(producter);

    sleep(2);

    p1.detach();

    return 0;
}