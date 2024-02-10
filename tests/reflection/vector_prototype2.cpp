//
// Created by Arjo Nagelhout on 09/02/2024.
//

#include <gtest/gtest.h>
#include <reflection/type_info.h>

#include <vector>

using namespace reflection;

namespace vector_prototype2
{
    // we want to serialize and deserialize vectors.
    // a vector should not have to be separately registered, just as a property
    // registering the property gives us the type std::vector<certain type>, so
    // can write some template that registers that it is a vector<>
    // let's first test if we can detect it being a vector

    struct Complex
    {
        bool a = true;
        bool b = false;
        bool c = true;
        bool d = false;
    };

    struct Data
    {
        std::vector<bool> a{
            false, true, false, true, true, false
        };
        std::vector<Complex> b{
            {
                .a = false,
                .b = false,
                .c = false,
                .d = false
            },
            {},
            {},
            {},
            {
                .a = true,
                .b = true,
                .c = true,
                .d = true
            }
        };
    };

    void setup(TypeInfoRegistry& r)
    {
        r.registerType<bool>({.name = "bool"});

        r.registerType<std::vector<bool>>({"vector<bool>"});
        r.registerType<std::vector<Complex>>({"vector<Complex>"});

        TypeInfoBuilder<Complex>("Complex")
            .addProperty<&Complex::a>("a")
            .addProperty<&Complex::b>("b")
            .addProperty<&Complex::c>("c")
            .addProperty<&Complex::d>("d")
            .registerType(r);

        TypeInfoBuilder<Data>("Data")
            .addProperty<&Data::a>("a")
            .addProperty<&Data::b>("b")
            .registerType(r);
    }

    TEST(Reflection, VectorPrototype2)
    {
        TypeInfoRegistry r;
        setup(r);

        Data d;

        auto callback = [&](StackFrame const& f) -> bool {

            std::cout << f.name << " (" << f.typeInfo->name << ")" << std::endl;

            if (isType<bool>(f.typeId))
            {
                std::cout << (*std::any_cast<bool*>(f.value) ? "true" : "false") << std::endl;
            }
            return true;
        };

        auto onPop = [&](StackFrame const& f) {

        };

        bool someValue = true;

        //iterateUsingStack(r, "someValue", &someValue, callback, onPop);

        iterateUsingStack(r, "root", &d, callback, onPop);
    }

    struct Data2
    {
        bool normalProperty = false;
        std::vector<bool> listProperty;
        std::unordered_map<std::string, bool> dictionaryProperty;
    };

    template<typename>
    struct is_list : std::false_type
    {
    };

    template<typename T>
    struct is_list<std::vector<T>> : std::true_type
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

    template<typename Type, auto Data>
    void addProperty()
    {
        using value_type = std::remove_reference_t<decltype(std::declval<Type>().*Data)>;

        if constexpr (is_list<value_type>::value)
        {
            std::cout << "is vector" << std::endl;
        }
        else if constexpr (is_dictionary<value_type>::value)
        {
            std::cout << "is dictionary" << std::endl;
        }
        else
        {
            std::cout << "is some other type" << std::endl;
        }
    }

    TEST(Reflection, VectorPrototype2_2)
    {
        addProperty<Data2, &Data2::listProperty>();
        addProperty<Data2, &Data2::normalProperty>();
        addProperty<Data2, &Data2::dictionaryProperty>();
    }
}