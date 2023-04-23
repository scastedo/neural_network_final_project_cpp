
    // for (auto& v : vg->get_values()) {
    //     std::cout << "Value: " << v->get_data() << std::endl;
    //     std::cout << "Children: ";
    //     for (auto& c : v->get_children()) {
    //         std::cout << c.lock()->get_data() << " ";
    //     }
    //     std::cout << std::endl;
    // }




// // Copy Constructor
// value::value(value &other)
// {
//     _data     = other.get_data();
//     _grad     = other.get_grad();
//     _children = other.get_children();
//     _graph    = other.get_graph();
// }
// // Move constructor
// value::value(value&& other) noexcept : _data(std::move(other._data)), 
//                                  _grad(std::move(other._grad)), 
//                                  _children(std::move(other._children)), 
//                                  _graph(std::move(other._graph)) 
//     {
//         other._data = 0;
//         other._grad = 0;
//         other._children.clear();
//     }
                            

// // Copy  Assignment operator
// value & value::operator=(value &other)
// {
//   if (&other == this) return *this;
//   _data = other.get_data();
//   _grad = other.get_grad();
//   _children = other.get_children();
//   _graph = other.get_graph();
//   return *this;
// }

// // Move Assignment operator
// value & value::operator=(value&& other)
// {
//   std::swap(_data, other._data);
//   std::swap(_grad, other._grad);
//   std::swap(_children, other._children);
//   std::swap(_graph, other._graph);
//   return *this;
// }

// // Addition, subtraction and multiplication MAYBE DO A TEMPLATE OF THE BELOW ONES
// value value::operator+(const value &other) const
// {
//     std::shared_ptr<value_graph> vg = _graph.lock(); // get a shared_ptr to the value graph
//     double new_grad{1};
//     value new_object{get_data()+ other.get_data(), new_grad , vg};
//     new_object._children.push_back(std::make_shared<value>(*this)); // add a shared_ptr to this object to result's children vector
//     new_object._children.push_back(std::make_shared<value>(other)); // add a shared_ptr to val object to result's children vector
//     vg -> add_value(std::make_shared<value>(new_object));
//     return new_object;
// }