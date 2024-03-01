//
// Created by Arjo Nagelhout on 01/03/2024.
//

#ifndef SHAPEREALITY_THREAD_POOL_H
#define SHAPEREALITY_THREAD_POOL_H

namespace BS
{
    class thread_pool;
}

namespace common
{
    // put inside struct, otherwise shared() would have internal linkage, now we have external linkage
    // ensuring throughout the entire program, only one instance exists, not just inside the current
    // compilation unit. C++ is wild.
    struct ThreadPool
    {
        [[nodiscard]] static BS::thread_pool& shared();
    };
}

#endif //SHAPEREALITY_THREAD_POOL_H
