# EntityRegistry Serialization Design

## Polymorphism

Serialization of an `EntityRegistry` is a hard problem, as the following data structure is what represents an `EntityRegistry`:

```c++
struct EntityRegistry
{
    SparseSet<EntityId> entities;
    std::unordered_map<TypeId, std::unique_ptr<SparseSetBase>> components; 
};
```

Where we define `SparseSetBase` and `SparseSet<EntityId>` as:

```c++
struct SparseSetBase
{
    std::vector<size_t> sparse;
    std::vector<size_t> dense;
};

template<typename Type>
struct SparseSet
{
    std::vector<Type> denseValues;
};
```

When we serialize a `std::unique_ptr<SparseSetBase>` we do not want to perform object slicing when serializing (meaning 
that we would only serialize the type that it is referenced by, in this case `SparseSetBase`).

We want to serialize it as the correct type that inherits from SparseSetBase, as to support type polymorphism in 
serialization.

The way this would for example be represented in a serialized json file is as follows:

(assuming Base and Child are properly registered in the TypeRegistry) 

```c++
struct Base
{
    bool base = false;
    int base1 = 0;
};

struct Child : public Base
{
    bool child = true;
    int child1 = 10;
};

void serialize()
{
    Base* a = new Child();
    
    std::string json = JsonSerializer::serialize(a);
}
```

json contains:

```json
{
  "$type": "Child",
  "data": {
    "base": false,
    "base1": 0,
    "child": true,
    "child1": 10
  }
}
```

When an array with pointers to different polymorphic types exists, the following should happen:

```c++
struct Child2 : public Base
{
    float somethingElse = 1.5f;
    bool definitely = false;
    int wow = 123;
};

struct Items
{
    std::vector<Base*> items;
};

void serialize()
{
    Base* a = new Child();
    Base* b = new Child2();
    Base* c = new Base();
    
    Items items{.items = {a, b, c}};
    
    std::string json = JsonSerializer::serialize(items);
}
```

should generate:

```json
{
  "items": [
    {
      "$type": "Child",
      "data": {
        "base": false,
        "base1": 0,
        "child": true,
        "child1": 10
      }
    },
    {
      "$type": "Child2",
      "data": {
        "base": false,
        "base1": 0,
        "somethingElse": 1.5,
        "definitely": false,
        "wow": 123
      }
    },
    {
      "$type": "Base",
      "data": {
        "base": false,
        "base1": 0
      }
    }
  ]
}
```

