// PHYS 30762 Programming in C++
// Assignment Final Project
// Sebastian Castedo 12 April 2023 

#include "value.h"

int main()
{
    value a{5};
    std::cout<<a.data()<< std::endl;
    value b{2};
    std::cout<<b.data()<< std::endl;
    value c{a*b};
    std::cout<<c.data()<< std::endl;
    return 0;
}
