#ifndef MOD_H
#define MOD_H
#include "value.h"

template <typename T>
class Model {
public:
  virtual ~Model() = default;

  virtual value operator()(const std::vector<value>& params, const T& input) const = 0;
};

template <typename T>
class PolynomialModel : public Model<T> {
public:
  PolynomialModel(size_t degree) : degree_(degree) {}

  value operator()(const std::vector<value>& params, const T& input) const override {
    value result{0};
    value input_val{static_cast<double>(input)};
    value term{1};
    // Calculate the polynomial value using the given parameters and input
    for (size_t j = 0; j <= degree_; j++)
    {
      result = result + params[j] * term;
      term = term * input_val;
    }
    return result;
  }

private:
  size_t degree_;
};



#endif