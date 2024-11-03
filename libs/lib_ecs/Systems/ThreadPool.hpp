/*
** EPITECH PROJECT, 2024
** CPP-ECS
** File description:
** Ecs pure base SystemTree
*/

#pragma once

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <atomic>

class ThreadPool {
public:
    explicit ThreadPool(size_t numThreads);

    ~ThreadPool();

    void submit(const std::function<void()> &task);

    void waitAll();

private:
    void worker();

    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    std::atomic<bool> stop;

    std::atomic<int> tasksInProgress;
    std::mutex completionMutex;
    std::condition_variable completionCondition;
};

// Global ThreadPool instance
extern ThreadPool globalThreadPool;
