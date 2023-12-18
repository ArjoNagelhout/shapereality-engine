# Graphics

A simple low-level library that wraps low-level graphics APIs and exposes a platform-agnostic interface for things like
creating buffers, textures and render pipeline states. 

It aims to do as little as possible. A renderer can then be built on top of this library. 

## Backends
The goal is to support multiple graphics backends, but for now only Vulkan and Metal are planned.

### Metal

### Vulkan