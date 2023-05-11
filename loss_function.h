#ifndef LOSS_FUNCTION_H
#define LOSS_FUNCTION_H

#include "value.h"

template <typename T>
class ILossFunction {
public:
    virtual ~ILossFunction() = default;
    virtual value operator()(const value& model_output, const T& target) const = 0;
};

template <typename T=double>
class RMSE: public ILossFunction<T>
{
    public:
        value operator()(const value& model_output, const T& target) const override
        {
            value target_val{static_cast<double>(target)};
            value diff{model_output - target_val};
            return (diff*diff);
        }
};

#endif