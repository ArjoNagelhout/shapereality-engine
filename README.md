# ShapeReality

ShapeReality is a cross-platform game engine written in C++ with the following goals:

- game logic should be written in the same language as the engine: the engine should be a library, not a framework with 
  bindings in between the engine and game logic. 
- to support inspecting, editing and composing assets, the engine needs an excellent visual editor that adheres to 
  platform standards
- external dependencies should be kept to a minimum

## Compiling shaders

On the long term, ideal is to have a platform-agnostic shader language and accompanying metadata such as `.glsl`, that 
compiles to both [`SPIR-V`](https://www.khronos.org/spir/) and [`MSL` or `Metal Intermediate Representation`](https://developer.apple.com/documentation/metal/shader_libraries/building_a_shader_library_by_precompiling_source_files?language=objc). 

However, this adds significant complexity to the codebase, so for now we use shaders that are adapted for each graphics 
backend's accepted shading language. 

```
python compile_shaders.py ../data/shaders ../build/shaders ../build/shaders/library
```

## Serialization

Todo

## Networking

Todo

# Modules

Modules are the core of the engine. They are meant to be not tightly coupled, have clear APIs
and boundaries to their responsibilities.

## Assets

Functionality for importing different assets formats, and loading these on runtime.

## Common

This is shared code between modules, such as error handling or logging, which should be
uniformly handled. Common should be as simple as possible. If it grows too much, this should be broken up
into its own module.

## Entity

A minimal **Entity Component System (ECS)** implementation heavily inspired by [entt](https://github.com/skypjack/entt) by
skypjack, using sparse sets and views for iterating over components.

An ECS's data structure can be thought of as a table, where the columns are entities, and the rows are
component types. Each entry in the table is either `null`, or of type component.

Systems are nothing more than a function that iterates over this table and alters its entries.

### Components

Entity contains reusable components with corresponding systems:

- **Hierarchy**: A tree with functionality to modify the hierarchy.

## Graphics

A thin platform abstraction layer for getting graphics on the screen and getting input from windows.

It implements a thin wrapper on top of low-level graphics APIs as **Metal** and **Vulkan**, and exposes
a platform-agnostic interface for things like creating buffers, textures and render pipeline states.

It aims to do as little as possible. A renderer could be built *on top* of this module.

### Supported graphics backends per platform

| Platform   | iOS     | macOS                                             | visionOS | Windows | Meta Quest | Android |
|------------|---------|---------------------------------------------------|----------|---------|------------|---------|
| **Metal**  | Planned | Yes                                               | Planned  |         |            |         |
| **Vulkan** |         | Planned (for development purposes using MoltenVK) |          | Planned | Planned    | Planned |

Notice the omission of Direct3D. This is because Vulkan suffices and performs well.

### XR

XR dictates how to render to the screen, so we will have to incorporate XR into the graphics module and adapt the 
graphics module so that it can support both a 6DOF + stereographic application and a 2D application.  

- OpenXR
- Oculus PC SDK

### WebXR

Todo

### WebGPU

Todo

## Input

Builds upon the `graphics` module to provide event based input, not just for windows, but also for external
input devices and XR input devices.

Maybe this should also have a simple way to bind certain actions that can be read out to specific input devices.
Similar to Unity's input system, but then statically typed and not via strings or a weird interface.

Supported input device types:

- Keyboard
- Mouse
- Trackpad (planned)
- Drawing tablet (planned)
- Multitouch touchscreen (planned)

## Math

A simple math library heavily inspired by [glm](https://github.com/g-truc/glm). It aims to provide a minimum
feature set for a 3D game engine, and support switching between column-major and row-major storage depending
on the target platform.

## Renderer

Builds on `graphics`, `entity` and `math` to build a minimal platform-agnostic renderer that can render `text`, 
`vector graphics`, `2D scenes`, `3D scenes` and `UI`, as defined by the rendering agnostic `Scene` module. 

### Render graph

To optimize the scheduling of different render passes, we employ a render graph that gets topologically sorted

## Scene

In order to support switching out the rendering to RealityKit for Vision Pro, we create a clear distinction between the
scene representation (present in this module, `Scene`), and the rendering of that scene representation (which is done
by the `Renderer` module). You could see `Renderer` as a backend for `Scene`, where something like `RealityKitAdapter`
would be an alternative backend.

This does introduce some additional complexity with what type of data entities are allowed to store and whether to use
one registry for both the rendering data and the scene simulation data, but the clear separation of concerns will be
beneficial in the long run. 

`Application` -> `Scene` -> `Renderer` -> `Graphics` -> `Metal`

`Application` -> `Scene` -> `RealityKit`