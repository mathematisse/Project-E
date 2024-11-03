/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base SystemTree
*/

#include "lib_ecs/Systems/ExecutionTypes.hpp"
#include "lib_log/log.hpp"
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <atomic>
#include "ThreadPool.hpp"

ThreadPool::ThreadPool(size_t numThreads):
    stop(false),
    tasksInProgress(0)
{
    std::cout << "Creating ThreadPool with " << numThreads << " threads" << std::endl;
    for (size_t i = 0; i < numThreads; ++i) {
        threads.emplace_back(&ThreadPool::worker, this);
    }
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }
    condition.notify_all();

    for (auto &thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void ThreadPool::submit(const std::function<void()> &task)
{
    std::cout << "Submitting task" << std::endl;
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        tasks.emplace(task);
        ++tasksInProgress;
    }
    condition.notify_one();
}

void ThreadPool::waitAll()
{
    if (tasksInProgress == 0) {
        return;
    }
    {
        std::unique_lock<std::mutex> lock(completionMutex);
        completionCondition.wait(lock, [this]() {
            return tasksInProgress == 0;
        });
    }
}

void ThreadPool::worker()
{
    while (true) {
        std::function<void()> task;

        {
            std::unique_lock<std::mutex> lock(queueMutex);

            condition.wait(lock, [this]() {
                return !tasks.empty() || stop;
            });

            if (stop && tasks.empty()) {
                return;
            }

            if (!tasks.empty()) {
                task = tasks.front();
                tasks.pop();
            } else {
                continue;
            }
        }

        task();

        if (--tasksInProgress == 0) {
            std::unique_lock<std::mutex> lock(completionMutex);
            completionCondition.notify_all();
        }
    }
}