In the case of the `EntityRegistry`, this type polymorphism is only one part of the puzzle. 
We also need:
- binary / json hybrid serialization (similar to gltf and Google's [Protocol Buffers](https://protobuf.dev))
- registration without requiring properties to be public (something akin to `friend class Registration`)
- automatic registration of each component type (templates) without having to manually add each template type. 
  - not sure yet how to do this?
- resolve type id from name: "Child" -> corresponding TypeId

## Json / binary hybrid
This requires the following:
- `BinarySerializer` and `JsonSerializer` are implemented for primitive types.  

## Other cleaning up tasks
EnumSerializer seems wrong, as it is a type to and we might need information about it. So it should be part of 
`reflection`, not the `serialize` subdirectory. 

## Conclusion
This solution is definitely more involved than implementing simple single purpose serialization for the EntityRegistry,
but with nested types (e.g. a `Scene` containing a bunch of json metadata plus an `EntityRegistry`). This would require
separating the `EntityRegistry` json into its separate file that the `Scene` json file references to. 
That feels wrong. 

Here I'm effectively writing a type system on top of C++'s type system. 
We probably also need to redo std::any, because it does not contain the TypeId. This is something we might need.
Also, something was mentioned about Small Buffer Optimization (SBO), this might enable making std::any more efficient?

## First step: polymorphism

How to implement this. The way we have currently implemented the reflection system is relatively straightforward: 

There is a `TypeRegistry` that contains a `TypeInfo` entry for each registered `TypeId`. 

Each `TypeInfo` has a list of nodes, and a list of properties it contains. Each member contains a `PropertyNode` tree, 
for example: 

```c++
struct Something
{
    std::vector<std::unordered_map<TypeId, std::vector<float>>> items;
};
```

becomes

```
TypeInfo
{
  name = "Something",
  nodes = [
    PropertyNode{
      type = Type::Vector,
      valueNode = 1
    },
    PropertyNode{
      type = Type::UnorderedMap,
      keyTypeId = <type id of TypeId here>
      valueNode = 2
    },
    PropertyNode{
      type = Type::Vector,
      valueNode = 3
    },
    PropertyNode{
      type = Type::Object,
      typeId = <type id of float here>
    }
  ],
  properties = [
    PropertyInfo{
      name = "items",
      node = 0
    }
  ]
}
```

or

```
TypeInfo contains list of properties:

┌───────────────────────┐
│ [0] = PropertyInfo        │
│ name: "items"         │
│ node: 0               │
└───────────────────────┘

and a list of nodes:

┌───────────────┐
│[0] = ListNode │
└───────┬───────┘
        │
        │ valueNode
        │
┌───────▼─────────────┐  keyTypeId
│[1] = DictionaryNode ├──────────────►  TypeInfo...
└───────┬─────────────┘
        │
        │ valueNode
        │
┌───────▼───────┐
│[2] = ListNode │
└───────┬───────┘
        │
        │ valueNode
        │
┌───────▼─────────┐      typeId
│[3] = ObjectNode ├──────────────────►  TypeInfo...
└─────────────────┘

```

How would we introduce type polymorphism in this graph?

We need to somehow register that a type has a base type, as we might not be able to deduce that using templates alone?
Or maybe we can actually deduce that. Let's test it. 

Maybe using [`<type_traits>`](https://en.cppreference.com/w/cpp/header/type_traits)?

We can simplify our reflection system by not supporting references, only "owned" pointers (smart pointers). 

there is an `is_polymorphic` type trait. For this to be the case it should at least one virtual function. 
Because this does not always have to be the case in the type of polymorphism we are interested in, we might have to
resort to a similar technique that is used by skypjack's `meta` library. This has a `.base()` method in the factory
class for creating `TypeInfo` objects. 

We should keep a clear distinction between the Serializers and the TypeInfo node structure. 

Alright. So, how do we add something to the graph

it is part of a member.

Should note whether it is a pointer or not.

We have:
ListNode for std::vector<value_type>
DictionaryNode for std::unordered_map<key_type, value_type>
and PointerNode for std::unique_ptr<SomeType>

if and only if something is a pointer, we need to perform a check whether the pointer type has child types. 

struct TypeInfo
{
  std::string name;
  TypeId parent; 
  std::vector<TypeId> children;
  std::vector<PropertyNode> nodes;
  std::vector<PropertyInfo> properties;
}

by calling `.base()` we basically set the `parent` for the type we call it on, and
add the type to the `parent`'s `children` list. 

In order to successfully serialize the correct type we do need to give this responsibility
to the serializer, as the serializer knows which type it has found in for example the json file, with
the following signature:

```json
{
  "$type": "Child",
  "data": {
    
  }
}
```

when recursively iterating over the nodes in the json serializer, we should have some way of adding the right
polymorphic type to the pointer type. 

We need to add a pointer type node. 

so we have:
ObjectNode
ListNode
DictionaryNode
PointerNode

## Determining polymorphic type on runtime
Let's say we have a `std::vector<std::unique_ptr<Base>>`. 

This would mean we have a PointerNode with `valueTypeId` = `TypeIndex<Base>::value()`. 

Then we can query the `TypeInfo` and see if it contains any `children`.
If so, it should determine on runtime whether the current pointed-to type is any of the children, or sub children. 

Let's make a little experiment. 