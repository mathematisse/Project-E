# Creating a module

Before creating a module, make sure you have read the [how to add a module](how_to_add_module.md) guide.

Once you have created a new module, you can start adding systems and components to it.

## Base Module class

Each module should have a base class that inherits from `engine::module::IModule`. This class should have a constructor that takes an `ECS::EntityManager` as an argument. The constructor should register all the systems and components of the module.

```cpp
namespace engine::module {

class PrintHello : public IModule {
    ECS::S::PrintHelloSystem printhelloSystem;

    ECS::S::PrintHelloNode PrintHelloNode;
public:
    PrintHello():
        PrintHelloNode("PrintHello", {&printhelloSystem})
    {
    }

    void load(ECS::EntityManager &entityManager) override
    {
        LOG_INFO("Loading PrintHello module");
        if (!entityManager.registerFixedSystemNode(PrintHelloNode, FIXED_ROOT_SYS_GROUP)) {
            LOG_ERROR("Failed to register PrintHello system node");
        }
    }
};

}

```

## Registering the module

To make the module available in the game, you need to add it mannually.

```cpp

#include "modules/PrintHello/PrintHello.hpp"


auto main() -> int
{
    ECS::EntityManager em;
    engine::module::PrintHello printHello(em);

    printHello.load(em);
}
```
