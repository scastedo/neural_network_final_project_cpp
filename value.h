#ifndef VAL_H
#define VAL_H
#include<iostream>
#include<cmath>
#include<vector>
#include<utility>
#include<functional>
#include<set>
#include<memory>
// Value Class for scalar autograd engine

// forward declarations
class _value;
class value;

// Hidden value class to manage data 
class _value
{
  friend class value;
  friend std::ostream & operator<<(std::ostream &, const _value &);//printing
  
  private:
    double _data{0};
    double _grad{0};
    std::vector<std::shared_ptr<_value>> _parents{};
    std::function<void()> _backward = [](){return;};

  public:
  // Constructors
  _value(const double&, const std::vector<std::shared_ptr<_value>>&); //param construct
  _value(const double&); //annother param construct
  _value() = default; // default
  ~_value() = default;// destruct

  // Cannot move or delete!
  _value(const _value&) = delete;// copy construct
  _value(_value&&)= delete;// move construct
  _value& operator=(const _value& other) = delete; // copy assign 
  _value& operator=(_value && other) = delete; //move assign

  // Accessors
  const double& get_data() const { return _data; }
  const double& get_grad() const { return _grad; }
  double& get_data() { return _data; }
  double& get_grad() { return _grad; }
  const std::vector<std::shared_ptr<_value>>& get_parents() const{return _parents;}

  // Setters: reset individual grad, reset all grad, set _backward value
  void reset_grad() {_grad = 0;}
  void reset_all_grad();
  void set_backward(const std::function<void()>& func) { _backward = func; }
  // Topological sort
  std::vector<_value*> build_topo();
  // Backpropagation
  void backward();
  void descend_grad(const double&);
};

// Publically facing value class
class value
{ 
  // Output value to terminal
  friend std::ostream& operator<<(std::ostream& os, const value&);
  // Scalar power 
  friend value pow(const value& val,  const double& exp_term)
  {
    auto out = value(std::pow(val.get_data(), exp_term), {val.get_ptr()});

    std::weak_ptr<_value> val_weak_ptr = val.get_ptr();
    std::weak_ptr<_value> out_weak_ptr = out.get_ptr();

    auto _back = [=]() {
        if(auto val_ptr = val_weak_ptr.lock()) {
            val_ptr->get_grad() += (exp_term * std::pow(val_ptr->get_data(), exp_term - 1)) * out_weak_ptr.lock()->get_grad();
        }
    };
    out.set_backward(_back);
    return out;
  }
  friend value exp(const value& val)
  {
    auto out = value(std::exp(val.get_data()), {val.get_ptr()});

    std::weak_ptr<_value> val_weak_ptr = val.get_ptr();
    std::weak_ptr<_value> out_weak_ptr = out.get_ptr();

    auto _back = [=]() {
        if(auto val_ptr = val_weak_ptr.lock()) {
            val_ptr->get_grad() += std::exp(val_ptr->get_data()) * out_weak_ptr.lock()->get_grad();
        }
    };
    out.set_backward(_back);
    return out;
  }
  // Natural Logarithm function
friend value log(const value& val)
{
  auto out = value(std::log(val.get_data()), {val.get_ptr()});

  std::weak_ptr<_value> val_weak_ptr = val.get_ptr();
  std::weak_ptr<_value> out_weak_ptr = out.get_ptr();

  auto _back = [=]() {
      if(auto val_ptr = val_weak_ptr.lock()) {
          val_ptr->get_grad() += (1.0 / val_ptr->get_data()) * out_weak_ptr.lock()->get_grad();
      }
  };
  out.set_backward(_back);
  return out;
}
    
  // Scalar arithmatic
  friend value operator+(double num, const value& val) {return val + num;}
  friend value operator-(double num, const value& val) {return val - num;}
  friend value operator*(double num, const value& val) {return val * num;}
  friend value operator/(double num, const value& val) {return val / num;}

  private:
   std::shared_ptr<_value> _ptr = nullptr;
   value(const double&, const std::vector<std::shared_ptr<_value>>&); // Private parametric constructor accessed by arithmetic to add parents
  public:
    // constructors(param and default, copy and move), destructors, 
   value() {_ptr = std::make_shared<_value>(0);} // default
   value(const double&); //Parametric
   value(const value&);// copy
   value(value&&);// move
   ~value(){_ptr = nullptr;};// destruct

    // Assignment operator (copy and move)
    value& operator=(const value&);
    value& operator=(value&&);
    
    // Accessing hidden _Value class
    const double& get_data() const { return _ptr->get_data();}
    const double& get_grad() const { return _ptr->get_grad();}
    double& get_data() { return _ptr->get_data(); }
    double& get_grad() { return _ptr->get_grad(); }
    const std::vector<std::shared_ptr<_value>>& get_parents() const{return _ptr->get_parents();}
    void zero_grad() const { _ptr->reset_grad(); }
    void zero_grad_all() const { _ptr->reset_all_grad(); }
    void set_backward(std::function<void()> func) const { _ptr->set_backward(func); }
    void backward() const { _ptr->backward(); }
    void descend_grad(const double& learning_rate) const { _ptr->descend_grad(learning_rate); }
    std::vector<_value*> build_topo() const { return _ptr->build_topo(); }
    
    // Accessors
    std::shared_ptr<_value> get_ptr() const { return _ptr; }
    // RELU

    // Operations
    value operator+(const value&) const;
    value operator+(const double&) const;
    value operator+=(const value&);
    value operator+=(const double&);

    value operator-(const value&) const;
    value operator-(const double&) const;
    value operator-=(const value&);
    value operator-=(const double&);

    value operator*(const value&) const;
    value operator*(const double&) const;
    value operator*=(const value&);
    value operator*=(const double&);
    value operator/(const value&) const;
    value operator/(const double&) const;
    value operator/=(const value&);
    value operator/=(const double&);
    value operator-();

    // Comparison operators
    bool operator==(const value&) const; 
    bool operator<(const value&) const;
    bool operator>(const value&) const;
    bool operator<=(const value&) const;
    bool operator>=(const value&) const;
};
#endif
