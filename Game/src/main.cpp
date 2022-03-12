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
    
    return 0;
}
