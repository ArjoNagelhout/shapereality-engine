//
// Created by Arjo Nagelhout on 22/04/2024.
//

#include "any_unique_ptr.h"

#include <cassert>

namespace reflection
{
    AnyDeleter::AnyDeleter() = default;

    AnyDeleter::~AnyDeleter()
    {
        reset();
    }

    AnyDeleter::AnyDeleter(AnyDeleter&& other) noexcept
    {
        handle = other.handle;
        deleter = other.deleter;
        other.handle = nullptr;
        other.deleter = nullptr;
    }

    AnyDeleter& AnyDeleter::operator=(AnyDeleter&& other) noexcept
    {
        handle = other.handle;
        deleter = other.deleter;
        other.handle = nullptr;
        other.deleter = nullptr;
        return *this;
    }

    void AnyDeleter::operator()(void* data)
    {
        assert(handle && "handle should not be nullptr");
        assert(deleter && "deleter should not be nullptr");
        handle(Action::Delete, data, deleter);
    }

    bool AnyDeleter::valid() const
    {
        return deleter && handle;
    }

    void AnyDeleter::reset()
    {
        // don't need to reset if contains no deleter
        if (deleter)
        {
            assert(handle && "if deleter is not nullptr, handle should not be nullptr");
            handle(Action::DestroyDeleter, nullptr, deleter);
        }
    }

    AnyUniquePtr::AnyUniquePtr() = default;

    AnyUniquePtr::AnyUniquePtr(nullptr_t) {}

    AnyUniquePtr::AnyUniquePtr(AnyUniquePtr&& other) noexcept
    {
        assert(other.handle && "handle of other should not be nullptr");
        if (other.handle)
        {
            // this = destination
            other.handle(Action::Move, &other, this);
        }
    }

    AnyUniquePtr& AnyUniquePtr::operator=(AnyUniquePtr&& other) noexcept
    {
        assert(other.handle && "handle of other should not be nullptr");
        if (other.handle)
        {
            other.handle(Action::Move, &other, this);
        }
        return *this;
    }

    AnyUniquePtr::~AnyUniquePtr()
    {
        reset();
    }

    AnyUniquePtr& AnyUniquePtr::operator=(nullptr_t)
    {
        reset();
        return *this;
    }

    AnyUniquePtr::operator bool() const
    {
        return data;
    }

    TypeId AnyUniquePtr::typeId() const
    {
        if (handle)
        {
            return *static_cast<TypeId*>(call(Action::TypeId));
        }
        return nullTypeId;
    }

    bool AnyUniquePtr::isType(TypeId typeId_) const
    {
        return typeId() == typeId_;
    }

    void* AnyUniquePtr::get() const
    {
        return data;
    }

    void AnyUniquePtr::reset()
    {
        if (handle && deleter.valid())
        {
            call(Action::Destroy, this);
        }
    }

    void AnyUniquePtr::swap(AnyUniquePtr& other)
    {
        if (this == &other)
        {
            return; // don't need to do anything
        }

        if (handle && other.handle)
        {
            AnyUniquePtr temporary;
            other.call(Action::Move, &temporary);
            call(Action::Move, &other);
            temporary.call(Action::Move, this);
        }
        else if (handle)
        {
            call(Action::Move, &other);
        }
        else if (other.handle)
        {
            other.call(Action::Move, this);
        }
    }

    bool AnyUniquePtr::empty() const
    {
        return !data;
    }

    void* AnyUniquePtr::release()
    {
        assert(data && "data should not be nullptr");
        void* temporary = data;
        data = nullptr;
        return temporary;
    }

    AnyDeleter AnyUniquePtr::releaseDeleter()
    {
        assert(deleter.valid() && "deleter should be valid");
        return std::move(deleter);
    }

    void* AnyUniquePtr::call(Action action, AnyUniquePtr* other) const
    {
        if (handle)
        {
            return handle(action, this, other);
        }
        return nullptr;
    }
}