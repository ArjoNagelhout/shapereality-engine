//
// Created by Arjo Nagelhout on 06/02/2024.
//

#include <gtest/gtest.h>

#include <stack>

namespace node_recurse
{
    size_t currentDepth = 0;

    struct Node
    {
        std::string name;
        std::vector<Node> children;
    };

    void printAtCurrentDepth(std::string const& string)
    {
        std::cout << std::string(currentDepth * 4, ' ') << string << std::endl;
    }

    void beginTreeNode()
    {
        currentDepth++;
        printAtCurrentDepth("begin");
    }

    void endTreeNode()
    {
        assert(currentDepth != 0 && "too many endTreeNode calls");
        printAtCurrentDepth("end");
        currentDepth--;
    }

    void print(Node* node)
    {
        beginTreeNode();
        printAtCurrentDepth(node->name);
        for (auto& child: node->children)
        {
            print(&child);
        }
        endTreeNode();
    }

    void printUsingStack(Node* node)
    {
        struct StackFrame
        {
            Node* node;
            size_t index = 0;
        };

        std::stack<StackFrame> stack;
        stack.emplace(StackFrame{.node = node});

        while (!stack.empty())
        {
            StackFrame& top = stack.top();
            Node* current = top.node;

            if (top.index == 0)
            {
                beginTreeNode();
                printAtCurrentDepth(current->name);
            }

            if (top.index == current->children.size())
            {
                endTreeNode();
                stack.pop();
            }
            else
            {
                stack.emplace(StackFrame{
                    .node = &current->children[top.index]
                });
                top.index++;
            }
        }
    }

    TEST(Reflection, NodeRecurse)
    {
        Node nodes{
            .name = "1",
            .children{
                Node{
                    .name = "1_1",
                    .children{
                        Node{.name = "1_1_1"},
                        Node{.name = "1_1_2"},
                        Node{.name = "1_1_3"}
                    }
                },
                Node{.name = "1_2"},
                Node{.name = "1_3"},
                Node{
                    .name = "1_4",
                    .children{
                        Node{.name = "1_4_1"},
                        Node{.name = "1_4_2"}
                    }
                }
            }
        };

        // recurse over nodes
        //print(&nodes);

        printUsingStack(&nodes);

        // after running
        assert(currentDepth == 0 && "too many beginTreeNode calls");
    }

}