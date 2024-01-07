//
// Created by Arjo Nagelhout on 30/12/2023.
//

#ifndef SHAPEREALITY_REGISTRY_H
#define SHAPEREALITY_REGISTRY_H

#include "config.h"
#include "type.h"
#include "sparse_set.h"

#include <vector>

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

        //--------------------------------------------------
        // Entities
        //--------------------------------------------------

        // returns whether was successful
        bool createEntity(entity_type entity)
        {
            bool success = entities.emplace(entity);

            if (success)
            {
                // we need to resize the component sparse sets as well
            }

            return success;
        }

        [[nodiscard]] bool entityExists(entity_type entity)
        {
            return entities.contains(entity);
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
        [[nodiscard]] constexpr SparseSet<Type>& getComponentType()
        {
            type_id typeIndex = getTypeIndex<Type>();
            auto* baseSet = components[typeIndex].get();
            auto* set = static_cast<SparseSet<Type>*>(baseSet);
            return *set;
        }

        /**
         * @tparam Type the type of the component
         * @param entity the entity to add the component to
         * @return whether adding was successful
         */
        template<typename Type, typename... Args>
        bool addComponent(entity_type entity, Args&&... args)
        {
            type_id typeIndex = getTypeIndex<Type>();
            if (components.contains(typeIndex))
            {
                if (components[typeIndex]->contains(entity))
                {
                    // component was already added
                    return false;
                }
            }
            else
            {
                // not yet initialized
                components[typeIndex] = std::make_unique<SparseSet<Type>>();
            }

            // virtual templated member functions are not allowed, so we need to cast the base sparse set
            // to the type specific one.
            getComponentType<Type>().emplace(entity, std::forward<Args>(args)...);

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
            type_id typeIndex = getTypeIndex<Type>();
            if (!components.contains(typeIndex))
            {
                return false;
            }

            if (!components[typeIndex]->contains(entity))
            {
                return false;
            }

            components[typeIndex]->remove(entity);
            return true;
        }

        template<typename Type>
        Type& getComponent(entity_type entity)
        {
            return getComponentType<Type>().get(entity);
        }

        // sort a specific component given a compare function
        template<typename Type, typename Compare, typename... Args>
        void sort(Compare compare, Args&&... args)
        {
            type_id typeIndex = getTypeIndex<Type>();
            if (!components.contains(typeIndex))
            {
                return;
            }

            getComponentType<Type>().sort(std::move(compare), std::forward<Args>(args)...);
        }

        /**
         *
         * @tparam Type type of the component
         * @return whether the entity contains the given component
         */
        template<typename Type>
        [[nodiscard]] bool entityHasComponent(entity_type entity)
        {
            type_id typeIndex = getTypeIndex<Type>();
            if (!components.contains(typeIndex))
            {
                return false;
            }

            return components[typeIndex]->contains(entity);
        }

    private:
        SparseSet<entity_type> entities;
        std::unordered_map<type_id, std::unique_ptr<SparseSetBase>> components;
    };
}

#endif //SHAPEREALITY_REGISTRY_H
