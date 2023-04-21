#ifndef VAL_H
#define VAL_H
#include<cmath>
#include<iostream>
// Value Class for scalar autograd engine

class value
{
  
  private:
    double _data{0};

  public:
    //default constructor
    value()= default;
    // Parametrised constructor
    value(double d) :
        _data{d}{}
    // Destructor  
    ~value() {std::cout<< "Value destructor called"<<std::endl;}


    // Accessor
    double  data() const{return _data;} 
    

    // Overloading operations // Addition, subtraction and multiplication
    value operator+(const value &)const;
    value operator-(const value &)const;
    value operator*(const value &)const;


};

#endif
