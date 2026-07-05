//
// Created by pierre on 7/5/26.
//

#ifndef X4STATIONGENERATOR__THREADPOOL_HPP
#define X4STATIONGENERATOR__THREADPOOL_HPP
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace common {
    template<typename T, typename... Args>
    concept is_executable = requires(T t, Args... args) { t(std::declval<args>()...); };

    class TaskBase {
    public:
        virtual ~TaskBase() = default;

        virtual void run() = 0;
    };

    template<is_executable T>
    class Task : public TaskBase {
    public:
        Task(T fn) : _fn(fn) {
        }

        virtual ~Task() = default;

        void run() override {
            _fn();
        }

    private:
        T _fn;
    };

    class ThreadPool {
    public:
        ThreadPool(size_t nthreads);

        ~ThreadPool();

        void enqueue(TaskBase *task);

        void start();

        void stop();

        void wait();

        bool empty();

    private:
        size_t                   _nthreads;
        std::vector<std::thread> _threads;
        std::queue<TaskBase *>   _tasks;
        std::mutex               _tasks_mutex;
        bool                     _should_stop;

        void _thread_loop();
    };
} // common

#endif //X4STATIONGENERATOR__THREADPOOL_HPP
