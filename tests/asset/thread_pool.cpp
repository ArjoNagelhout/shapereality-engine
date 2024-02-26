//
// Created by Arjo Nagelhout on 26/02/2024.
//

#include <gtest/gtest.h>

#include <BS_thread_pool.hpp>
#include <iostream>
#include <thread>

namespace thread_pool_test
{
    TEST(Asset, ThreadPool)
    {
        unsigned int n = std::thread::hardware_concurrency();
        std::cout << "created thread pool with " << n << " threads." << std::endl;
        assert(n > 0);
        auto threadPool = BS::thread_pool(n);
    }
}