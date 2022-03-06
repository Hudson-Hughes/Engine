#include <json.hpp>
#include <world.hpp>
#include <people.hpp>

#include <iostream>

using namespace std;

constexpr MaskQuery q = GenerateQueryMasks<READ<hambly>, NOT<sister>, OPTIONAL<trevor>>();

int main(){
    std::cout << "Running " << ENGINE_VERSION << std::endl;
    World world;
    EntityID e1 = world.createEntity(ComponentsMask<trevor, sister>);
    // std::cout << std::endl;
    // std::cout << world << std::endl << std::endl;
    EntityID e2 = world.createEntity(ComponentsMask<sister, trevor>, e1);
    world.addComponents(e1, ComponentsMask<hambly>);
    world.orphan(e2);
    world.setParent(e1, e2);
    
    std::cout << world << std::endl << std::endl;
}