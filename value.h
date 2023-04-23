#ifndef VAL_H
#define VAL_H
#include<cmath>
#include<vector>
#include<iostream>
#include<memory>
#include<sstream>
#include<iomanip>
#include<algorithm>
// Value Class for scalar autograd engine

class value_graph; // forward declaration

class value
{
  friend std::ostream & operator<<(std::ostream &, const value &);
  
  private:
    double _data{0};
    double _grad{0};
    std::vector<std::shared_ptr<value>> _children;
    std::vector<std::shared_ptr<value>> _parents;
    std::weak_ptr<value_graph> _graph;
    std::string _name{""};
  public:
    //default & parametrised & copy constructor
    value()= default;
    value(double, double, std::weak_ptr<value_graph>);
    value(value&);
    // Destructor  
    ~value() = default;

    // Accessor
    double get_data() const{return _data;} 
    double get_grad() const{return _grad;}
    std::vector<std::shared_ptr<value>> get_children() const {return _children;}
    std::vector<std::shared_ptr<value>> get_parents() const{return _parents;}
    std::weak_ptr<value_graph> get_graph() const{return _graph;}

    void add_children(const std::shared_ptr<value>&);
    void add_parents(const std::shared_ptr<value>&);
    value& operator=(value &);
    value operator+(value&);
    value operator-(value&);
    value operator*(value&);
};

class value_graph {
private:
    std::vector<std::shared_ptr<value>> _values{}; // collection of all value objects created
public:
  value_graph() = default;
  void print_graph() const;
  void add_value(const std::shared_ptr<value>&);
  void remove_value(const std::shared_ptr<value>&);
  std::vector<std::shared_ptr<value>> get_values() const{return _values;}
};

#endif
