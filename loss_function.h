#ifndef LOSS_FUNCTION_H
#define LOSS_FUNCTION_H

#include "value.h"

template <typename T>
class ILossFunction
{
    public:
        virtual ~ILossFunction() = default;
        virtual value operator()(const value& model_output, const T& target) const = 0;
};

template <typename T=double>
class MSE: public ILossFunction<T>
{
    public:
        value operator()(const value& model_output, const T& target) const override
        {
            value target_val{static_cast<double>(target)};
            value diff{model_output - target_val};
            return (diff*diff);
        }
};

template <typename T=double>
class HuberLoss: public ILossFunction<T>
{
    public:
        value operator()(const value& model_output, const T& target) const override
        {
            value delta{1.0}; // delta is a hyperparameter and can be tuned
            value target_val{static_cast<double>(target)};
            value diff{model_output - target_val};
            value abs_diff{diff < 0 ? -diff : diff}; // compute absolute difference
            
            if(abs_diff <= delta)
                return 0.5 * diff * diff;
            else
                return delta * (abs_diff - 0.5 * delta);
        }
};
#endif