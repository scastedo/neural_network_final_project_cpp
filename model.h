#ifndef MOD_H
#define MOD_H
#include "value.h"
#include <string>
// Interface of model
template <typename T = double>
class IModel {
  public:
    virtual ~IModel() = default;
    virtual value operator()(const std::vector<value>& params, const T& input) const = 0;
    virtual size_t num_params() const = 0;
    virtual std::string representation(const std::vector<value>& params) const = 0; // new method
    // Defining the output of the data of the model. F(x) = Y
    std::vector<double> output_model(const std::vector<value>& parameters, const std::vector<double>& inputs) const
    {
      std::vector<double> outputs(inputs.size()); 
      std::transform(inputs.begin(), inputs.end(), outputs.begin(), 
      [&parameters, this](const double& input)
      {
          return (*this)(parameters, input).get_data();
      });
      return outputs;
    }
};
namespace models{
// defining the polynomial model
template <typename T = double>
class PolynomialModel : public IModel<T> {
  public:
    PolynomialModel()
    {
      degree_ = degree_validation(); 
    }
    // Function that returns poly model
    value operator()(const std::vector<value>& params, const T& input) const override
    {
      // Extra error check
      if(params.size() != num_params())
      {
        throw std::invalid_argument("Incorrect number of parameters for PolynomialModel");
      }
      value result{0};
      value input_val{static_cast<double>(input)};
      value term{1};
      for (size_t j = 0; j <= degree_; j++)
      {
        result += params[j] * term;
        term *= input_val;
      }
      return result;
    }
    //Returns number of paraeters
    size_t num_params() const override
    {
      return degree_ + 1;
    }
    // Representing model to csv file and terminal
    std::string representation(const std::vector<value>& params) const override
    {
      std::string result;
      for (size_t j = degree_+1; j >0; --j)
      {
        double param_val = params[j-1].get_data();
        // handle sign
        std::string sign = (param_val < 0) ? " - " : " + ";
        if (j -1== degree_) sign = (param_val < 0) ? "-" : "";
        param_val = std::abs(param_val);
        // handle coefficient
        std::string coefficient = (param_val == 1 && j-1 != 0) ? "" : std::to_string(param_val);
        // remove trailing zeros and decimal point for integer coefficients
        size_t idx = coefficient.find_last_not_of('0');
        if (idx != std::string::npos && coefficient[idx] == '.')
        {
          coefficient.erase(idx + 1, std::string::npos);
        }
        // handle variable and power
        std::string variable;
        if (j-1 == 0) variable = "";
        else if (j-1 == 1) variable = "x";
        else variable = "x^" + std::to_string(j-1);
        result += sign + coefficient + variable;
      }
      return result;
    }
  private:
    size_t degree_;
    //Determining polynomial degree
    size_t degree_validation()
    {
      std::cout<<"Enter Degree of Polynomial: ";
      size_t input;
      std::string input_string;
      std::getline(std::cin, input_string);
      while (!std::all_of(input_string.begin(),
          input_string.end(), ::isdigit) || input_string.empty()|| std::stoi(input_string) >5)
      {
          std::cout << "Sorry invalid, please enter a positive integer 0-5: ";
          std::cin.clear();
          std::getline(std::cin, input_string);
      }
      input = std::stoull(input_string);
      return input;  
    } 

};

//Implementing exponential model
template <typename T = double>
class ExponentialModel : public IModel<T>
{
  public:
    //Giving function of exponential model
    value operator()(const std::vector<value>& params, const T& input) const override
    {
      if(params.size() != num_params())
      {
        throw std::invalid_argument("Incorrect number of parameters for PolynomialModel");
      }
      value input_val{static_cast<double>(input)};
      return params[0] * exp(params[1] * input_val);
    }
    //Returning number of parameters
    size_t num_params() const override
    {
      return 2; // Base and exponent
    }
    //Printing representation
    std::string representation(const std::vector<value>& params) const override
    {
      return std::to_string(params[0].get_data()) + " * e^(" + std::to_string(params[1].get_data()) + " * x)";
    }
};

template <typename T = double>
class LogisticModel : public IModel<T>
{
  public:
    //Giving function of logistic model
    value operator()(const std::vector<value>& params, const T& input) const override
    {
      if(params.size() != num_params())
      {
        throw std::invalid_argument("Incorrect number of parameters for PolynomialModel");
      }
      value input_val{static_cast<double>(input)};
      return params[0] / (1.0 + exp(-1*params[1] * (input_val - params[2])));
    }
    size_t num_params() const override
    {
      return 3; // Maximum value, growth rate, and midpoint
    }
    //Representation
    std::string representation(const std::vector<value>& params) const override
    {
      return std::to_string(params[0].get_data()) + " / (1 + e^-(" + std::to_string(params[1].get_data()) + " * (x - " + std::to_string(params[2].get_data()) + ")))";
    }
  };
}
#endif