# Renderer

This is the juicy bit of the engine.

This is where things are actually getting drawn to the screen, be it vector graphics, 3D meshes or text (oh god oh 
help glyph caching and rasterization). 

The Renderer depends on `entity`, as it has systems (i.e. functions) that can be operated on
the defined components, e.g. `MeshRendererComponent` or `TransformComponent`.  