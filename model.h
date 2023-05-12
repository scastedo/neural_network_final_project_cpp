#ifndef MOD_H
#define MOD_H
#include "value.h"
#include <string>

template <typename T>
class IModel {
public:
  virtual ~IModel() = default;
  virtual value operator()(const std::vector<value>& params, const T& input) const = 0;
  virtual size_t num_params() const = 0;
  virtual std::string representation(const std::vector<value>& params) const = 0; // new method
  std::vector<double> output_model(std::vector<value> parameters, std::vector<double> inputs)
  {
    std::vector<double> outputs{}; 
    for (size_t i=0; i<inputs.size();i++)
    {
        value initial_model =  (*this)(parameters, inputs[i]);
        outputs.push_back(initial_model.get_data());
    } 
    return outputs;
  }
  
};

template <typename T>
class PolynomialModel : public IModel<T> {
public:
  PolynomialModel(size_t degree) : degree_(degree) {}
  value operator()(const std::vector<value>& params, const T& input) const override {
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
  size_t num_params() const override {
    return degree_ + 1;
  }
  std::string representation(const std::vector<value>& params) const override {
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
      if (idx != std::string::npos && coefficient[idx] == '.') {
        coefficient.erase(idx + 1, std::string::npos);
      }
      
      // handle variable and power
      std::string variable;
      if (j-1 == 0) variable = "";
      else if (j-1 == 1) variable = "x";
      else variable = "x^" + std::to_string(j);
      
      result += sign + coefficient + variable;
    }
    return result;
}
private:
  size_t degree_;
};

template <typename T>
class ExponentialModel : public IModel<T> {
public:
  value operator()(const std::vector<value>& params, const T& input) const override {
    value input_val{static_cast<double>(input)};
    return params[0] * exp(params[1] * input_val);
  }
  size_t num_params() const override {
    return 2; // Base and exponent
  }
  std::string representation(const std::vector<value>& params) const override {
    return std::to_string(params[0].get_data()) + " * e^(" + std::to_string(params[1].get_data()) + " * x)";
  }

};

template <typename T>
class LogisticModel : public IModel<T> {
public:
  value operator()(const std::vector<value>& params, const T& input) const override {
    value input_val{static_cast<double>(input)};
    return params[0] / (1.0 + exp(-1*params[1] * (input_val - params[2])));
  }
  size_t num_params() const override {
    return 3; // Maximum value, growth rate, and midpoint
  }
  std::string representation(const std::vector<value>& params) const override {
    return std::to_string(params[0].get_data()) + " / (1 + e^-(" + std::to_string(params[1].get_data()) + " * (x - " + std::to_string(params[2].get_data()) + ")))";
  }
};
  
#endif