//
// Created by Arjo Nagelhout on 14/02/2024.
//

#ifndef SHAPEREALITY_JSON_H
#define SHAPEREALITY_JSON_H

#include "reflection/type_info.h"

#include "nlohmann/json.hpp"

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
        struct Functions
        {
            std::function<
            void(nlohmann::json const&, std::any)> from;
            std::function<void(std::any const&, nlohmann::json&)> to;
        };

        explicit JsonSerializer(TypeInfoRegistry& r);

        ~JsonSerializer();

        template<typename Type>
        void emplace(Functions&& f)
        {
            type_id typeId = TypeIndex<Type>::value();
            assert(!functions.contains(typeId) && "already registered functions for type");
            functions.emplace(typeId, f);
        }

        template<typename Type>
        void fromJson(nlohmann::json const& in, Type& out)
        {
            objectFromJson(in, &out, TypeIndex<Type>::value());
        }

        template<typename Type>
        Type fromJson(std::string const& in)
        {
            nlohmann::json parsed = nlohmann::json::parse(in);
            Type out;
            fromJson(parsed, out);
            return out;
        }

        template<typename Type>
        nlohmann::json toJson(Type& in)
        {
            nlohmann::json out = nlohmann::json::object();
            objectToJson(&in, out, TypeIndex<Type>::value());
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
        std::unordered_map<type_id, Functions> functions;

        void objectFromJson(nlohmann::json const& in, std::any out, type_id typeId);

        void nodeFromJson(nlohmann::json const& in, std::any out, TypeInfo& info, size_t nodeIndex);

        void objectToJson(std::any in, nlohmann::json& out, type_id typeId);

        void nodeToJson(std::any in, nlohmann::json& out, TypeInfo& info, size_t nodeIndex);
    };
}

#endif //SHAPEREALITY_JSON_H
