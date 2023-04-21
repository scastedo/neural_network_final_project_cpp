#include "value.h"
// Addition, subtraction and multiplication
value value::operator+(const value &val)const
{
    value new_object;
    new_object = value(data()+val.data());
    return new_object;
}
value value::operator-(const value &val)const
{
    value new_object;
    new_object = value(data()-val.data());
    return new_object;
}
value value::operator*(const value &val)const
{
    value new_object;
    new_object = value(data()*val.data());
    return new_object;
}
