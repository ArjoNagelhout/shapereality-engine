//
// Created by Arjo Nagelhout on 14/02/2024.
//

#ifndef SHAPEREALITY_JSON_H
#define SHAPEREALITY_JSON_H

#include <reflection/type_info.h>
#include <nlohmann/json.hpp>
#include <cassert>

namespace reflection
{
    /**
     * JsonSerializer enables serializing types that have been registered
     * to a TypeInfoRegistry, to and from json (nlohmann/json.hpp)
     *
     * Primitive types (ones that have not been registered in the TypeInfoRegistry
     * but which should be serialized) can be registered by emplacing a `from`
     * and `to` function for a given type.
     *
     * Built-in primitive types don't have to be manually added. These include:
     * - bool
     * - int
     * - float
     * - double
     * - std::string
     *
     * It is not supported to have multiple TypeInfoRegistries per JsonSerializer
     */
    class JsonSerializer final
    {
    public:
        // std::any in these parameters always is a pointer to the data (it can't be nullptr)
        struct Functions
        {
            std::function<void(nlohmann::json const&, std::any)> from;
            std::function<void(std::any, nlohmann::json&)> to;
        };

        template<typename Type>
        void emplace(Functions&& f)
        {
            assert(r.contains<Type>() && "Type is not registered yet");

            TypeId typeId = TypeIndex<Type>::value();
            assert(!functions.contains(typeId) && "already registered functions for type");
            functions.emplace(typeId, f);
        }

        // convenience function for registering serialization functions without having to
        // do the any_cast yourself.
        template<typename Type>
        void emplace(std::function<void(nlohmann::json const&, Type*)> from,
                     std::function<void(Type*, nlohmann::json&)> to)
        {
            emplace<Type>(Functions{
                .from = [=](nlohmann::json const& in, std::any out) {
                    from(in, std::any_cast<Type*>(out));
                }, .to = [=](std::any in, nlohmann::json& out) {
                    to(std::any_cast<Type*>(in), out);
                }}
            );
        }

        template<typename Type>
        void fromJson(nlohmann::json const& in, Type& out)
        {
            typeFromJson(in, &out, TypeIndex<Type>::value());
        }

        // named differently because nlohmann::json objects get
        // implicitly converted to std::string
        template<typename Type>
        Type fromJsonString(std::string const& in)
        {
            nlohmann::json parsed = nlohmann::json::parse(in);
            Type out;
            fromJson(parsed, out);
            return out;
        }

        template<typename Type>
        Type fromJson(nlohmann::json const& in)
        {
            Type out;
            fromJson(in, out);
            return out;
        }

        template<typename Type>
        nlohmann::json toJson(Type& in)
        {
            nlohmann::json out = nlohmann::json::object();
            typeToJson(&in, out, TypeIndex<Type>::value());
            return out;
        }

        template<typename Type>
        std::string toJsonString(Type& in, int const indentationAmount = -1, char const indentCharacter = ' ')
        {
            nlohmann::json out = toJson(in);
            return out.dump(indentationAmount, indentCharacter);
        }

    private:
        TypeInfoRegistry& r;
        std::unordered_map<TypeId, Functions> functions; // serialization from and to json functions for primitive types

        explicit JsonSerializer(TypeInfoRegistry& r);

        void typeFromJson(nlohmann::json const& in, std::any out, TypeId typeId);

        void propertyNodeFromJson(nlohmann::json const& in, std::any out, ClassInfo& info, size_t nodeIndex);

        void typeToJson(std::any in, nlohmann::json& out, TypeId typeId);

        void propertyNodeToJson(std::any in, nlohmann::json& out, ClassInfo& info, size_t nodeIndex);

        friend class Reflection;
    };
}

#endif //SHAPEREALITY_JSON_H
