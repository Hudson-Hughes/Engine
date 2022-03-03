#include <hello.hpp>
#include <json.hpp>
#include <world.hpp>
#include <people.hpp>

#include <iostream>

int main(){
    
    hello::say_hello();
    std::cout << "Running " << ENGINE_VERSION << std::endl;
    World world;
    EntityID e1 = world.createEntity(ComponentsMask<hambly, trevor>);
    std::cout << std::endl;
    std::cout << world << std::endl << std::endl;
    // EntityID e2 = world.createEntity(ComponentMask());
    std::cout << world << std::endl;
}