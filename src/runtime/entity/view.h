//
// Created by Arjo Nagelhout on 06/01/2024.
//

#ifndef SHAPEREALITY_VIEW_H
#define SHAPEREALITY_VIEW_H

#include <tuple>
#include <iostream>

namespace entity
{
    /**
     * A view is a
     *
     * @tparam Get which components to get
     */
    template<typename... Get>
    class View final
    {
    public:
        explicit View(Get &..._components)
        {
            auto const component = std::get(components, 0);
            std::cout << component << std::endl;
//            for (auto const component : )
//            {
//
//            }
        }

        ~View() = default;



    private:
        std::tuple<Get*...> components;
    };
}

#endif //SHAPEREALITY_VIEW_H
