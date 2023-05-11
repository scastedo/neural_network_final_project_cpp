#ifndef MOD_H
#define MOD_H
#include "value.h"

template <typename T>
class IModel {
public:
  virtual ~IModel() = default;
  virtual value operator()(const std::vector<value>& params, const T& input) const = 0;
  virtual size_t num_params() const = 0;
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
      result = result + params[j] * term;
      term = term * input_val;
    }
    return result;
  }
  size_t num_params() const override {
    return degree_ + 1;
  }

private:
  size_t degree_;
};

#endif