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
            .value = property.getter(value),
            .type = property.type,
        };
        switch (property.type)
        {
            case PropertyType::Object:
            {
                result.object.index = 0;
                result.object.typeId = property.object.typeId;
                result.object.typeInfo = r.getTypeInfo(property.object.typeId);
                break;
            }
            case PropertyType::List:
            {
                result.list.index = 0;
                result.list.propertyInfo = &property;
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
                    ListStackFrame& list = top.list;

                    size_t size = list.propertyInfo->list.size(top.value);

                    bool recurse = size > 0;

                    if (size > 0 && list.index == 0)
                    {
                        std::cout << "we got a list property" << std::endl;
                        recurse = callback(top);
                    }

                    if (recurse && list.index < size)
                    {
                        std::cout << "list index: " << list.index << std::endl;
                        list.index++;
                    }
                    else
                    {
                        if (recurse)
                        {
                            onPop(top);
                        }
                        stack.pop();
                    }

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