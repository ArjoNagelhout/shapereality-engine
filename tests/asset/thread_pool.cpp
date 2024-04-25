//
// Created by Arjo Nagelhout on 26/02/2024.
//

#include <gtest/gtest.h>

#include <BS_thread_pool.hpp>
#include <iostream>
#include <thread>
#include <chrono>

namespace thread_pool_test
{
    using InputFile = size_t;

    struct Task
    {
        std::future<void> future;

        explicit Task(std::future<void>&& future_) : future(std::move(future_)) {}
    };

    struct Result
    {
        bool test = true;
    };

    struct Storage
    {
        std::unordered_map<InputFile, Task> tasks;
        std::unordered_map<InputFile, Result> results;
        std::mutex tasksMutex;
    };

    void performTask(Storage& storage, InputFile inputFile)
    {
        std::cout << "perform task " << inputFile << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    void startTaskIfNeeded(BS::thread_pool& pool, Storage& storage, InputFile inputFile)
    {
        std::lock_guard<std::mutex> guard(storage.tasksMutex);
        if (storage.tasks.contains(inputFile))
        {
            // task already running
            std::cout << "task " << inputFile << " already running" << std::endl;
            return;
        }

        std::cout << "start task " << inputFile << std::endl;

        std::future<void> future = pool.submit_task([&storage, inputFile]() {
            performTask(storage, inputFile);

            // clean up
            std::cout << "clean task " << inputFile << std::endl;

            // lock
            std::lock_guard<std::mutex> guard(storage.tasksMutex);

            // move task
            Task task(std::move(storage.tasks.at(inputFile)));
            storage.tasks.erase(inputFile);
        });
        storage.tasks.emplace(inputFile, std::move(future));
    }

    TEST(Asset, ThreadPool)
    {
        return;
        Storage storage;

        unsigned int n = std::thread::hardware_concurrency();
        std::cout << "created thread pool with " << n << " threads." << std::endl;
        assert(n > 0);
        BS::thread_pool pool = BS::thread_pool(n);

        startTaskIfNeeded(pool, storage, 0);
        startTaskIfNeeded(pool, storage, 0);

        std::this_thread::sleep_for(std::chrono::seconds(1));

        startTaskIfNeeded(pool, storage, 1);

        std::this_thread::sleep_for(std::chrono::seconds(3));

        startTaskIfNeeded(pool, storage, 2);

        std::cout << "weee" << std::endl;
    }
}