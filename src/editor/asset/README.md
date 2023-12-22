# Asset

Contains everything related to importing, serializing and deserializing assets.

This will probably be refactored once the requirements are more clear.

There are two systems: 
- editor (similar to Unity's AssetDatabase), can import all sorts of meshes
- runtime (all assets are compiled to their corresponding binary representation
  that are used by the rest of the engine runtime)