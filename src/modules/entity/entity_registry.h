//
// Created by Arjo Nagelhout on 30/12/2023.
//

#ifndef SHAPEREALITY_ENTITY_REGISTRY_H
#define SHAPEREALITY_ENTITY_REGISTRY_H

#include "config.h"
#include "entity/type.h"
#include "entity/sparse_set.h"
#include "entity/view.h"

#include <reflection/type_id.h>

#include <vector>
#include <unordered_map>

using namespace reflection;

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
     * and we simply iterateUsingStack over that view and perform operations on the data.
     *
     * These operations are called systems.
     *
     * And there we have it: an entity-component system
     */
    class EntityRegistry final
    {
    public:
        explicit EntityRegistry() = default;

        ~EntityRegistry() = default;

        // delete copy constructor and assignment operator
        EntityRegistry(EntityRegistry const& registry) = delete;

        EntityRegistry& operator=(EntityRegistry const& registry) = delete;

        //--------------------------------------------------
        // Entities
        //--------------------------------------------------

        // returns whether was successful
        bool createEntity(EntityId entity)
        {
            bool success = entities.emplace(entity);
            // we don't need to resize the components as well, these can
            // be resized on demand
            return success;
        }

        [[nodiscard]] bool entityExists(EntityId entity)
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
        void destroyEntity(EntityId entity)
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
            TypeId typeId = TypeIndex<Type>::value();
            auto* baseSet = components.at(typeId).get();
            return static_cast<SparseSet<Type>*>(baseSet);
        }

        template<typename Type>
        [[nodiscard]] bool componentTypeExists() const
        {
            TypeId typeId = TypeIndex<Type>::value();
            return components.contains(typeId);
        }

        /**
         * @tparam Type the type of the component
         * @param entity the entity to add the component to
         * @return whether adding was successful
         */
        template<typename Type, typename... Args>
        bool addComponent(EntityId entity, Args&& ... args)
        {
            if (!entityExists(entity))
            {
                return false;
            }

            TypeId typeId = TypeIndex<Type>::value();
            if (components.contains(typeId))
            {
                if (components.at(typeId)->contains(entity))
                {
                    return false; // error: component was already added
                }
            }
            else
            {
                // not yet initialized
                components[typeId] = std::make_unique<SparseSet<Type>>();
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
        bool removeComponent(EntityId entity)
        {
            if (!entityExists(entity))
            {
                return false;
            }

            if (!componentTypeExists<Type>())
            {
                return false;
            }

            TypeId typeId = TypeIndex<Type>::value();
            if (!components.at(typeId)->contains(entity))
            {
                return false;
            }

            components.at(typeId)->remove(entity);
            return true;
        }

        template<typename Type>
        bool removeComponentType()
        {
            if (!componentTypeExists<Type>())
            {
                return false;
            }

            TypeId typeId = TypeIndex<Type>::value();
            components.erase(typeId);
            return true;
        }

        template<typename Type>
        Type& getComponent(EntityId entity)
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
        [[nodiscard]] bool entityContainsComponent(EntityId entity)
        {
            if (!componentTypeExists<Type>())
            {
                return false;
            }

            TypeId typeId = TypeIndex<Type>::value();
            return components.at(typeId)->contains(entity);
        }

        // clears all components and the entities they contain
        void clear()
        {
            entities.clear();
            components.clear();
        }

        SparseSet<EntityId> entities;
        std::unordered_map<TypeId, std::unique_ptr<SparseSetBase>> components;
    };
}

#endif //SHAPEREALITY_ENTITY_REGISTRY_H
