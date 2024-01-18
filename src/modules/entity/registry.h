//
// Created by Arjo Nagelhout on 30/12/2023.
//

#ifndef SHAPEREALITY_REGISTRY_H
#define SHAPEREALITY_REGISTRY_H

#include "config.h"
#include "type.h"
#include "sparse_set.h"
#include "view.h"

#include <vector>
#include <unordered_map>

namespace entity
{
    // https://stackoverflow.com/questions/21269083/how-to-create-a-multiple-typed-object-pool-in-c

    /**
     * The Registry contains a sparse set of entities. Entities are simply an index (an integer)
     *
     * It also contains a map of sparse sets of components. These components contain only data
     * (Plain Old Datastructures)
     *
     * See:
     *
     * entity               0       1       2       3       4
     * transform            {}      {}      {}      {}      {}
     * mesh_renderer        {}      {}      {}      _       _
     * physics_rigidbody    _       _       {}      _       _
     *
     * _ = TOMBSTONE
     * {} = component data
     *
     * Then we get a view, such as that of all entities with transform and mesh_renderer components,
     * and we simply iterate over that view and perform operations on the data.
     *
     * These operations are called systems.
     *
     * And there we have it: an entity-component system
     */
    class Registry final
    {
    public:
        explicit Registry() = default;

        ~Registry() = default;

        // delete copy constructor and assignment operator
        [[maybe_unused]] Registry(Registry const& registry) = delete;

        Registry& operator=(Registry const& registry) = delete;

        //--------------------------------------------------
        // Entities
        //--------------------------------------------------

        // returns whether was successful
        bool createEntity(entity_type entity)
        {
            bool success = entities.emplace(entity);
            // we don't need to resize the components as well, these can
            // be resized on demand
            return success;
        }

        [[nodiscard]] bool entityExists(entity_type entity)
        {
            return entities.contains(entity);
        }

        [[nodiscard]] size_type size() const
        {
            return entities.size();
        }

        [[nodiscard]] size_type entityCount() const
        {
            return entities.denseSize();
        }

        // also destroys its components (or at least makes them inaccessible)
        void destroyEntity(entity_type entity)
        {
            entities.remove(entity);

            // remove components
            for (auto& component: components)
            {
                component.second->remove(entity);
            }
        }

        //--------------------------------------------------
        // Components
        //--------------------------------------------------

        // casts the base sparse set to the inherited sparse set with associated type
        template<typename Type>
        [[nodiscard]] SparseSet<Type>* getComponentType() const
        {
            if (!componentTypeExists<Type>())
            {
                return nullptr;
            }
            type_id typeIndex = getTypeIndex<Type>();
            auto* baseSet = components.at(typeIndex).get();
            return static_cast<SparseSet<Type>*>(baseSet);
        }

        template<typename Type>
        [[nodiscard]] bool componentTypeExists() const
        {
            type_id typeIndex = getTypeIndex<Type>();
            return components.contains(typeIndex);
        }

        /**
         * @tparam Type the type of the component
         * @param entity the entity to add the component to
         * @return whether adding was successful
         */
        template<typename Type, typename... Args>
        bool addComponent(entity_type entity, Args&& ... args)
        {
            if (!entityExists(entity))
            {
                return false;
            }

            type_id typeIndex = getTypeIndex<Type>();
            if (components.contains(typeIndex))
            {
                if (components.at(typeIndex)->contains(entity))
                {
                    return false; // error: component was already added
                }
            }
            else
            {
                // not yet initialized
                components[typeIndex] = std::make_unique<SparseSet<Type>>();
            }

            // virtual templated member functions are not allowed, so we need to cast the base sparse set
            // to the type specific one.
            getComponentType<Type>()->emplace(entity, std::forward<Args>(args)...);

            return true;
        }

        /**
         *
         * @tparam Type
         * @param entity
         * @return whether destroying was successful
         */
        template<typename Type>
        bool removeComponent(entity_type entity)
        {
            if (!entityExists(entity))
            {
                return false;
            }

            if (!componentTypeExists<Type>())
            {
                return false;
            }

            type_id typeIndex = getTypeIndex<Type>();
            if (!components.at(typeIndex)->contains(entity))
            {
                return false;
            }

            components.at(typeIndex)->remove(entity);
            return true;
        }

        template<typename Type>
        bool removeComponentType()
        {
            if (!componentTypeExists<Type>())
            {
                return false;
            }

            type_id typeIndex = getTypeIndex<Type>();
            components.erase(typeIndex);
            return true;
        }

        template<typename Type>
        Type& getComponent(entity_type entity)
        {
            return getComponentType<Type>()->get(entity);
        }

        template<typename Type, typename Compare, typename... Args>
        bool sort(Compare compare, Args&& ... args)
        {
            if (!componentTypeExists<Type>())
            {
                return false;
            }
            return getComponentType<Type>()->sort(std::move(compare), std::forward<Args>(args)...);
        }

        // todo: enable creating a view that doesn't calculate which component to use,
        //       but simply takes the first one in the list.
        //       this could be useful in the context of wanting to use the order of a specific sorted component type,
        //       such as for mesh renderers or when updating transforms.
        template<typename... Types>
        [[nodiscard]] auto view(IterationPolicy iterationPolicy = IterationPolicy::UseSmallestComponent)
        {
            return View<SparseSet<Types>...>(iterationPolicy, getComponentType<Types>()...);
        }

        /**
         *
         * @tparam Type type of the component
         * @return whether the entity contains the given component
         */
        template<typename Type>
        [[nodiscard]] bool entityContainsComponent(entity_type entity)
        {
            if (!componentTypeExists<Type>())
            {
                return false;
            }

            type_id typeIndex = getTypeIndex<Type>();
            return components.at(typeIndex)->contains(entity);
        }

        // clears the entire registry and all its
        void clear()
        {
            entities.clear();
            components.clear();
        }

    private:
        SparseSet<entity_type> entities;
        std::unordered_map<type_id, std::unique_ptr<SparseSetBase>> components;
    };
}

#endif //SHAPEREALITY_REGISTRY_H
