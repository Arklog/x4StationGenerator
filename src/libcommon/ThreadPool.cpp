//
// Created by pierre on 7/5/26.
//

#include "ThreadPool.hpp"

#include <functional>

namespace common {
    ThreadPool::ThreadPool(size_t nthreads) : _nthreads(nthreads), _should_stop(false), _tasks_mutex(), _tasks(),
                                              _threads() {
    }

    ThreadPool::~ThreadPool() {
        stop();

        while (!_tasks.empty()) {
            delete _tasks.front();
            _tasks.pop();
        }
    }

    void ThreadPool::enqueue(TaskBase *task) {
        _tasks_mutex.lock();
        _tasks.push(task);
        _tasks_mutex.unlock();
    }

    void ThreadPool::start() {
        for (size_t i = 0; i < _nthreads; ++i)
            _threads.emplace_back(std::bind(&ThreadPool::_thread_loop, this));
    }

    void ThreadPool::stop() {
        _should_stop = true;

        for (auto &thread: _threads)
            thread.join();
    }

    void ThreadPool::wait() {
        while (!empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
        }
    }

    bool ThreadPool::empty() {
        _tasks_mutex.lock();
        bool empty = _tasks.empty();
        _tasks_mutex.unlock();
        return empty;
    }

    void ThreadPool::_thread_loop() {
        do {
            TaskBase *task = nullptr;
            _tasks_mutex.lock();
            if (!_tasks.empty()) {
                task = _tasks.front();
                _tasks.pop();
            }
            _tasks_mutex.unlock();

            if (task) {
                task->run();
                delete task;
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(30));
            }
        } while (!_should_stop);
    }
} // common
