#ifndef VAL_H
#define VAL_H
#include<cmath>
#include<iostream>
// Value Class for scalar autograd engine

class value
{
  private:
    double data{0};
    double grad{1};
    double _backward_grad{1};
    std::string previous_node{""};
    std::string previous_op{""};

  public:
    //default constructor
    value()= default;
    // Parametrised constructor
    value(double d, double g, double bg, std::string pn, std::string po) :
        data{d}, grad{g}, _backward_grad{bg},
        previous_node{pn}, previous_op{po}{}
    // Destructor  
    ~value() {std::cout<< "Value destructor called"<<std::endl;}    
};

#endif
