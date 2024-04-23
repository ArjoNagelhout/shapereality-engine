//
// Created by Arjo Nagelhout on 12/04/2024.
//

#ifndef SHAPEREALITY_UNIQUE_ANY_POINTER_H
#define SHAPEREALITY_UNIQUE_ANY_POINTER_H

#include <memory>
#include <reflection/type_id.h>
#include <utility>
#include <cassert>

namespace reflection
{
    struct UniqueAnyPointer;

    namespace unique_any_implementation
    {
        enum class Action
        {
            Move,
            Destroy,
            Get,
            TypeId,
        };

        // forward declaration
        template<typename Type, typename Deleter>
        struct UniqueAnyPointerHandler;
    }

    namespace any_deleter_implementation
    {
        enum class Action
        {
            DestroyDeleter,
            Delete
        };

        template<typename Type, typename Deleter>
        struct AnyDeleterHandler
        {
            static void handle(Action action, void* data, void* deleter)
            {
                switch (action)
                {
                    case Action::DestroyDeleter:
                    {
                        destroy(deleter);
                        break;
                    }
                    case Action::Delete:
                    {
                        delete_(data, deleter);
                        break;
                    }
                }
            }

            static void destroy(void* deleter)
            {
                assert(deleter && "deleter should not be nullptr");
                auto* deleter_ = static_cast<Deleter*>(deleter);
                delete deleter_;
            }

            static void delete_(void* data, void* deleter)
            {
                assert(deleter && "deleter should not be nullptr");
                auto* data_ = static_cast<Type*>(data);
                auto& deleter_ = *static_cast<Deleter*>(deleter);
                if (data_)
                {
                    deleter_(data_);
                }
            }
        };

        template<typename Type, typename Deleter>
        struct Tag
        {
        };
    }

    /**
     * AnyDeleter wraps any deleter.
     * To be used in conjunction with UniqueAnyPointer
     *
     * Because AnyDeleter allocates the deleter on the heap,
     * AnyDeleter is not copyable.
     */
    struct AnyDeleter
    {
        using Action = any_deleter_implementation::Action;
        template<typename Type, typename Deleter> using Tag = any_deleter_implementation::Tag<Type, Deleter>;
        template<typename Type, typename Deleter> using Handler = any_deleter_implementation::AnyDeleterHandler<Type, Deleter>;
        using HandleFunctionPointer = void (*)(Action action, void* data, void* deleter);

        // empty any deleter
        explicit AnyDeleter();

        // default construct deleter
        template<typename Type, typename Deleter = std::default_delete<Type>>
        requires std::is_default_constructible_v<Deleter>
        explicit AnyDeleter(Tag<Type, Deleter>)
        {
            deleter = new Deleter();
            handle = &Handler<Type, Deleter>::handle;
        }

        // move deleter
        template<typename Type, typename Deleter>
        requires std::is_move_constructible_v<Deleter>
        explicit AnyDeleter(Tag<Type, Deleter>, Deleter&& deleter_)
        {
            deleter = new Deleter(deleter_);
            handle = &Handler<Type, Deleter>::handle;
        }

        // copy deleter
        template<typename Type, typename Deleter>
        requires std::is_copy_constructible_v<Deleter>
        explicit AnyDeleter(Tag<Type, Deleter>, Deleter const& deleter_)
        {
            deleter = new Deleter(deleter_);
            handle = &Handler<Type, Deleter>::handle;
        }

        // calls reset
        ~AnyDeleter();

        // move constructor
        AnyDeleter(AnyDeleter&& other) noexcept;

        // move assignment operator
        AnyDeleter& operator=(AnyDeleter&& other) noexcept;

        // copy constructor
        AnyDeleter(AnyDeleter const&) = delete;

        // copy assignment operator
        AnyDeleter& operator=(AnyDeleter const&) = delete;

        // perform delete
        void operator()(void* data);

        [[nodiscard]] bool valid() const;

        void reset();

    private:
        void* deleter = nullptr;
        HandleFunctionPointer handle = nullptr;
    };

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
        using Action = unique_any_implementation::Action;
        template<typename Type, typename Deleter> using Tag = any_deleter_implementation::Tag<Type, Deleter>;
        template<typename Type, typename Deleter> using Handler = unique_any_implementation::UniqueAnyPointerHandler<Type, Deleter>;
        using HandleFunctionPointer = void* (*)(Action action, UniqueAnyPointer const* this_, UniqueAnyPointer* other);

        // construct empty pointer
        explicit UniqueAnyPointer();

        // construct empty pointer
        explicit UniqueAnyPointer(nullptr_t);

        // construct from pointer, default constructible deleter
        template<typename Type, typename Deleter = std::default_delete<Type>>
        requires std::is_default_constructible_v<Deleter>
        explicit UniqueAnyPointer(Type* data_)
            : data(data_), deleter(Tag<Type, Deleter>())
        {
            handle = &Handler<Type, Deleter>::handle;
        }

