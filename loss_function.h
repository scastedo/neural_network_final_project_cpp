#ifndef LOSS_FUNCTION_H
#define LOSS_FUNCTION_H
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "value.h"
template <typename T = double>
class ILossFunction
{
    public:
        virtual ~ILossFunction() = default;
        virtual value operator()(const value& model_output, const T& target) const = 0;
};
namespace loss_functions{

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
        HuberLoss(): huber_delta_(huber_validation()){}
        value operator()(const value& model_output, const T& target) const override
        {
            value target_val{static_cast<double>(target)};
            value diff{model_output - target_val};
            value abs_diff{diff < 0 ? -diff : diff}; // compute absolute difference
            
            if(abs_diff <= huber_delta_)
                return 0.5 * diff * diff;
            else
                return huber_delta_ * (abs_diff - 0.5 * huber_delta_);
        }
    private:
        value huber_delta_{};
    
        double huber_validation()
        {
            double input;
            bool verifiction{false};
            while (!verifiction)
            {
            std::cout << "Enter Huber Delta Hyperparameter (usually 1.3): ";
            std::string input_string;
            std::getline(std::cin, input_string);
            while (!isFloat(input_string))
            {
                std::cout << "Sorry invalid, please enter a positive number: ";
                std::cin.clear();
                std::getline(std::cin, input_string);
            }
            input = std::stod(input_string);
            if (input>=0){verifiction=true;}
            else {std::cout<<"Out of Bounds, try again";}
            }
            return input;
        }
        bool isFloat(std::string myString ) 
        {
            std::istringstream iss(myString);
            float f;
            iss >> std::noskipws >> f; // noskipws considers leading whitespace invalid
            // Check the entire string was consumed and if either failbit or badbit is set
            return iss.eof() && !iss.fail(); 
        }
};
}
#endif