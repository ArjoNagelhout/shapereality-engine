//
// Created by Arjo Nagelhout on 23/02/2024.
//

#ifndef SHAPEREALITY_PROMISE_H
#define SHAPEREALITY_PROMISE_H

#include <utility>
#include <string>

namespace common
{
    /**
     * A Promise is a simple type an asynchronous function returns,
     * on which the caller can call methods such as .onComplete() and
     */
    template<typename Type>
    class Promise
    {
    public:
        struct Error
        {
            size_t code{};
            std::string message;
        };

        enum class State
        {
            None,
            Running,
            Success,
            Error
        };

        using SuccessCallback = std::function<void(Type const& value)>;
        using ErrorCallback = std::function<void(Error const& error)>;

        void onSuccess(SuccessCallback& callback)
        {
            onSuccess_ = callback;
        }

        void onError(ErrorCallback& callback)
        {
            onError_ = callback;
        }

    private:
        SuccessCallback& onSuccess_;
        ErrorCallback& onError_;
    };
}

#endif //SHAPEREALITY_PROMISE_H
