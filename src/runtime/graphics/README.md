# Graphics

A simple low-level library that wraps low-level graphics APIs and exposes a platform-agnostic interface for things like
creating buffers, textures and render pipeline states. 

It aims to do as little as possible. A renderer can then be built on top of this library. 

Annoying bit: input is so ingrained into window logic, input needs to know about graphics, and 
graphics needs to know about input. So the best way I found (which is also done by SDL), is to
simply merge the input logic into the graphics code. 

## Backends
The goal is to support multiple graphics backends, but for now only Vulkan and Metal are planned.

### Metal

### Vulkan