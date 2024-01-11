# Entity

A bare minimum entity-component system. 

It takes heavy inspiration from [`entt`](https://github.com/skypjack/entt) by skypjack. 
The main reason for not directly adopting `entt` is because I can't reason about the 
sheer amount of syntactical complexity that arises from doing C++ template metaprogramming.

Also, I like to have simple and single-purpose dependencies, not a 90.000 LOC 
"library of libraries".

## Components

Contains reusable utility components that are not specific to any usage.