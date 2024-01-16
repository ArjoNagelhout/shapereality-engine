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

It aims to do as little as possible. A renderer could be built *on top* of this library.

## Math

A simple math library heavily inspired by [glm](https://github.com/g-truc/glm). It aims to provide a minimum 
feature set for a 3D game engine, and support switching between column-major and row-major storage depending 
on the target platform.

## Renderer

Builds on `graphics`, `entity` and `math` to build a minimal platform-agnostic renderer that can render `text`, `vector graphics`, 
`2D scenes`, `3D scenes` and `UI`.