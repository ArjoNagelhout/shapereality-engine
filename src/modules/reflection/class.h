//
// Created by Arjo Nagelhout on 08/04/2024.
//

#ifndef SHAPEREALITY_CLASS_H
#define SHAPEREALITY_CLASS_H

#include <reflection/type_info.h>

namespace reflection
{
    // helpers for detecting whether something is a std::vector or std::unordered_map
    template<typename>
    struct is_list : std::false_type
    {
    };

    template<typename Type>
    struct is_list<std::vector<Type>> : std::true_type
    {
    };

    template<typename>
    struct is_dictionary : std::false_type
    {
    };

    template<typename Key, typename Value>
    struct is_dictionary<std::unordered_map<Key, Value>> : std::true_type
    {
    };

    template<typename>
    struct is_pointer : std::false_type
    {
    };

    template<typename Type>
    struct is_pointer<std::unique_ptr<Type>> : std::true_type
    {
    };

    struct PropertyInfo final
    {
        std::string name; // name of property
        size_t node; // index to node of property (we don't use pointers so that TypeInfo is trivially copyable)

        std::any (* get)(std::any); // function pointer to get pointer to value of this property
    };

    struct TypePropertyNode final
    {
        TypeId typeId; // type
    };

    struct ListPropertyNode final
    {
        size_t valueNode; // index to TypeNode, Value of std::vector<Value>

        // first parameter (std::any) is the pointer to the std::vector we want to get the size of
        size_t (* size)(std::any); // pointer to function to get size of std::vector

        void (* resize)(std::any, size_t); // pointer to function to set size of std::vector

        std::any (* at)(std::any, size_t); // pointer to function to get value at index of std::vector
    };

    struct DictionaryPropertyNode final
    {
        TypeId keyTypeId; // Key of std::unordered_map<Key, Value>
        size_t valueNode; // index to TypeNode, Value of std::unordered_map

        using IterateCallback = std::function<void(std::string const&, std::any)>; // parameters: key, value

        void (* addKey)(std::any, std::string const&); // adds a key to the std::unordered_map

        void (* iterate)(std::any, IterateCallback const&);

        std::any (* at)(std::any, std::string const&); // get at key

        void (* clear)(std::any); // clear dictionary
    };

    struct PointerPropertyNode final
    {
        // we don't aim to support a pointer inside a pointer, or pointer to a dictionary as that would make no sense
        // therefore, we can directly store the type id
        TypeId valueTypeId;
    };

    struct PropertyNode final
    {
        enum class Type
        {
            Type, // a type, either an enum, primitive or class / struct
            List, // std::vector
            Dictionary, // std::unordered_map
            Pointer // std::unique_ptr
        };

        Type type;

        union
        {
            TypePropertyNode object;
            ListPropertyNode list;
            DictionaryPropertyNode dictionary;
            PointerPropertyNode pointer;
        };
    };

    struct ClassInfo final : public TypeInfo
    {
        explicit ClassInfo(std::string name);

        TypeId base = nullTypeId; // base class
        std::vector<TypeId> children; // child classes
        std::vector<PropertyNode> nodes; // container that owns all nodes that describe this type
        std::vector<PropertyInfo> properties; // list of properties inside this type

        bool (* isType)(std::any) = nullptr; // whether the provided value of base type is this type

        std::any (* castBaseTypeToThisType)(std::any) = nullptr;
    };

    // Type = containing type
    // Data = pointer to member variable
    template<typename Type, auto Data>
    std::any getProperty(std::any value)
    {
        auto* v = std::any_cast<Type*>(value);
        return &(std::invoke(Data, v));
    }

    // list

    template<typename Type>
    size_t listSize(std::any value)
    {
        auto* v = std::any_cast<Type*>(value);
        return v->size();
    }

    template<typename Type>
    void listResize(std::any value, size_t size)
    {
        auto* v = std::any_cast<Type*>(value);
        v->resize(size);
    }

    template<typename Type>
    std::any listAt(std::any value, size_t index)
    {
        auto* v = std::any_cast<Type*>(value);
        return &((*v)[index]);
    }

    // dictionary

    // converting keys for std::unordered_map
    template<typename Type>
    constexpr Type primitiveFromString(std::string const& string)
    {
        //@formatter:off
        if constexpr (std::is_same_v<Type, std::string>) { return string; }
        else if constexpr (std::is_same_v<Type, int>) { return std::stoi(string); }
        else if constexpr (std::is_same_v<Type, long>) { return std::stol(string); }
        else if constexpr (std::is_same_v<Type, unsigned long>) { return std::stoul(string); }
        else if constexpr (std::is_same_v<Type, long long>) { return std::stoll(string); }
        else if constexpr (std::is_same_v<Type, unsigned long long>) { return std::stoull(string); }
        else if constexpr (std::is_same_v<Type, float>) { return std::stof(string); }
        else if constexpr (std::is_same_v<Type, double>) { return std::stod(string); }
        else if constexpr (std::is_same_v<Type, long double>) { return std::stold(string); }
        //@formatter:on
    }

