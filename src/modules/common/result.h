//
// Created by Arjo Nagelhout on 22/02/2024.
//

#ifndef SHAPEREALITY_RESULT_H
#define SHAPEREALITY_RESULT_H

#include <utility>
#include <string>

namespace common
{
    /**
     *
     */
    template<typename Type>
    class Result
    {
    public:
        struct Error
        {
            size_t code{0};
            std::string message;
        };

        ~Result()
        {
            success_ ? value.~Type() : error.~Error();
        }

        explicit Result(Type&& value_)
        {
            success_ = true;
        }

        explicit Result(size_t code, std::string const& message)
        {
            success_ = false;
            error.code = code;
            error.message = message;
        }

        // delete copy constructor and assignment operator
        Result(const Result&) = delete;
        Result& operator=(const Result&) = delete;

        static Result createSuccess(Type&& value_)
        {
            return Result(value_);
        }

        static Result createError(size_t code_ = 0, std::string const& message_ = "")
        {
            return Result(code_, message_);
        }

        [[nodiscard]] bool success()
        {
            return success_;
        }

    private:
        bool success_ = false;
        union
        {
            Type value;
            Error error{};
        };
    };
}

#endif //SHAPEREALITY_RESULT_H
