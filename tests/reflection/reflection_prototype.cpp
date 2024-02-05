//
// Created by Arjo Nagelhout on 05/02/2024.
//

#include <gtest/gtest.h>

namespace prototype1
{
    // stored as pointer to function in property info
    // Data is the pointer to member variable
    template<typename Type, auto Data>
    void set(void* instance, void* value)
    {

    }

    // stored as pointer to function in property info
    // Data is the pointer to member variable
    template<typename Type, auto Data>
    void* get(void* instance)
    {
        //std::invoke(Data)
        return nullptr;
    }

    // info
    struct PropertyInfo
    {
        void (* set)(void* instance, void* value){};

        void* (* get)(void* instance){};
    };

    // wrapper that casts the void pointer to the required type
    template<typename Type, typename Data>
    Data* getProperty(Type instance, PropertyInfo info)
    {
        void* ref = static_cast<void*>(&instance);
        void* value = info.get(ref);
        return static_cast<Data*>(value);
    }

    // wrapper that casts type to the void pointer
    template<typename Type>
    void setProperty(Type instance, PropertyInfo info)
    {

    }

    struct SimpleData
    {
        float data1;
        int data2;
        bool data3;
    };

    TEST(Reflection, Prototype)
    {
        SimpleData simpleData{
            .data1 = 1.0f,
            .data2 = 12,
            .data3 = true
        };

        PropertyInfo data1Info{
            .set = &set<float, &SimpleData::data1>,
            .get = &get<float, &SimpleData::data1>
        };

        float* data1 = getProperty<SimpleData, float>(simpleData, data1Info);

//        std::cout << "data1: " << *data1 << std::endl;
    }
}

namespace prototype2
{
    struct SimpleData
    {
        float value1;
        int value2;
        bool value3;
    };

    void function(int parameter)
    {
        std::cout << "parameter: " << parameter << std::endl;
    }

    template<typename Type>
    void function2(Type type)
    {
        std::cout << "function2: " << type << std::endl;
    }

    std::any getter()
    {
        return std::string("return value");
    }

    template<typename ValueType>
    ValueType get(std::any (* getter)())
    {
        return std::any_cast<ValueType>(getter());
    }

    TEST(Reflection, Prototype2)
    {
        SimpleData d{
            .value1 = 3.0f,
            .value2 = 32,
            .value3 = true,
        };

        float SimpleData::* v1 = &SimpleData::value1;

        ASSERT_EQ(d.value1, 3.0f);
        d.*v1 = 12.0f;
        ASSERT_EQ(d.value1, 12.0f);

        function(32);
        void (* a)(int) = &function; // function pointer
        a(16);

        function2(32);
        function2("something");
        void (* f2_int)(int) = &function2;
        void (* f2_string)(std::string) = &function2;

        f2_int(1);
        f2_string("f2_string");

        std::any (* g)() = &getter;
        auto v = get<std::string>(g);

        std::cout << "v: " << v << std::endl;


    }
}

namespace prototype3
{
    struct SimpleData
    {
        float value1;
        int value2;
        bool value3;
    };

    // Data is the pointer to member variable
    // the type of getter should be a function pointer with
    // no specific types: std::any(*)(std::any)
    template<typename Type, auto Data>
    std::any getter(std::any const& instance)
    {
        auto castInstance = std::any_cast<Type>(instance);
        return std::invoke(Data, castInstance);
    }

    // Type is the containing type that contains the property
    template<typename ValueType>
    ValueType get(std::any (* getter)(std::any const&), std::any const& instance)
    {
        return std::any_cast<ValueType>(getter(instance));
    }

    TEST(Reflection, Prototype3)
    {
        SimpleData d1{
            .value1 = 3.0f,
            .value2 = 32,
            .value3 = true,
        };

        SimpleData d2{
            .value1 = 123.0f,
            .value2 = 10,
            .value3 = false
        };

        std::any (* value1Getter)(std::any const&) = getter<SimpleData, &SimpleData::value1>;

        auto d1_value1 = get<float>(value1Getter, d1);
        auto d2_value1 = get<float>(value1Getter, d2);

        std::cout << "d_value1: " << d1_value1 << std::endl;
        std::cout << "d2_value1: " << d2_value1 << std::endl;
    }
}