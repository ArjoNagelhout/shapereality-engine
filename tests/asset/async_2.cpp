//
// Created by Arjo Nagelhout on 01/03/2024.
//

#include <gtest/gtest.h>

#include <future>
#include <iostream>
#include <chrono>

namespace async_2
{
    void sleep(int seconds)
    {
        std::this_thread::sleep_for(std::chrono::seconds(seconds));
    }

    void lalala()
    {
        sleep(1);
        std::cout << "done" << std::endl;
    }

    template<typename T>
    std::shared_future<T> then(std::shared_future<T> f, std::function<void()> const& callback)
    {
        return std::async([&f, callback]() {
            f.wait();
            callback();
        });
    }

    template<typename T>
    T await(std::shared_future<T> f)
    {
        return f.get();
    }

    TEST(Async, Lalala)
    {
        std::shared_future a = std::async(lalala);
        std::shared_future b = then(a, []() {
            sleep(2);
            std::cout << "done with a" << std::endl;
        });
        std::shared_future c = then(b, []() {
            sleep(1);
            std::cout << "done with b" << std::endl;
        });
        std::shared_future d = then(a, []() {
            sleep(4);
            std::cout << "done with a, 2" << std::endl;
        });

        std::cout << "wejo" << std::endl;
    }

    TEST(Await, Lalala)
    {
        std::shared_future a = std::async(lalala);
        await(a);
        std::shared_future b = std::async([]() {
            sleep(2);
        });
        await(b);
        std::cout << "done with a" << std::endl;
    }
}