//
// Created by Arjo Nagelhout on 30/12/2023.
//

#ifndef SHAPEREALITY_REGISTRY_H
#define SHAPEREALITY_REGISTRY_H

#include "config.h"

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

        void createEntity()
        {
            entities.emplace_back();
        }

        // also destroys its components
        void destroyEntity()
        {

        }

        /**
         *
         * @tparam Type the type of the component
         * @param entity the entity to add the component to
         */
        template<typename Type>
        void addComponent(entity_type entity)
        {
            // check if the sparse set for this component was already created
        }

        void destroyComponent()
        {

        }

        /**
         *
         * @tparam Type type of the component
         */
        template<typename Type>
        void hasComponent(entity_type entity)
        {

        }

    private:
        std::vector<entity_type> entities;
        std::unordered_map<type_id, std::unique_ptr<SparseSetBase>> components;
    };
}

#endif //SHAPEREALITY_REGISTRY_H
