//
// Created by Arjo Nagelhout on 26/02/2024.
//

#include <gtest/gtest.h>

#include <BS_thread_pool.hpp>
#include <iostream>
#include <thread>

namespace thread_pool_test2
{
    using InputFile = size_t;

    struct Result
    {
        bool test = true;
    };

    void performTask(InputFile inputFile)
    {
        std::cout << "perform task " << inputFile << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(inputFile));
    }

    struct Storage
    {
        BS::thread_pool& threadPool;

        // we use a shared future to enable copying in the destructor and waiting on them there,
        // while still enabling removing them from tasks on completion.
        std::unordered_map<InputFile, std::shared_future<void>> tasks;
        std::unordered_map<InputFile, Result> results;
        std::mutex tasksMutex;

        explicit Storage(BS::thread_pool& threadPool_) : threadPool(threadPool_) {}

        ~Storage()
        {
            // for std::futures that were not created by std::async, we need to explicitly wait as the
            // destructor of a std::future does not wait.

            std::unique_lock<std::mutex> lock(tasksMutex);
            std::vector<std::shared_future<void>> copied;
            copied.reserve(tasks.size());
            for (auto& task: tasks)
            {
                copied.emplace_back(task.second);
            }
            lock.unlock();

            for (auto& task: copied)
            {
                task.get();
            }

            std::cout << "storage destroyed" << std::endl;
        }

        void startTaskIfNeeded(InputFile inputFile)
        {
            if (tasks.contains(inputFile))
            {
                // task already running
                std::cout << "task " << inputFile << " already running" << std::endl;
                return;
            }

            std::cout << "start task " << inputFile << std::endl;

            std::shared_future<void> future = threadPool.submit_task([=, this]() {
                performTask(inputFile);

                // lock
                std::lock_guard<std::mutex> guard(tasksMutex);

                // erase task
                tasks.erase(inputFile);

                // clean up
                std::cout << "task done " << inputFile << std::endl;
            });
            tasks.emplace(inputFile, std::move(future));
        }
    };

    TEST(Asset, ThreadPool2)
    {
        unsigned int n = std::thread::hardware_concurrency();
        std::cout << "created thread pool with " << n << " threads." << std::endl;
        assert(n > 0);
        BS::thread_pool pool = BS::thread_pool(n);

        Storage storage(pool);
        storage.startTaskIfNeeded(0);
        storage.startTaskIfNeeded(1);
        storage.startTaskIfNeeded(2);
        storage.startTaskIfNeeded(3);
        storage.startTaskIfNeeded(4);
    }
}