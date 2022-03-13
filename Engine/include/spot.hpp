#pragma once

class Archetype;

struct Spot {
    Archetype* archetype = nullptr;
    int index = -1;
    Spot(Archetype* arch = nullptr, int index = -1) : archetype(arch), index(index) {};
};
