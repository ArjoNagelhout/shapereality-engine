# ShapeReality

ShapeReality is an experimental engine for creating high quality XR experiences, apps and games.

It's goals are to be lightweight and modular in design, taking the best bits from existing engines and creative tools.

In the end, an engine is a tool, not a goal in itself, so its sole purpose is to enable the creation of the
best XR experiences in the market. 

## Design considerations

### Shader compilation
- Shaders should ideally be written in one shading language (e.g. GLSL) and compiled to MSL (Metal Shading Language) and SPIR-V, depending on the rendering backend.
  - For now, it is probably faster to rewrite the shaders for both Metal and GLSL. 
- Initially, the shader compiler should not be part of the main codebase. Only when we have a full editor it will be useful. 
  - For the time being, the game engine is not a full-blown application on its own, but functions more as a library you can statically link against.

Use the following (temporary) script for compiling shaders. 

```shell
python compile_shaders.py ../data/shaders ../build/shaders ../build/shaders/library
```

### Renderer

#### Text rendering and UI
- High quality text rendering should be universal for both 2D UI and Spatial UI.
  - Using SDF, with runtime glyph caching, instead of having to create a bitmap font at compile time.
- UI should be universal for mobile, desktop and VR and should be feature rich enough to support building the editor experience with it. 

#### High performance transparent textured objects
- In my art workflow I like to use a lot of billboarding. This should be as efficient as possible. 

#### Graphics wrapper
- There should be a clear distinction between the low level graphics wrapper and the renderer. 
- Rendering code should thus be graphics backend agnostic and built only on top of the graphics wrapper. 
  - We can query for certain features that are supported by the different backends / platforms and enable / disable certain features in the renderer accordingly.

### Editor
- There should be an editor similar to Unity that enables arranging the assets and setting their import settings for example.

### XR
- Important. Should work with VisionOS and Quest 3.

### Physics
- Use a library. Don't care about this.

### Audio
- Use a library. Really don't care about implementing this ourselves. 

### Networking
- Too hard. But probably library + tight integration with the assets system. Server-Client split that also is present for editing the game, not just on runtime. 

### Asset formats
- As there is an ungodly amount of different assets we could import (.fbx, .obj, .dae, BIM files), we need to focus on the ones that are important from the get-go. 

### Scripting and building an app binary
- C++. 
- How to do linking? 
- How do we compile and build a binary executable that can immediately be shipped?

### Collaboration
- Collaboration can be achieved by hosting a simple backend that has assets in a repository with the project files in git.
- If we want to have scene live editing, we might have to write a more complicated assets backend. 
  - We want to store a wide range of assets inside a simple S3 bucket and corresponding SQL database. This can make asset distribution and even implementing collaboration way easier, compared to having
  everything stored locally or tracked in git. 
  - This backend should be easy to self-host and should use a simple docker container.

### Dependencies
- Kept to a minimum. Prefer small, well-maintained libraries.
- Look as much as possible at existing implementations and libraries. Don't reinvent the wheel with regard to
  architecture or algorithms. Something something standing on the shoulders of giants.