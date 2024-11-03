# ECS

All logic in Project-E is based on the Entity-Component-System (ECS) architecture. This design pattern is used to manage modular objects in the engine. The ECS architecture is composed of three main components: Entities, Components, and Systems.

For example, an entity can be a player, an enemy, a UI element, etc. A component is data that can be attached to an entity to give it specific properties. A system is an operation that performs actions on an entity that has a specific set of components.

To make use of the ECS architecture, it is encouraged to use break down the logic of your game into small, reusable components. This will allow you to create complex behaviors by combining simple components.

**Project-E's Implementation of ECS**

## Component

A component is a simple data structure that holds data relevant to a specific aspect of an entity. For example, a `Position` component might hold the x and y coordinates of an entity.

```cpp
DECLARE_COMPONENT(
    Position,
    float, // x
    float  // y
);
```

## System

```cpp
class ApplyVelocitySystem : public S::AMonoSystem<C::Position::Pool, C::Velocity::Pool>,
                            public S::ADeltaTimeSystem {
public:
    ~ApplyVelocitySystem() override = default;

protected:
    void _innerOperate(
        typename C::Position::Pool::Types &cposition, typename C::Velocity::Pool::Types &cvelocity
    ) override
    {
        auto [x, y] = cposition;
        auto [vX, vY] = cvelocity;
        x += vX * _deltaTime;
        y += vY * _deltaTime;
    }
};
```

Here, `ApplyVelocitySystem` is a system that operates on entities with both `Position` and `Velocity` components.

`_innerOperate` is a method that is called for each entity that has the required components. The method receives the data of the components as arguments.

`S::AMonoSystem` is a base class that defines the system as a system that operates on entities with a single component.
`S::ADeltaTimeSystem` is a base class that provides the system with the delta time of the frame.

Each Component has a `Pool` that stores the data of all entities that have that component. The `Pool` is a collection of `Types` that represent the data of each entity.

for more information on ECS inner workings, you can refer to the [ECS documentation](component.md)

## Basic Simulation Loop

```cpp

DECLARE_COMPONENT(
    Position,
    float, // x
    float  // y
);

DECLARE_COMPONENT(
    Velocity,
    float, // x
    float  // y
);

using SomeEntity = Aspect<C::Position, C::Velocity>;

auto main() -> int
{
    ECS::EntityManager em;
    engine::module::Core core(em);
    ApplyVelocitySystem applyVelocitySystem;

    ECS::E::SomeEntity ePool(64);
    ECS::S::SystemTreeNode rootNode(
        "Root", {&applyVelocitySystem}
    );
    if (em.registerFixedSystemNode(rootNode) == false) {
        return 1;
    }

    auto start = std::chrono::high_resolution_clock::now();

    while (true) {
        auto end = std::chrono::high_resolution_clock::now();
        auto deltaTime = std::chrono::duration<float>(end - start).count();
        start = end;

        em.addTime(deltaTime);
    }
    em.deleteEverything();
}

```
