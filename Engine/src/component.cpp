#include <component.hpp>

std::string ComponentMaskAsString(ComponentMask mask) {
    std::stringstream ss; ss << "< ";
    for (int i = 0; i < 512; i++) if (mask.test(i)) ss << ComponentNames[i] << " ";
    ss << ">";
    return ss.str();
}

ComponentMask AlteredMask(ComponentMask add, ComponentMask remove, ComponentMask original) {
    return (original | add) & remove.flip();
}