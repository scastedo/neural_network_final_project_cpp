// PHYS 30762 Programming in C++
// Assignment Final Project
// Sebastian Castedo 12 April 2023 

#include "value.h"

int main()
{
    value a;
    std::cout<<a.data()<< " "<< a.grad()<< " " << a.prev_node()<< " " <<a.prev_op() << std::endl;
    a = value(5, 4, 2, "b","*");
    std::cout<<a.data()<< " "<< a.grad()<< " " << a.prev_node()<< " " <<a.prev_op() << std::endl;
    return 0;
}
