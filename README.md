## ⚠️ Work in progress

This codebase is nowhere near usable or complete. It is a testing ground for building a cross-platform game engine, with a graphics API wrapper, ECS, platform abstractions, asynchronous asset loading, C++ runtime reflection and a renderer. 

# shapereality-engine

A game engine and app framework with the ambition to provide a robust way to create high quality cross-platform XR experiences, prototypes and productivity
applications.

## Shader compilation

Shader compilation is not yet implemented. Here's the command for building the shaders manually using a Python script:

```
python compile_shaders.py ../data/shaders ../build/shaders ../build/shaders/library
```

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

In addition, an **Entity Component System (ECS)** is used to create a registry with entities that contain these types:

```c++
EntityRegistry entities;

// creating an entity
Entity entity = entities.createEntity<Transform, MeshRenderer>();

// editing an entity's data
Transform& transform = entities.get<Transform>(entity);
transform.position = Vector<3>{0.0f, 1.0f, 0.0f};
```

Regular C++ functions, called **systems** in the context of an ECS, can be used to forEach over the entities with specific **components**:

```c++
void system(EntityRegistry& entities)
{
    for (auto [transform, meshRenderer]: entities.view<Transform, MeshRenderer>())
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

### 6. An Editor built using the engine's modules

The editor enables managing a project's assets, configuring their parameters, import settings, and composing them into a scene. This editor should have a great
user interface and user experience, enabled by the engine's core modules themselves.

The Editor "application" should not be a large codebase. When responsibilities grow too large, it should be broken up into modules that are placed in the
modules folder.

The Editor should integrate well with the operating system. (e.g. no custom file browsers when macOS has a built-in one that is infinitely better).

### 7. UI should feel native

- 2D UI should feel native.
- 3D UI should be defined using the same primitives as 2D UI.
- Multilingual Unicode text editing and 3D text rendering with glyph caching
- Native feeling high-performance scroll views. Should be nothing short of what iOS scrolling performance is like. They also built it with code. Why should
  every game engine have abysmal scroll view performance?

### 8. Driven by need

Dogfooding is the most powerful way to avoid feature and scope creep. The engine should never be the goal in itself, but be an enabler for a specific
application or experience. This is also why the focus is on augmented and virtual reality, rather than attempting to support every platform under the sun.
