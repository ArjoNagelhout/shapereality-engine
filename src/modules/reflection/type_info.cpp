//
// Created by Arjo Nagelhout on 07/02/2024.
//

#include "type_info.h"

#include <stack>

namespace reflection
{
    StackFrame createStackFrame(TypeInfoRegistry& r, PropertyInfo& property, std::any const& value)
    {
        StackFrame result{
            .name = property.name,
            .type = property.type,
        };
        switch (property.type)
        {
            case PropertyType::Object:
            {
                result.object.typeId = property.object.typeId;
                result.object.typeInfo = r.getTypeInfo(property.object.typeId);
                result.value = property.getter(value);
                break;
            }
            case PropertyType::List:
            {
                result.value = property.getter(value);
                result.list.propertyInfo = &property;
//                result.value = property.getter(value);
//                result.list.propertyInfo = &property;
                break;
            }
            case PropertyType::Dictionary:
            {
                break;
            }
        }
        return result;
    }

    void iterateUsingStack(TypeInfoRegistry& r,
                           type_id typeId,
                           std::string const& name,
                           std::any const& instance,
                           iterate_callback const& callback,
                           iterate_on_pop const& onPop)
    {
        std::stack<StackFrame> stack;
        stack.emplace(StackFrame{
            .name = name,
            .value = instance,
            .type = PropertyType::Object,
            .object = {
                .typeId = typeId,
                .typeInfo = r.getTypeInfo(typeId)
            }
        });

        while (!stack.empty())
        {
            StackFrame& top = stack.top();
            switch (top.type)
            {
                case PropertyType::Object:
                {
                    ObjectStackFrame& object = top.object;

                    bool recurse = (object.typeInfo && !object.typeInfo->properties.empty()); // don't recurse if no properties

                    if (object.typeInfo && object.index == 0)
                    {
                        recurse = callback(top); // if contains properties, but should not recurse, set this value
                    }

                    if (recurse && object.typeInfo && object.index < object.typeInfo->properties.size())
                    {
                        PropertyInfo& property = object.typeInfo->properties[object.index];
                        stack.emplace(createStackFrame(r, property, top.value));
                        object.index++;
                    }
                    else
                    {
                        // if reached the end, but was recursing, call the end
                        if (recurse)
                        {
                            onPop(top);
                        }
                        stack.pop();
                    }
                    break;
                }
                case PropertyType::List:
                {
                    std::cout << "we got a list property" << std::endl;
                    ListStackFrame& list = top.list;

                    // we should get the size
                    size_t size = list.propertyInfo->list.size(top.value);

                    std::cout << "size: " << size << std::endl;

                    //list.propertyInfo->list.listGetter(top.value, list.index);
                    list.index++;
                    // we need to create a getter / setter function
                    // but then with a given index of the list

                    stack.pop();
                    break;
                }
                case PropertyType::Dictionary:
                {
                    stack.pop();
                    break;
                }
            }
        }
    }
}