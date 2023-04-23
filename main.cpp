// PHYS 30762 Programming in C++
// Assignment Final Project
// Sebastian Castedo 12 April 2023 

#include "value.h"

int main()
{
    std::weak_ptr<value_graph> graph_1;
    std::shared_ptr<value_graph> vg = std::make_shared<value_graph>();
    graph_1 = vg;
    value a{5,1, graph_1};
    std::cout << "&&&&&&&&& a &&&&&&&&&&&" << std::endl;
    std::cout << a << std::endl;

    value b{2, 1, graph_1};
    std::cout << "&&&&&&&&& b &&&&&&&&&&&" << std::endl;
    std::cout << b << std::endl;

    value c{b};
    std::cout << "&&&&&&&&& c = b &&&&&&&&&&&" << std::endl;
    std::cout << c << std::endl;

    std::cout << "&&&&&&&&& Child of c = b &&&&&&&&&&&" << std::endl;
    for (const auto& val : c.get_children()) {
        std::cout << *val << std::endl;
    }
    value d = a+b;
    std::cout << "&&&&&&&&& d = a+b &&&&&&&&&&&" << std::endl;
    std::cout << d << std::endl;
    std::cout << "&&&&&&&&& child of d = a,b &&&&&&&&&&&" << std::endl;
    for (const auto& val : d.get_children()) {
        std::cout << *val << std::endl;
    }

    std::vector<std::shared_ptr<value>> values = vg->get_values();
    std::cout << "Number of values in graph: " << values.size() << std::endl;
    for (const auto& val : values) {
        std::cout << *val << std::endl;
    }

        

    // for (auto& v : vg->get_values()) {
    //     std::cout << "Value: " << v->get_data() << std::endl;
    //     std::cout << "Children: ";
    //     for (auto& c : v->get_children()) {
    //         std::cout << c.lock()->get_data() << " ";
    //     }
    //     std::cout << std::endl;
    // }

    return 0;
}
