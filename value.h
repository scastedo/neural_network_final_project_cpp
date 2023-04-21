#ifndef VAL_H
#define VAL_H
#include<cmath>
#include<iostream>
// Value Class for scalar autograd engine

class value
{
  
  private:
    double _data{0};
    double _grad{1};
    double _backward_grad{1};
    std::string _previous_node{""};
    std::string _previous_op{""};

  public:
    //default constructor
    value()= default;
    // Parametrised constructor
    value(double d, double g, double bg, std::string pn, std::string po) :
        _data{d}, _grad{g}, _backward_grad{bg},
        _previous_node{pn}, _previous_op{po}{}
    // Destructor  
    ~value() {std::cout<< "Value destructor called"<<std::endl;}


    // Accessor
    double  data() const{return _data;} 
    double grad() const{return _grad;}
    std::string prev_node() const{return _previous_node;}
    std::string prev_op() const{return _previous_op;}  

};

#endif
