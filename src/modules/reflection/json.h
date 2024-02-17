//
// Created by Arjo Nagelhout on 14/02/2024.
//

#ifndef SHAPEREALITY_JSON_H
#define SHAPEREALITY_JSON_H

#include <reflection/type_info.h>

#include <nlohmann/json.hpp>

namespace reflection::json
{
    // serialization from and to json using type info

    //-----------------------------------------------------
    // Converter
    //-----------------------------------------------------

    struct ConvertFunctions
    {
        std::function<void(nlohmann::json const&, std::any)> fromJson;
        std::function<void(std::any const&, nlohmann::json&)> toJson;
    };

    // a simple registry that contains two-way json conversion functions
    // for any given type. this enables the user to register any
    // arbitrary type and give it a custom conversion function
    class Converter final
    {
    public:
        explicit Converter();

        template<typename Type>
        void emplace(ConvertFunctions&& converter)
        {
            type_id typeId = TypeIndex<Type>::value();
            assert(!functions.contains(typeId) && "already registered converter functions for type");
            functions.emplace(typeId, converter);
        }

        // returns whether conversion was successful (i.e. functions contains the provided typeId)
        [[nodiscard]] bool fromJson(type_id typeId, nlohmann::json const& in, std::any out);

        // returns whether conversion was successful (i.e. functions contains the provided typeId)
        [[nodiscard]] bool toJson(type_id typeId, std::any const& in, nlohmann::json& out);

    private:
        std::unordered_map<type_id, ConvertFunctions> functions;
    };

    //-----------------------------------------------------
    // From JSON
    //-----------------------------------------------------

    void objectFromJson(TypeInfoRegistry& r,
                        Converter& converter,
                        nlohmann::json const& in,
                        std::any out,
                        type_id typeId);

    void nodeFromJson(TypeInfoRegistry& r,
                      Converter& converter,
                      nlohmann::json const& in,
                      std::any out,
                      TypeInfo& info,
                      size_t nodeIndex);

    template<typename Type>
    void fromJson(TypeInfoRegistry& r,
                  Converter& converter,
                  nlohmann::json const& in,
                  Type& out)
    {
        objectFromJson(r, converter, in, &out, TypeIndex<Type>::value());
    }

    template<typename Type>
    Type fromJson(TypeInfoRegistry& r,
                  Converter& converter,
                  std::string const& in)
    {
        nlohmann::json parsed = nlohmann::json::parse(in);
        Type out;
        fromJson(r, converter, parsed, out);
        return out;
    }

    //-----------------------------------------------------
    // To JSON
    //-----------------------------------------------------

    void objectToJson(TypeInfoRegistry& r,
                      Converter& converter,
                      std::any in,
                      nlohmann::json& out,
                      type_id typeId);

    void nodeToJson(TypeInfoRegistry& r,
                    Converter& converter,
                    std::any in,
                    nlohmann::json& out,
                    TypeInfo& info,
                    size_t nodeIndex);

    template<typename Type>
    nlohmann::json toJson(TypeInfoRegistry& r,
                          Converter& converter,
                          Type& in)
    {
        nlohmann::json out = nlohmann::json::object();
        objectToJson(r, converter, &in, out, TypeIndex<Type>::value());
        return out;
    }
}

#endif //SHAPEREALITY_JSON_H
