#include "value.h"
// Value Graph adding new vals
void value_graph::add_value(const std::shared_ptr<value>& val)
{
  _values.push_back(val);
}
void value_graph::remove_value(const std::shared_ptr<value>& val)
{
  _values.erase(std::remove(_values.begin(), _values.end(), val), _values.end());
}

void value_graph::print_graph() const
{



}


void value::add_children(const std::shared_ptr<value>& val)
{
  _children.push_back(val);
}
void value::add_parents(const std::shared_ptr<value>& val)
{
  _parents.push_back(val);
}

// Overload insertion to output stream for value
std::ostream & operator<<(std::ostream &os, const value &val)
{
  os << "Value(" << val.get_data() << ", " << val.get_grad() << ")";
  return os;
}

// Parametrised Constructor
value::value(double d, double g, std::weak_ptr<value_graph> vg) :
  _data{d} , _grad{g}, _graph{vg}
{
  if (auto graph = _graph.lock()) // check if weak_ptr is still valid
  {
    graph->add_value(std::shared_ptr<value>(this)); // add shared_ptr to the graph
  }
}


// Copy Constructor
value::value(value& other)
{
  _data = other.get_data();
  _grad = other.get_grad();
  _graph = other.get_graph();
  other.add_parents(std::shared_ptr<value>(this)); 
  _children.push_back(std::shared_ptr<value>(&other));  
  if (auto vg = _graph.lock()) {
      vg->add_value(std::shared_ptr<value>(this));
  }
}


// Addition, subtraction and multiplication MAYBE DO A TEMPLATE OF THE BELOW ONES
value value::operator+(value &other)
{
  double new_grad{1};
  value new_object{get_data()+ other.get_data(), new_grad , _graph};
  new_object.add_children(std::shared_ptr<value>(this)); // add a shared_ptr to this object to result's children vector
  new_object.add_children(std::shared_ptr<value>(&other)); // add a shared_ptr to val object to result's children vector
  other.add_parents(std::shared_ptr<value>(&new_object));
  add_parents(std::shared_ptr<value>(&new_object));
  if (auto vg = _graph.lock()) {
      vg->add_value(std::shared_ptr<value>(&new_object));
  }
  return new_object;
}
value value::operator-(value &other)
{
  double new_grad{1};
  value new_object{get_data()- other.get_data(), new_grad , _graph};
  new_object.add_children(std::shared_ptr<value>(this)); // add a shared_ptr to this object to result's children vector
  new_object.add_children(std::shared_ptr<value>(&other)); // add a shared_ptr to val object to result's children vector
  other.add_parents(std::shared_ptr<value>(&new_object));
  add_parents(std::shared_ptr<value>(&new_object));
  if (auto vg = _graph.lock()) {
      vg->add_value(std::shared_ptr<value>(&new_object));
  }
  return new_object;
}
value value::operator*(value &other)
{
  double new_grad{1};
  value new_object{get_data()* other.get_data(), new_grad , _graph};
  new_object.add_children(std::shared_ptr<value>(this)); // add a shared_ptr to this object to result's children vector
  new_object.add_children(std::shared_ptr<value>(&other)); // add a shared_ptr to val object to result's children vector
  other.add_parents(std::shared_ptr<value>(&new_object));
  add_parents(std::shared_ptr<value>(&new_object));
  if (auto vg = _graph.lock()) {
      vg->add_value(std::shared_ptr<value>(&new_object));
  }
  return new_object;
}

// Copy  Assignment operator
value & value::operator=(value &other)
{
  if (&other == this) return *this;
  _data = other.get_data();
  _grad = other.get_grad();
  _graph = other.get_graph();
  _children.clear();
  _children.push_back(std::shared_ptr<value>(&other));
  other.add_parents(std::shared_ptr<value>(this)); 

  if (auto vg = _graph.lock()) {
      vg->add_value(std::shared_ptr<value>(this));
  }

  return *this;
}
