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
     * @tparam Types which component types to get
     */
    template<typename... Types>
    requires (std::is_base_of_v<SparseSetBase, Types>&& ...)
    class View final
    {
    public:
        explicit View(Types &..._components) : components(_components...)
        {
            (..., (std::cout << _components.size() << std::endl));

//            std::apply([](auto&... args) {
//                (..., (std::cout << args << std::endl));
//            }, components);
            //auto const component = std::get(_components..., 0);
            //std::cout << component << std::endl;
        }

        ~View() = default;

    private:
        std::tuple<Types&...> components;
    };
}

#endif //SHAPEREALITY_VIEW_H
