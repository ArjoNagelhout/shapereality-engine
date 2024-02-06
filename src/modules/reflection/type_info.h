//
// Created by Arjo Nagelhout on 04/02/2024.
//

#ifndef SHAPEREALITY_TYPE_INFO_H
#define SHAPEREALITY_TYPE_INFO_H

#include <string>
#include <unordered_map>
#include <iostream>
#include <any>
#include <cassert>

#include "type_id.h"

/**
 * @namespace reflection
 * @brief simple reflection of plain-old datastructures (POD) or compositions of PODs
 */
namespace reflection
{
    // Data is the pointer to member variable
    // the type of getter should be a function pointer with
    // no specific types: std::any(*)(std::any)
    // Type is the containing type of the property
    template<typename Type, auto Data>
    std::any getter(std::any const& instance)
    {
        auto castInstance = std::any_cast<Type>(instance);
        return std::invoke(Data, castInstance);
    }

    // instance is a pointer, as we can't have references to std::any
    template<typename Type, auto Data>
    void setter(std::any instance, std::any value)
    {
        using value_type = std::remove_reference_t<decltype(std::declval<Type>().*Data)>;

        auto castInstance = std::any_cast<Type*>(instance);
        std::invoke(Data, castInstance) = std::any_cast<value_type>(value);
    }

    struct PropertyInfo
    {
        std::string name;
        type_id typeId;

        std::any (* getter)(std::any const&);

        void (* setter)(std::any, std::any);

        template<typename ValueType>
        ValueType get(std::any const& instance)
        {
            return std::any_cast<ValueType>(getter(instance));
        }

        template<typename Type, typename ValueType>
        void set(Type& instance, ValueType value)
        {
            setter(&instance, value);
        }
    };

    struct TypeInfo
    {
        std::string name;
        std::vector<PropertyInfo> properties;
    };

    struct PrimitiveInfo
    {
        std::string name;
    };

    struct TypeInfoRegistry;

    template<typename Type>
    class TypeInfoBuilder
    {
    public:
        explicit TypeInfoBuilder(std::string name)
        {
            typeInfo.name = std::move(name);
        }

        template<auto Data>
        TypeInfoBuilder& addProperty(std::string const& name)
        {
            // we assume the property is non-const and non-volatile
            using value_type = std::remove_reference_t<decltype(std::declval<Type>().*Data)>;

            type_id id = TypeIndex<value_type>().value();
            typeInfo.properties.emplace_back(PropertyInfo{
                .name = name,
                .typeId = id,
                .getter = getter<Type, Data>,
                .setter = setter<Type, Data>
            });

            return *this;
        }

        [[nodiscard]] TypeInfo build() const
        {
            return typeInfo;
        }

        // registers the type to the provided `TypeInfoRegistry`
        void registerType(TypeInfoRegistry& r);

    private:
        TypeInfo typeInfo;
    };

    template<typename Type>
    [[nodiscard]] bool isType(type_id id)
    {
        return id == TypeIndex<Type>::value();
    }

    // contains a map of type ids to info's
    class TypeInfoRegistry
    {
    public:
        template<typename Type>
        void registerType(TypeInfo&& info)
        {
            type_id id = TypeIndex<Type>().value();
            assert(!types.contains(id) && "error: type was already registered");
            types[id] = std::move(info);
        }

        template<typename Type>
        void registerPrimitive(PrimitiveInfo&& info)
        {
            type_id id = TypeIndex<Type>().value();
            assert(!primitives.contains(id) && "error: primitive was already registered");
            primitives[id] = std::move(info);
        }

        // returns nullptr if type was not registered (i.e. unknown type)
        [[nodiscard]] TypeInfo* getTypeInfo(type_id id)
        {
            if (types.contains(id))
            {
                return &types[id];
            }
            else
            {
                return nullptr;
            }
        }

        template<typename Type>
        [[nodiscard]] TypeInfo* getTypeInfo()
        {
            type_id id = TypeIndex<Type>().value();
            return getTypeInfo(id);
        }

        [[nodiscard]] PrimitiveInfo* getPrimitiveInfo(type_id id)
        {
            if (primitives.contains(id))
            {
                return &primitives[id];
            }
            else
            {
                return nullptr;
            }
        }

        template<typename Type>
        [[nodiscard]] PrimitiveInfo* getPrimitiveInfo()
        {
            type_id id = TypeIndex<Type>().value();
            return getPrimitiveInfo(id);
        }

        [[nodiscard]] bool isPrimitive(type_id id)
        {
            return primitives.contains(id);
        }

    private:
        std::unordered_map<type_id, TypeInfo> types;
        std::unordered_map<type_id, PrimitiveInfo> primitives;
    };

    template<typename Type>
    void TypeInfoBuilder<Type>::registerType(TypeInfoRegistry& r)
    {
        r.registerType<Type>(std::move(typeInfo));
    }
}

#endif //SHAPEREALITY_TYPE_INFO_H
