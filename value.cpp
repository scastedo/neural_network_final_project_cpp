#include "value.h"

/////////////////////////////
/////////////////////////////
//////// Private Value //////
/////////////////////////////

std::ostream & operator<<(std::ostream &os, const _value &val)
{
  os << "Value(" << val.get_data() << ", " << val.get_grad() << ")";
  return os;
}

// parametrised constructor
_value::_value(const double& d, const std::vector<std::shared_ptr<_value>>& parents):
  _data{d}, _parents{parents}{}

_value::_value(const double& d): _data{d}{}

// RESETTING GRAD
void _value::reset_all_grad()
{
  auto order = build_topo();
  for (auto n=order.rbegin(); n!=order.rend(); ++n)
      (*n)->reset_grad();
}
// TOPOLOGICAL SORT Directed acyclic graph
std::vector<_value*> _value::build_topo()
{
    std::vector<_value*> sorted_nodes;
    std::set<_value*> visited_nodes;
    std::function<void(_value*)> _build_topo = [&](_value* node)
    {
        if (visited_nodes.find(node) != visited_nodes.end())
            return;

        visited_nodes.insert(node);
        for (auto& parent_ptr : node->get_parents())
            _build_topo(parent_ptr.get());
        sorted_nodes.push_back(node);
    };
    _build_topo(this);
    return sorted_nodes;
}
// BACKPROPOGATION
void _value::backward()
{
  auto order = build_topo();
  // Set dx/dx=1
  _grad = 1;
  for (auto n=order.rbegin(); n!=order.rend(); ++n)
      (*n)->_backward();
}

/////////////////////////////
/////////////////////////////
////////  Public Value //////
/////////////////////////////
std::ostream & operator<<(std::ostream &os, const value &val)
{
  os << "Value(" << val.get_data() << ", " << val.get_grad() << ")";
  return os;
}
// Constructors
value::value(const double& data, const std::vector<std::shared_ptr<_value>>& parents): // Private constructor
  _ptr{std::make_shared<_value>(data, parents)}{}
value::value(const double& d): _ptr{std::make_shared<_value>(d)}{} //Public constructor
value::value(const value& other): _ptr{other._ptr}{} //copy constructor
value::value(value&& other): _ptr{other._ptr}{other._ptr = nullptr;} //move constructor
value & value::operator=(const value &other) //Copy assignment
{
  if (&other == this) return *this;
  _ptr = other._ptr;
  return *this;
}
value& value::operator=(value&& other) //move assignment
{
  std::swap(_ptr, other._ptr);
  return *this;
}


value value::operator+(const value& other) const
{
    value out(get_data() + other.get_data(), {get_ptr(), other.get_ptr()}); // adds values and adds parents 

    //Setting up the backward function for later use
    std::weak_ptr<_value> this_weak_ptr = get_ptr();
    std::weak_ptr<_value> other_weak_ptr = other.get_ptr();
    std::weak_ptr<_value> out_weak_ptr = out.get_ptr();
    auto _back = [=]() 
    {    // Lambda closure only contains weak pointers
        if(auto this_ptr = this_weak_ptr.lock()) {
            this_ptr->get_grad() +=  out_weak_ptr.lock()->get_grad();
        }
        if(auto other_ptr = other_weak_ptr.lock()) {
            other_ptr->get_grad() +=  out_weak_ptr.lock()->get_grad();
        }
    };
    out.set_backward(_back);
    return out;
}

value value::operator+(const double& other) const 
{
    auto temp = value(other);
    return operator+(temp);
}
value value::operator+=(const value& other)
{
    *this = *this + other;
    return *this;
}
value value::operator+=(const double& other)
{
    auto temp = value(other);
    *this = *this + temp;
    return *this;
}


value value::operator-(const value& other) const 
{
    auto out = value(get_data() - other.get_data(), {get_ptr(), other.get_ptr()});
    

    std::weak_ptr<_value> this_weak_ptr = get_ptr();
    std::weak_ptr<_value> other_weak_ptr = other.get_ptr();
    std::weak_ptr<_value> out_weak_ptr = out.get_ptr();
    auto _back = [=]() 
    {    // Lambda closure only contains weak pointers
        if(auto this_ptr = this_weak_ptr.lock()) {
            this_ptr->get_grad() += out_weak_ptr.lock()->get_grad();
        }
        if(auto other_ptr = other_weak_ptr.lock()) {
            other_ptr->get_grad() += out_weak_ptr.lock()->get_grad();
        }
    };

    out.set_backward(_back);
    return out;
}

value value::operator-(const double& other) const
{
    auto temp = value(other);
    return operator-(temp);
}
value value::operator-=(const value& other)
{
    *this = *this - other;
    return *this;
}
value value::operator-=(const double& other)
{
    auto temp = value(other);
    *this = *this - temp;
    return *this;
}

value value::operator*(const value& other) const
{
    auto out = value(get_data() * other.get_data(),{get_ptr(), other.get_ptr()});


    std::weak_ptr<_value> this_weak_ptr = get_ptr();
    std::weak_ptr<_value> other_weak_ptr = other.get_ptr();
    std::weak_ptr<_value> out_weak_ptr = out.get_ptr();
    auto _back = [=]() 
    {    // Lambda closure only contains weak pointers
        if(auto this_ptr = this_weak_ptr.lock()) {
            this_ptr->get_grad() += other_weak_ptr.lock()->get_data() * out_weak_ptr.lock()->get_grad();
        }
        if(auto other_ptr = other_weak_ptr.lock()) {
            other_ptr->get_grad() += this_weak_ptr.lock()->get_data() * out_weak_ptr.lock()->get_grad();
        }
    };
    out.set_backward(_back);
    return out;
}

value value::operator*(const double& other) const
{
    auto temp = value(other);
    return operator*(temp);
}
value value::operator*=(const value& other)
{
    *this = *this * other;
    return *this;
}
value value::operator*=(const double& other)
{
    auto temp = value(other);
    *this = *this * temp;
    return *this;
}
value value::operator/(const value& other) const
{
    auto temp = pow(other, -1);
    return operator*(temp);
}

value value::operator/(const double& other) const
{
    auto temp = value(other);
    return operator/(temp);
}
value value::operator/=(const value& other)
{
    *this = *this / other;
    return *this;
}
value value::operator/=(const double& other)
{
    auto temp = value(other);
    *this = *this / temp;
    return *this;
}


value value::operator-()
{
    return operator*(-1);
}

bool value::operator==(const value& other) const
{
    return get_data() == other.get_data();
}

bool value::operator<(const value& other) const
{
    return get_data() < other.get_data();
}

bool value::operator>(const value& other) const
{
    return get_data() > other.get_data();
}

bool value::operator<=(const value& other) const
{
    return get_data() <= other.get_data();
}

bool value::operator>=(const value& other) const
{
    return get_data() >= other.get_data();
}