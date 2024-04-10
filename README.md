# ShapeReality

A game engine and app framework with the ambition to provide a robust way to create high quality cross-platform XR experiences, prototypes and productivity
applications.

## Design principles

### 1. Minimal dependencies

A platform abstraction library such as [SDL](https://github.com/libsdl-org/SDL), a graphics API
wrapper such as Meta's [igl](https://github.com/facebook/igl), or an entity component system library like
skypjack's [entt](https://github.com/skypjack/entt) comes with added complexity that might not be directly needed for the use case at hand. These libraries
also creep their way into the entire engine, or are the foundation on which the engine would be built, which makes it hard to remove them as a dependency.
When a library gets deprecated or maintenance for them slows down, this burden is now placed on you. Which would mean have just inherited a large codebase
that you don't fully understand.\
So we strive to only depend on libraries that have a single clear use-case and can easily be swapped out for another library, such
as `cgltf`, `fmt`, `json`, `lodepng` or `thread-pool`.

### 2. Modern C++

Uses Modern C++ and the latest C++ standard (C++23 as of writing) that is supported by major compilers (Clang, Apple Clang, GCC) on supported platforms.

There is some debate about what modern C++ is exactly, and explaining which features to use or not would require writing an entire style guide, but for now:

We use:

- `<memory>`: `std::unique_ptr<Type>`, `std::shared_ptr<Type>`, etc.
- containers: `std::unordered_map`, `std::vector`
- `std::string`
- `std::filesystem`
- template metaprogramming

We don't use:

- `std::format` is not well-supported by Apple Clang, so we use the `fmt` library for this
- `std::expected` is not well-supported by Apple Clang, so we have a custom type `ValueResult<Type>`.
- We avoid using `std::async` as we want control over the threads that are being used using `BS::thread_pool`
- Macros. Only simple cross-platform conditional compilation macros should be used such as: `PLATFORM_MACOS` or `PLATFORM_ANDROID`

### 3. Data oriented design, OOP kept to a minimum

Godot or Unity are focused on modelling the game world using classes and multiple levels of inheritance. This is bad for maintainability and clarity of which
code is doing what. This engine aims to be as simple as possible, giving the user the ability to compose types in the following way:

```c++
struct Mesh
{
    Buffer vertices;
    Buffer indices;
};

struct Material
{
    Shader* shader;
    Properties properties;
};

struct MeshRenderer
{
    Mesh* mesh;
    Material* material;
};

struct Transform
{
    Vector<3> position;
    Quaternion rotation;
    Vector<3> scale;
};
```

In addition, an **Entity Component System (ECS)** is used to create a world with entities that contain these types:

```c++
World world;

// creating an entity
Entity entity = world.createEntity<Transform, MeshRenderer>();

// editing an entity's data
Transform& transform = world.get<Transform>(entity);
transform.position = Vector<3>{0.0f, 1.0f, 0.0f};
```

Regular C++ functions, called **systems** in the context of an ECS, can be used to iterate over the entities with specific **components**:

```c++
void system(World& world)
{
    for (auto [transform, meshRenderer]: world.view<Transform, MeshRenderer>())
    {
        // do something with each entity that contains a Transform and MeshRenderer
    }
}
```

No inheritance trees like `SoftBody3D` < `GeometryInstance3D` < `VisualInstance3D` < `Node3D` < `Node` < `Object`.

### 4. No scripting language

Game code should be written in the same language as the engine: the engine is a library. Being able to directly inspect what
the engine is doing when calling certain functions, instead of having to rely on spotty documentation or manually looking for which
functions are being called by the bindings, is a better development experience.

It also enables the user of the engine to learn how a game engine works in an incremental way, rather than shielding them from the
internals and hoping they build a good mental model of what happens under the hood.

Exposed internals makes writing performant code easier, as it is clearer which functions perform many operations.

The only downside to this design principle is having to recompile the engine when changes are made to the game code.
However, for professional applications and XR experiences, this is not an issue.

### 5. No distinction between Editor and Runtime

The engine is simply a set of modules, so when you ship a game or application, you can still use all the editor's import
logic as that is part of the `asset` and `import_gltf` module for example. 

This makes it easier to write productivity applications, or a window manager for example.