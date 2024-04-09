//
// Created by Arjo Nagelhout on 08/04/2024.
//

#ifndef SHAPEREALITY_TYPE_INFO_REGISTRY_H
#define SHAPEREALITY_TYPE_INFO_REGISTRY_H

#include <reflection/type_id.h>
#include <utility>
#include <any>
#include <unordered_map>

namespace reflection
{
    class TypeInfo;

    /**
     *
     */
    class TypeInfoRegistry final
    {
    public:
        void emplace(std::unique_ptr<TypeInfo>&& info, TypeId typeId);

        template<typename Type>
        void emplace(std::unique_ptr<TypeInfo>&& info)
        {
            TypeId typeId = TypeIndex<Type>::value();
            emplace(std::move(info), typeId);
        }

        [[nodiscard]] bool contains(TypeId typeId) const;

        template<typename Type>
        [[nodiscard]] bool contains() const
        {
            TypeId typeId = TypeIndex<Type>::value();
            return contains(typeId);
        }

        [[nodiscard]] TypeInfo* get(TypeId typeId);

        template<typename Type>
        [[nodiscard]] TypeInfo* get()
        {
            TypeId typeId = TypeIndex<Type>::value();
            return get(typeId);
        }

        [[nodiscard]] TypeId getChildType(std::any value, TypeId baseTypeId);

        template<typename Type>
        [[nodiscard]] TypeId getChildType(Type* value)
        {
            TypeId typeId = TypeIndex<Type>::value();
            return getChildType(value, typeId);
        }

    private:
        std::unordered_map<TypeId, std::unique_ptr<TypeInfo>> types;

        explicit TypeInfoRegistry();

        friend class Reflection;
    };
}

#endif //SHAPEREALITY_TYPE_INFO_REGISTRY_H
