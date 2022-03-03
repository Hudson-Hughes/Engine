#include <hello.hpp>
#include <json.hpp>
#include <world.hpp>
#include <people.hpp>

#include <iostream>

using namespace std;

int main(){
    
    hello::say_hello();
    std::cout << "Running " << ENGINE_VERSION << std::endl;
    World world;
    EntityID e1 = world.createEntity(ComponentsMask<hambly, trevor>);
    // std::cout << std::endl;
    // std::cout << world << std::endl << std::endl;
    std::cout << e1.generation << "\t" << e1.index << endl;
    EntityID e2 = world.createEntity(ComponentsMask<hambly, trevor>);
    std::cout << world << std::endl << std::endl;
}