    template<typename Type>
    constexpr std::string primitiveToString(Type value)
    {
        if constexpr (std::is_same_v<Type, std::string>)
        {
            return value;
        }
        else
        {
            return std::to_string(value);
        }
    }

    template<typename Type>
    void dictionaryAddKey(std::any value, std::string const& key)
    {
        using mapped_type = Type::mapped_type;
        using key_type = Type::key_type;

        auto* v = std::any_cast<Type*>(value);
        auto k = primitiveFromString<key_type>(key);
        (*v)[k] = mapped_type{}; // default initialize for key
    }

    template<typename Type>
    void dictionaryIterate(std::any value, DictionaryPropertyNode::IterateCallback const& callback)
    {
        auto& v = *std::any_cast<Type*>(value);
        for (auto& [key, entryValue]: v)
        {
            // convert key to string
            std::string string = primitiveToString(key);
            callback(string, &entryValue);
        }
    }

    template<typename Type>
    std::any dictionaryAt(std::any value, std::string const& key)
    {
        using key_type = Type::key_type;

        auto* v = std::any_cast<Type*>(value);
        auto k = primitiveFromString<key_type>(key);

        return &((*v)[k]);
    }

    template<typename Type>
    void dictionaryClear(std::any value)
    {
        auto* v = std::any_cast<Type*>(value);
        v->clear();
    }

    //-----------------------------------------------------
    // Register property
    //-----------------------------------------------------

    template<typename Type>
    size_t addNode(ClassInfo& info)
    {
        PropertyNode node{};
        if constexpr (is_list<Type>::value)
        {
            node.type = PropertyNode::Type::List;
            node.list.valueNode = addNode<typename Type::value_type>(info);
            node.list.size = listSize<Type>;
            node.list.resize = listResize<Type>;
            node.list.at = listAt<Type>;
        }
        else if constexpr (is_dictionary<Type>::value)
        {
            node.type = PropertyNode::Type::Dictionary;
            node.dictionary.keyTypeId = TypeIndex<typename Type::key_type>::value();
            node.dictionary.valueNode = addNode<typename Type::mapped_type>(info);
            node.dictionary.addKey = dictionaryAddKey<Type>;
            node.dictionary.iterate = dictionaryIterate<Type>;
            node.dictionary.at = dictionaryAt<Type>;
            node.dictionary.clear = dictionaryClear<Type>;
        }
        else if constexpr (is_pointer<Type>::value)
        {
            node.type = PropertyNode::Type::Pointer;
            node.pointer.valueTypeId = TypeIndex<typename Type::element_type>::value();
        }
        else
        {
            node.type = PropertyNode::Type::Type;
            node.object.typeId = TypeIndex<Type>::value();
        }

        size_t index = info.nodes.size();
        info.nodes.emplace_back(node);
        return index;
    };

    //-----------------------------------------------------
    // Polymorphism
    //-----------------------------------------------------

    template<typename Type, typename BaseType>
    bool isType(std::any value)
    {
        auto** v = std::any_cast<BaseType*>(&value);
        assert(v);
        return dynamic_cast<Type*>(*v);
    }

    // should only be called if we know the downcast is possible
    template<typename Type, typename BaseType>
    std::any castBaseTypeToThisType(std::any value)
    {
        // don't need to downcast as it already is this type
        if (std::any_cast<Type*>(&value))
        {
            return value;
        }

        // otherwise, downcast
        auto** v = std::any_cast<BaseType*>(&value);
        assert(v);
        return dynamic_cast<Type*>(*v);
    }

    //-----------------------------------------------------
    // Builder
    //-----------------------------------------------------

    namespace register_
    {
        template<typename Type>
        class Class final
        {
        public:
            explicit Class(std::string name) : info(std::make_unique<ClassInfo>(std::move(name))) {}

            // Data is pointer to member variable
            template<auto Data>
            [[nodiscard]] Class& member(std::string name)
            {
                using property_type = std::remove_reference_t<decltype(std::declval<Type>().*Data)>;
                size_t root = addNode<property_type>(*info);

                info->properties.emplace_back(PropertyInfo{
                    .name = std::move(name),
                    .node = root,
                    .get = getProperty<Type, Data>
                });

                return *this;
            }

            template<typename BaseType>
            [[nodiscard]] Class& base()
            {
                info->base = TypeIndex<BaseType>::value();
                info->isType = isType<Type, BaseType>;
                info->castBaseTypeToThisType = castBaseTypeToThisType<Type, BaseType>;
                return *this;
            }

            void emplace(TypeRegistry& types)
            {
                types.emplace<Type>(std::move(info));
            }

        private:
            std::unique_ptr<ClassInfo> info;
        };
    }
}

#endif //SHAPEREALITY_CLASS_H
