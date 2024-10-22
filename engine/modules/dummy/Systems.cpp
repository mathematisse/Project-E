#include <iostream>
#include "engine/modules/dummy/Systems.hpp"

namespace ECS::S {

RandomizeNumberSystem::RandomizeNumberSystem():
    AMonoSystem(true)
{
}

void RandomizeNumberSystem::_innerOperate(C::NumberPool::Types &cnumber)
{
    auto [number] = cnumber;
    number = std::rand() % 100;
}

////////////////////////////////////////////////////////////////////////////////

ShowNumberSystem::ShowNumberSystem():
    AMonoSystem(true)
{
}

void ShowNumberSystem::_innerOperate(C::NumberPool::Types &cnumber)
{
    auto [number] = cnumber;
    std::cout << "Number: " << number << std::endl;
}

}
