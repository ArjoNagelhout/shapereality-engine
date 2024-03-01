//
// Created by Arjo Nagelhout on 01/03/2024.
//

#include "thread_pool.h"

#include <BS_thread_pool.hpp>

namespace common
{
    BS::thread_pool& ThreadPool::shared()
    {
        static BS::thread_pool instance_;
        return instance_;
    }
}