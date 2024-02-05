//
// Created by Arjo Nagelhout on 05/02/2024.
//

#include <gtest/gtest.h>

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
    void (*set)(void* instance, void* value){};
    void* (*get)(void* instance){};
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

    std::cout << "data1: " << *data1 << std::endl;
}