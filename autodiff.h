#ifndef DIFF_H
#define DIFF_H
#include "model.h"

// Implementation of Interface of differentiation

// class AutoDiff
// {
// public:
//     static value compute_derivative(const IFunction& func, const value& x)
//     {
//         value y = func.eval(x);
//         y.backward();
//         return x;
//     }
// };

class LossFunction {
public:
    virtual ~LossFunction() = default;
    virtual value operator()(const value& model_output, const double& target) const =0;
};

class RMSE: public LossFunction
{
    public:
        value operator()(const value& model_output, const double& target) const override
        {
            value target_val{static_cast<double>(target)};
            value diff{model_output - target_val};
            // diff = pow(diff,2);
            // diff = pow(diff,0.5);               
            // error = error/static_cast<double>(target.size());
            // error = pow(error,0.5);
            return (diff*diff);
        }
};
#endif