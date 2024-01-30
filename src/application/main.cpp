//
// Created by Arjo Nagelhout on 22/12/2023.
//

#include <iostream>
#include <utility>

struct PropertyInfo;
struct MethodInfo;

using TypeId = size_t;

struct TypeInfo
{
    std::string name;
    std::vector<PropertyInfo> properties;
    std::vector<MethodInfo> methods;
};

// property
// method

struct PropertyInfo
{
    std::string name;
    TypeId type;
};

struct MethodInfo
{
    std::string name;
    TypeId returnType;
    std::vector<TypeId> parameters;
};

std::unordered_map<TypeId, TypeInfo> registry;


struct Builder
{
    explicit Builder(std::string typeName) : typeName(std::move(typeName)) {}

    ~Builder() = default;

    Builder& addMethod(MethodInfo&& methodInfo)
    {
        methods.emplace_back(methodInfo);
        return *this;
    }

    Builder& addProperty(PropertyInfo&& propertyInfo)
    {
        properties.emplace_back(propertyInfo);
        return *this;
    }

    TypeId bake()
    {
        TypeId id = 0;
        registry.emplace(id, TypeInfo{
            .name = typeName,
            .properties = properties,
            .methods = methods,
        });
        return id;
    }

private:
    std::string typeName;
    std::vector<MethodInfo> methods;
    std::vector<PropertyInfo> properties;
};

int main(int argc, char* argv[])
{
    std::cout << "Editor" << std::endl;

    TypeId id = Builder("Editor")
        .addMethod(MethodInfo {
            .name = "Wauw"
        })
        .addProperty(PropertyInfo {
            .name = "leuk"
        })
        .bake();

    return 0;
}

struct PrimitiveType
{
    union
    {
        int intValue;
        void* objectValue;
        bool boolValue;
        float floatValue;
    };
    int tag = 0;
};

template<typename Type>
struct ReflectedType
{
    virtual PrimitiveType getValue(std::string const& propertyName) = 0;


};

struct MyType : public ReflectedType<MyType>
{
    PrimitiveType getValue(std::string const& propertyName) override
    {
        if (propertyName == "leuk")
        {
            return PrimitiveType { .floatValue = leuk, .tag = 3 };
        }
        else if (propertyName == "jaja")
        {
            return PrimitiveType { .boolValue = jaja, .tag = 2};
        }
        else
        {
            return PrimitiveType { .boolValue = nee, .tag = 2};
        }
    }

    float leuk = 10.f;
    bool jaja = false;
    bool nee = true;
};

struct Vector3
{

};

struct Ba
{
    Vector3 ooooh;
    bool wie_woont_er_diep_onder_de_zee;
};

struct Bie
{
    Ba ba;
    Ba ba1;
    Ba ba2;
    Ba baLast;
    Bie*********************** baFinal;
    Ba baFinalFinal;
};

struct Boe
{
    Bie ba;
    Bie bie;
};