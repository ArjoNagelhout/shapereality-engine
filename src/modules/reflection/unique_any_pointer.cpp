//
// Created by Arjo Nagelhout on 22/04/2024.
//

#include "unique_any_pointer.h"

namespace reflection
{
    UniqueAnyPointer::UniqueAnyPointer() = default;

    UniqueAnyPointer::UniqueAnyPointer(nullptr_t) {}

    UniqueAnyPointer::UniqueAnyPointer(UniqueAnyPointer&& other) noexcept
    {
        if (other.handle)
        {
            other.handle(Action::Move, &other, this);
        }
    }

    UniqueAnyPointer::~UniqueAnyPointer()
    {
        reset();
    }

    bool UniqueAnyPointer::isType(TypeId typeId_) const
    {
        return typeId() == typeId_;
    }

    TypeId UniqueAnyPointer::typeId() const
    {
        if (handle)
        {
            return *static_cast<TypeId*>(call(Action::TypeId));
        }
        return nullTypeId;
    }

    void UniqueAnyPointer::reset()
    {
        if (data)
        {
            call(Action::Destroy, this);
        }
    }

    void UniqueAnyPointer::swap(UniqueAnyPointer& other)
    {
        if (this == &other)
        {
            return; // don't need to do anything
        }

        if (handle && other.handle)
        {
            UniqueAnyPointer temporary;
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

    void* UniqueAnyPointer::call(Action action, UniqueAnyPointer* other) const
    {
        if (handle)
        {
            return handle(action, this, other);
        }
        return nullptr;
    }
}