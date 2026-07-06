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
        _threads_running.resize(_nthreads, true);
        for (size_t i = 0; i < _nthreads; ++i)
            _threads.emplace_back(std::bind(&ThreadPool::_thread_loop, this, i));
    }

    void ThreadPool::stop() {
        _should_stop = true;

        for (auto &thread: _threads)
            thread.join();
    }

    void ThreadPool::wait() {
        while (!empty() || running()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
        }
    }

    bool ThreadPool::empty() {
        _tasks_mutex.lock();
        bool empty = _tasks.empty();
        _tasks_mutex.unlock();
        return empty;
    }

    bool ThreadPool::running() {
        std::lock_guard<std::mutex> lock(_threads_running_mutex);
        if (_threads_running.empty())
            return false;
        return std::any_of(_threads_running.begin(), _threads_running.end(), [](bool running) { return running; });
    }

    void ThreadPool::_thread_loop(size_t id) {
        do {
            TaskBase *task = nullptr;
            _tasks_mutex.lock();
            if (!_tasks.empty()) {
                task = _tasks.front();
                _tasks.pop();
                _threads_running_mutex.lock();
                _threads_running[id] = true;
                _threads_running_mutex.unlock();
            } else {
                _threads_running_mutex.lock();
                _threads_running[id] = false;
                _threads_running_mutex.unlock();
            }
            _tasks_mutex.unlock();

            if (task) {
                task->run();
                delete task;
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(30));
            }
        } while (!_should_stop);

        _threads_running_mutex.lock();
        _threads_running[id] = false;
        _threads_running_mutex.unlock();
    }
} // common