        // construct from pointer, movable deleter
        template<typename Type, typename Deleter>
        requires std::is_move_constructible_v<Deleter>
        explicit UniqueAnyPointer(Type* data_, Deleter&& deleter_)
            : data(data_), deleter(Tag<Type, Deleter>(), std::forward<Deleter>(deleter_))
        {
            handle = &Handler<Type, Deleter>::handle;
        }

        // construct from pointer, copy deleter
        template<typename Type, typename Deleter>
        requires std::is_copy_constructible_v<Deleter>
        explicit UniqueAnyPointer(Type* data_, Deleter const& deleter_)
            : data(data_), deleter(Tag<Type, Deleter>(), std::forward<Deleter>(deleter_))
        {
            handle = &Handler<Type, Deleter>::handle;
        }

        // construct from unique pointer
        template<typename Type, typename Deleter>
        explicit UniqueAnyPointer(std::unique_ptr<Type, Deleter>&& other)
            : data(other.release()), deleter(Tag<Type, Deleter>(), std::forward<Deleter>(other.get_deleter()))
        {
            handle = &Handler<Type, Deleter>::handle;
        }

        // move constructor
        UniqueAnyPointer(UniqueAnyPointer&& other) noexcept;

        // destructor (calls reset)
        ~UniqueAnyPointer();

        // move assignment operator
        UniqueAnyPointer& operator=(UniqueAnyPointer&& other) noexcept;

        // delete copy constructor
        UniqueAnyPointer(UniqueAnyPointer const&) = delete;

        // delete copy assignment operator
        UniqueAnyPointer& operator=(UniqueAnyPointer const&) = delete;

        // assignment operator from unique_ptr
        template<typename Type, typename Deleter>
        UniqueAnyPointer& operator=(std::unique_ptr<Type, Deleter>&& other)
        {
            reset();
            data = other.release();
            deleter = AnyDeleter(Tag<Type, Deleter>(), std::forward<Deleter>(other.get_deleter()));
            handle = &Handler<Type, Deleter>::handle;
            return *this;
        }

        // reset
        UniqueAnyPointer& operator=(nullptr_t);

        // implicit conversion to bool operator
        operator bool() const;

        // type

        [[nodiscard]] TypeId typeId() const;

        [[nodiscard]] bool isType(TypeId typeId) const;

        template<typename Type>
        [[nodiscard]] bool isType() const
        {
            return isType(TypeIndex<Type>::value());
        }

        // methods

        [[nodiscard]] void* get() const;

        template<typename Type>
        [[nodiscard]] Type* get()
        {
            assert(isType<Type>());
            return static_cast<Type*>(get());
        }

        template<typename Type>
        [[nodiscard]] Type const* get() const
        {
            assert(isType<Type>());
            return static_cast<Type const*>(get());
        }

        // reset the data and remove type information
        void reset();

        // swap this unique any and other unique any
        void swap(UniqueAnyPointer& other);

        // empty
        [[nodiscard]] bool empty() const;

        // release ownership

        // should always be used in conjunction with releaseDeleter(),
        // because the resource that is referenced by the pointer
        // might require a special deleter.
        [[nodiscard]] void* release();

        // should always be used in conjunction with releaseDeleter(),
        // because the resource that is referenced by the pointer
        // might require a special deleter.
        template<typename Type>
        [[nodiscard]] Type* release()
        {
            return static_cast<Type*>(release());
        }

        // because AnyDeleter is only movable, not copyable,
        // the caller is responsible for managing the deleter
        // after calling this method
        [[nodiscard]] AnyDeleter releaseDeleter();

    private:
        void* data = nullptr;
        AnyDeleter deleter;
        HandleFunctionPointer handle = nullptr;

        // handle constructor
        template<typename Type, typename Deleter>
        explicit UniqueAnyPointer() : handle(&Handler<Type, Deleter>::handle)
        {

        }

        // perform action
        void* call(Action action, UniqueAnyPointer* other = nullptr) const;

        template<typename Type, typename Deleter> friend
        class unique_any_implementation::UniqueAnyPointerHandler;
    };

    namespace unique_any_implementation
    {
        template<typename Type, typename Deleter>
        struct UniqueAnyPointerHandler
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

            static void destroy(UniqueAnyPointer& this_)
            {
                this_.deleter(this_.data);

                // destroy also removes type information
                this_.handle = nullptr;
                this_.data = nullptr;
                this_.deleter = AnyDeleter();
            }

            static void move(UniqueAnyPointer& this_, UniqueAnyPointer& destination)
            {
                destination.data = this_.data;
                destination.handle = &UniqueAnyPointerHandler::handle;
                destination.deleter = std::move(this_.deleter);

                this_.handle = nullptr;
                this_.data = nullptr;
                this_.deleter = AnyDeleter();
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
        return UniqueAnyPointer(pointer);
    }
}

namespace std
{
    template<>
    inline void swap<reflection::UniqueAnyPointer>(reflection::UniqueAnyPointer& lhs, reflection::UniqueAnyPointer& rhs) noexcept
    {
        lhs.swap(rhs);
    }
}

#endif //SHAPEREALITY_UNIQUE_ANY_POINTER_H
