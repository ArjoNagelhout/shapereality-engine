//
// Created by Arjo Nagelhout on 19/02/2024.
//

#ifndef SHAPEREALITY_BINARY_H
#define SHAPEREALITY_BINARY_H

#include <reflection/type_info.h>
#include <cassert>

namespace reflection::binary
{
    // serializes the type_info into binary
    // this needs custom work because we need to store the lengths
    // of lists and keys of dictionaries

    // we could also choose to only support serializing very simple
    // data structures to binary (i.e. no support for lists or dictionaries)
    // however, that could be confusing

    // note that a lot of this code is a "duplicate" of json.h. However, trying to
    // make recursively iterating over a tree data structure generic, where the
    // caller also needs to maintain a stack of data, becomes more unreadable
    // and hard to reason about than simply copying the behaviour.

    //-----------------------------------------------------
    // Converter
    //-----------------------------------------------------

    struct ConvertFunctions
    {
        std::function<void(void*, std::any)> fromBinary;
        std::function<void(std::any const&, void*)> toBinary;
    };

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
        [[nodiscard]] bool fromBinary(type_id typeId, void* in, std::any out);

        // returns whether conversion was successful (i.e. functions contains the provided typeId)
        [[nodiscard]] bool toBinary(type_id typeId, std::any const& in, void* out);

    private:
        std::unordered_map<type_id, ConvertFunctions> functions;
    };

    //-----------------------------------------------------
    // From binary
    //-----------------------------------------------------

    void objectFromBinary(TypeInfoRegistry& r,
                          Converter& converter,
                          void* in,
                          std::any out,
                          type_id typeId);

    void nodeFromBinary(TypeInfoRegistry& r,
                        Converter& converter,
                        void* const& in,
                        std::any out,
                        TypeInfo& info,
                        size_t nodeIndex);

    template<typename Type>
    void fromBinary(TypeInfoRegistry& r,
                    Converter& converter,
                    void* in,
                    Type& out)
    {
        objectFromBinary(r, converter, in, &out, TypeIndex<Type>::value());
    }

    template<typename Type>
    Type fromBinary(TypeInfoRegistry& r,
                    Converter& converter,
                    std::string const& in)
    {
        void* parsed = (void*)&in;
        Type out;
        fromBinary(r, converter, parsed, out);
        return out;
    }

    //-----------------------------------------------------
    // To binary
    //-----------------------------------------------------

    void objectToBinary(TypeInfoRegistry& r,
                        Converter& converter,
                        std::any in,
                        void* out,
                        type_id typeId);

    void nodeToBinary(TypeInfoRegistry& r,
                      Converter& converter,
                      std::any in,
                      void* out,
                      TypeInfo& info,
                      size_t nodeIndex);

    template<typename Type>
    void* toBinary(TypeInfoRegistry& r,
                   Converter& converter,
                   Type& in)
    {
        void* out;
        objectToBinary(r, converter, &in, out, TypeIndex<Type>::value());
        return out;
    }
}

#endif //SHAPEREALITY_BINARY_H
