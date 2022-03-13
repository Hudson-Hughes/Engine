#include <json.hpp>
#include <world.hpp>
#include <people.hpp>
#include <filter.hpp>
#include <component.hpp>
#include <system.hpp>

#include <iostream>
#include <iterator>
#include <cstddef>

using namespace std;

constexpr MaskQuery q = Query<READ<hambly>, NOT<sister>, OPTIONAL_READ<trevor>>;


int main() {
    World w;
    EntityID i = w.createEntity(ComponentMask());
    w.addComponents(i, ComponentsMask<hambly>);
    Archetype* arch = w.archetypes[0].front();
    hambly* p = get<hambly*>(arch->grab<PARENT_READ<hambly>>());
    p->a = 5;
    hambly r = get<hambly>(arch->grab<READ<hambly>>());
    r.a = 3;
    cout << p->a << "\t" << r.a << endl;
    return 0;
}
