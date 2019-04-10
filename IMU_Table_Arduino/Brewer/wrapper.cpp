#include "wrapper.h"
#include <iostream>
Wrapper::Wrapper(){}

void Wrapper::printAll(){
    std::cout << "Position X: " << positionX << "\t\tPosition Y: " << positionY << "\t\tPosition Z: " << positionZ << std::endl;
}

