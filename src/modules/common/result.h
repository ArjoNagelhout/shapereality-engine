//
// Created by Arjo Nagelhout on 22/02/2024.
//

#ifndef SHAPEREALITY_RESULT_H
#define SHAPEREALITY_RESULT_H

#include <utility>
#include <string>
#include <type_traits>

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
            success_ ? value_.~Type() : error_.~Error();
        }

        explicit Result(Type&& value)
        requires (!std::is_trivially_copyable_v<Type>)
        {
            value_ = std::move(value);
            success_ = true;
        }

        explicit Result(Type value)
        requires std::is_trivially_copyable_v<Type>
        {
            value_ = value;
            success_ = true;
        }

        explicit Result(size_t code, std::string const& message)
        {
            success_ = false;
            error_.code = code;
            error_.message = message;
        }

        // delete copy constructor and assignment operator
        Result(const Result&) = delete;

        Result& operator=(const Result&) = delete;

        // creates success result
        static Result makeSuccess(Type&& value)
        requires (!std::is_trivially_copyable_v<Type>)
        {
            return Result(std::forward<Type>(value));
        }

        // creates success result
        static Result makeSuccess(Type value)
        requires std::is_trivially_copyable_v<Type>
        {
            return Result(value);
        }

        // creates error result
        template<typename ErrorCode>
        requires std::is_enum_v<ErrorCode> && std::is_same_v<std::underlying_type_t<ErrorCode>, size_t>
        static Result makeError(ErrorCode code_ = {}, std::string const& message_ = "")
        {
            return Result(static_cast<size_t>(code_), message_);
        }

        // returns whether an error occurred
        [[nodiscard]] bool error() const
        {
            return !success_;
        }

        // returns whether the result is successful
        [[nodiscard]] bool success() const
        {
            return success_;
        }

    private:
        bool success_ = false;
        union
        {
            Type value_;
            Error error_{};
        };
    };
}

#endif //SHAPEREALITY_RESULT_H
