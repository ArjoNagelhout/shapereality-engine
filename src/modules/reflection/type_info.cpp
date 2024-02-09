//
// Created by Arjo Nagelhout on 07/02/2024.
//

#include "type_info.h"

#include <stack>

namespace reflection
{
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
            .typeId = typeId,
            .typeInfo = r.getTypeInfo(typeId),
            .value = instance,
        });

        while (!stack.empty())
        {
            StackFrame& top = stack.top();
            bool recurse = (top.typeInfo && !top.typeInfo->properties.empty()); // don't recurse if no properties

            if (top.typeInfo && top.index == 0)
            {
                recurse = callback(top); // if contains properties, but should not recurse, set this value
            }

            if (recurse && top.typeInfo && top.index < top.typeInfo->properties.size())
            {
                PropertyInfo& property = top.typeInfo->properties[top.index];
                stack.emplace(StackFrame{
                    .name = property.name,
                    .typeId = property.typeId,
                    .typeInfo = r.getTypeInfo(property.typeId),
                    .value = property.getter(top.value),
                });
                top.index++;
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
        }
    }
}