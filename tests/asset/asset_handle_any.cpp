//
// Created by Arjo Nagelhout on 12/04/2024.
//

#include <gtest/gtest.h>

namespace asset_handle_any
{
    template<typename Type>
    void deleterImplementation(void* data)
    {
        delete (Type*)data;
    }

    struct AssetHandle
    {
        void* data = nullptr;
        void (* deleter)(void*);

        template<typename Type>
        void set(Type* data_)
        {
            std::any a;
            assert(data_ && "data should not be nullptr");
            data = data_;
            deleter = deleterImplementation<Type>;
        }

        template<typename Type>
        void set(std::unique_ptr<Type>&& data_)
        {
            assert(data_ && "data should not be nullptr");
            data_.get_deleter();
            data = data_.release();
        }
    };
}