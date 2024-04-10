//
// Created by Arjo Nagelhout on 22/02/2024.
//

#ifndef SHAPEREALITY_RESULT_H
#define SHAPEREALITY_RESULT_H

#include <utility>
#include <string>
#include <type_traits>
#include <cassert>

namespace common
{
    /**
     * Taken from Google Abseil (https://abseil.io/docs/cpp/guides/status)
     */
    enum class ResultCode : unsigned int
    {
        Success = 0,
        Cancelled = 1,
        Unknown = 2,
        InvalidArgument = 3,
        DeadlineExceeded = 4,
        NotFound = 5,
        AlreadyExists = 6,
        PermissionDenied = 7,
        ResourceExhausted = 8,
        FailedPrecondition = 9,
        Aborted = 10,
        OutOfRange = 11,
        Unimplemented = 12,
        Internal = 13, // internal error
        Unavailable = 14,
        DataLoss = 15,
        Unauthenticated = 16, // authentication credentials are not valid
    };

    [[nodiscard]] std::string_view toString(ResultCode code);

    /**
     * A simple implementation for error handling by return values.
     *
     * When Result is erroneous, you can call code() or message() to get the error code and error message, respectively.
     *
     * Terminates the program (if asserts are enabled) when trying to access the value when Result is erroneous, or when
     * trying to access the error code and message when Result is valid.
     */
    template<typename Type>
    class ValueResult
    {
    public:
        ~ValueResult()
        {
            code_ == ResultCode::Success ? value_.~Type() : message_.~basic_string();
        }

        // non-trivial copy constructor
        ValueResult(ValueResult const& other) : code_(other.code_)
        {
            if (code_ == ResultCode::Success)
            {
                // copy value
                new(&value_) Type(other.value_); // placement new using copy constructor
                //value_ = other.value_; creates segfault at the std::filesystem::path destructor with the ImportResultData
            }
            else
            {
                // copy error using placement new
                new(&message_) std::string(other.message_);
            }
        }

        // delete assignment operator
        ValueResult& operator=(const ValueResult&) = delete;

        // creates success result
        static ValueResult makeSuccess(Type&& value)
        requires (!std::is_trivially_copyable_v<Type>)
        {
            return ValueResult(std::forward<Type>(value));
        }

        // creates success result
        static ValueResult makeSuccess(Type value)
        requires std::is_trivially_copyable_v<Type>
        {
            return ValueResult(value);
        }

        // creates error result
        static ValueResult makeError(ResultCode code, std::string const& message = "")
        {
            return ValueResult(code, message);
        }

        // returns whether an error occurred
        [[nodiscard]] bool error() const
        {
            return code_ != ResultCode::Success;
        }

        // returns whether the result is successful
        [[nodiscard]] bool success() const
        {
            return code_ == ResultCode::Success;
        }

        // get the error message
        [[nodiscard]] std::string const& message() const
        {
            assert(code_ != ResultCode::Success);
            return message_;
        }

        // get the value of the result
        [[nodiscard]] Type const& get() const
        requires (!std::is_trivially_copyable_v<Type>)
        {
            assert(code_ == ResultCode::Success);
            return value_;
        }

        // get the value of the result
        [[nodiscard]] Type get() const
        requires std::is_trivially_copyable_v<Type>
        {
            assert(code_ == ResultCode::Success);
            return value_;
        }

        // get the error code
        [[nodiscard]] ResultCode code() const
        {
            return code_;
        }

    private:
        ResultCode code_;
        union
        {
            Type value_;
            std::string message_{};
        };

        explicit ValueResult(Type&& value)
        requires (!std::is_trivially_copyable_v<Type>)
            : value_(std::move(value))
        {
            code_ = ResultCode::Success;
        }

        explicit ValueResult(Type value)
        requires std::is_trivially_copyable_v<Type>
        {
            value_ = value;
            code_ = ResultCode::Success;
        }

        explicit ValueResult(ResultCode code, std::string const& errorMessage)
        {
            code_ = code;
            message_ = errorMessage;
        }
    };
}

#endif //SHAPEREALITY_RESULT_H
