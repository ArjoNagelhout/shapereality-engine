//
// Created by Arjo Nagelhout on 12/04/2024.
//

#ifndef SHAPEREALITY_UNIQUE_ANY_POINTER_H
#define SHAPEREALITY_UNIQUE_ANY_POINTER_H

#include <memory>
#include <reflection/type_id.h>

namespace reflection
{
    struct UniqueAnyPointer;

    namespace any_implementation
    {
        enum class Action
        {
            Move,
            Destroy,
            Get,
            TypeId
        };

        // forward declaration
        template<typename Type, typename Deleter>
        struct UniqueAnyPointerHandle;
    }

    /**
     * A unique any pointer is an alternative for std::unique_ptr<Type, Deleter>.
     *
     * Design goals:
     * - interoperable with std::unique_ptr<Type, Deleter> (convert from and to using move semantics)
     * - not copyable or copy constructible.
     * - ability to take ownership of a pointer, similar to std::unique_ptr
     * - release() and get_deleter() are only supported if the caller knows the type at compile time,
     *   otherwise we would return a void* to the deleter, thus rendering it useless.
     * - on reset() or release(), type information gets reset
     */
    class UniqueAnyPointer
    {
    public:
        using Action = any_implementation::Action;
        template<typename Type, typename Deleter> using Handler = any_implementation::UniqueAnyPointerHandle<Type, Deleter>;
        using HandleFunctionPointer = void* (*)(Action action, UniqueAnyPointer const* this_, UniqueAnyPointer* other);

        // construct empty pointer
        UniqueAnyPointer();

        // construct empty pointer
        explicit UniqueAnyPointer(nullptr_t);

        // construct from pointer, default constructible deleter
        template<typename Type, typename Deleter>
        requires std::is_default_constructible_v<Deleter>
        explicit UniqueAnyPointer(Type* data_)
        {
            deleter = new Deleter(); // calls default constructor
            handle = &Handler<Type, Deleter>::handle;
        }

        // construct from pointer, movable deleter
        template<typename Type, typename Deleter>
        requires std::is_move_constructible_v<Deleter>
        explicit UniqueAnyPointer(Type* data_, Deleter&& deleter_) : data(data_)
        {
            deleter = new Deleter(deleter_); // calls move constructor
            handle = &Handler<Type, Deleter>::handle;
        }

        template<typename Type, typename Deleter>
        requires std::is_copy_constructible_v<Deleter>
        explicit UniqueAnyPointer(Type* data_, Deleter& deleter_) : data(data_)
        {
            deleter = new Deleter(deleter_); // calls copy constructor
            handle = &Handler<Type, Deleter>::handle;
        }

        // construct from unique pointer
        template<typename Type, typename Deleter>
        explicit UniqueAnyPointer(std::unique_ptr<Type, Deleter>&& other)
        {
//            data = other.release();
//            deleter = new Deleter(other.get_deleter());
        }

        // move constructor
        UniqueAnyPointer(UniqueAnyPointer&& other) noexcept;

        // destructor (calls reset)
        ~UniqueAnyPointer();

        // delete copy constructor
        UniqueAnyPointer(UniqueAnyPointer const&) = delete;

        // delete copy assignment operator
        UniqueAnyPointer& operator=(UniqueAnyPointer const&) = delete;

        // type

        [[nodiscard]] bool isType(TypeId typeId) const;

        template<typename Type>
        [[nodiscard]] bool isType() const
        {
            return isType(TypeIndex<Type>::value());
        }

        [[nodiscard]] TypeId typeId() const;

        // methods

        template<typename Type>
        [[nodiscard]] Type* get()
        {
            assert(isType<Type>());
            if (data)
            {
                return static_cast<Type*>(data);
            }
            return nullptr;
        }

        // reset the data and remove type information
        void reset();

        // swap this unique any and other unique any
        void swap(UniqueAnyPointer& other);

    private:
        void* data = nullptr;
        void* deleter = nullptr;
        HandleFunctionPointer handle = nullptr;

        // perform action
        void* call(Action action, UniqueAnyPointer* other = nullptr) const;

        template<typename Type, typename Deleter> friend
        class any_implementation::UniqueAnyPointerHandle;
    };

    namespace any_implementation
    {
        template<typename Type, typename Deleter>
        struct UniqueAnyPointerHandle
        {
            static void* handle(Action action, UniqueAnyPointer const* this_, UniqueAnyPointer* other)
            {
                switch (action)
                {
                    case Action::Move:
                    {
                        // second parameter is destination
                        move(const_cast<UniqueAnyPointer&>(*this_), *other);
                        return nullptr;
                    }
                    case Action::Destroy:
                    {
                        destroy(const_cast<UniqueAnyPointer&>(*this_));
                        return nullptr;
                    }
                    case Action::Get:
                    {
                        return get(const_cast<UniqueAnyPointer&>(*this_));
                    }
                    case Action::TypeId:
                    {
                        return typeId();
                    }
                }
            }

            template<typename... Args>
            static void create(UniqueAnyPointer& destination, Args&& ... args)
            {
                destination.handle = &UniqueAnyPointerHandle::handle;
            }

            static void destroy(UniqueAnyPointer& this_)
            {
                auto* data_ = static_cast<Type*>(this_.data);
                auto& deleter = *static_cast<Deleter*>(this_.deleter);
                deleter(data_);
            }

            static void move(UniqueAnyPointer& this_, UniqueAnyPointer& destination)
            {
                destination.data = this_.data;
                destination.handle = &UniqueAnyPointerHandle::handle;
                this_.handle = nullptr;
            }

            [[nodiscard]] static void* get(UniqueAnyPointer& this_)
            {
                return this_.data;
            }

            [[nodiscard]] static void* typeId()
            {
                return const_cast<TypeId*>(&TypeIndex<Type>::value());
            }
        };
    }

    // this uses the default deleter
    template<typename Type, typename... Args>
    [[nodiscard]] UniqueAnyPointer makeUniqueAny(Args&& ... args)
    {
        Type* pointer = new Type(std::forward<Args>(args)...);
        return UniqueAnyPointer(pointer, std::default_delete<Type>());
    }
}

#endif //SHAPEREALITY_UNIQUE_ANY_POINTER_H
