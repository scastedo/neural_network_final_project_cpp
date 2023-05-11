#ifndef CURV_H
#define CURV_H
#include "loss_function.h"
#include "optimiser.h"

template <typename T = double>
class IOptimizer
{
  public:
      virtual std::vector<value> fit(const std::vector<value>& initial_params, size_t iterations, double learning_rate) = 0;
};

template <typename T = double>
class CurveFitter: public IOptimizer<T>
{
  public:
    CurveFitter(const std::vector<T>& inputs, const std::vector<T>& targets, std::shared_ptr<IModel<T>> model, std::shared_ptr<ILossFunction<T>> loss_function, std::shared_ptr<IFitMethod<T>> fit_method)
      : inputs_(inputs), targets_(targets), model_(model), loss_function_(loss_function), fit_method_(fit_method) {}

    std::vector<value> fit(const std::vector<value>& initial_params, size_t max_iterations, double learning_rate) override
    {
      return fit_method_->fit(initial_params, max_iterations, learning_rate, inputs_, targets_, model_, loss_function_);
    }

private:
  const std::vector<T>& inputs_;
  const std::vector<T>& targets_;
  std::shared_ptr<IModel<T>> model_;
  std::shared_ptr<ILossFunction<T>> loss_function_;
  std::shared_ptr<IFitMethod<T>> fit_method_;
};
#endif