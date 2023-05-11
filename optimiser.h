#ifndef OPT_H
#define OPT_H
#include "value.h"
#include "loss_function.h"
#include "model.h"

template <typename T = double>
class IOptimizer {
public:
    IOptimizer(double learning_rate) : learning_rate_(learning_rate) {}

    virtual std::vector<value> fit(const std::vector<value>& , size_t iterations, double learning_rate) = 0;

protected:
    double learning_rate_;
};


template <typename T = double>
class CurveFitter: public IOptimizer<T>
{
public:
  CurveFitter(const std::vector<T>& inputs, const std::vector<T>& targets, std::shared_ptr<IModel<T>> model, std::shared_ptr<ILossFunction<T>> loss_function)
    : IOptimizer<T>(0.0), inputs_(inputs), targets_(targets), model_(model), loss_function_(loss_function) {}

  std::vector<value> fit(const std::vector<value>& initial_params, size_t max_iterations, double learning_rate)
  {
    std::vector<value> params = initial_params;
    for (size_t i = 0; i < max_iterations; ++i) 
    {
      for (auto &param : params){param.zero_grad_all();}
      value loss{0};
      
      //Forward Pass and Calculate Loss
      for(size_t j = 0; j<inputs_.size();j++)
      {
        value model_output = (*model_)(params, inputs_[j]);
        loss = loss + (*loss_function_)(model_output, targets_[j]);
      }
      loss = loss/(static_cast<double>(inputs_.size()));
      loss = pow(loss,0.5);

      // Backward Pass and Calculate gradient
      loss.backward();
      // std::cout<< loss << std::endl;

      // Update parameters
      for (size_t k = 0; k < params.size(); ++k)
      {
        params[k] = params[k] -  learning_rate * params[k].get_grad();
      }
    }
    return params;
  }
  std::vector<value> adam_fit(const std::vector<value>& initial_params, size_t max_iterations, double learning_rate)
  {
    std::vector<value> params = initial_params;
    std::vector<double> mt(params.size(), 0.0);
    std::vector<double> vt(params.size(), 0.0);
    double beta1 = 0.9, beta2 = 0.999, epsilon = 1e-8;
    for (size_t i = 0; i < max_iterations; ++i)
    {
      for (auto &param : params){param.zero_grad_all();}
      value loss{0};
      for(size_t j = 0; j<inputs_.size();j++)
      {
        value model_output = (*model_)(params, inputs_[j]);
        loss = loss + (*loss_function_)(model_output, targets_[j]);
      }

      // Compute gradients
      loss.backward();

      // Update parameters with ADAM
      for (size_t k = 0; k < params.size(); ++k) {
        double gk = params[k].get_grad() / static_cast<double>(inputs_.size());
        mt[k] = beta1 * mt[k] + (1.0 - beta1) * gk;
        vt[k] = beta2 * vt[k] + (1.0 - beta2) * gk * gk;
        double mhat = mt[k] / (1.0 - std::pow(beta1, i+1));
        double vhat = vt[k] / (1.0 - std::pow(beta2, i+1));
        params[k] = params[k] - learning_rate * mhat / (std::sqrt(vhat) + epsilon);
      }
    }
    return params;
  }

private:
  const std::vector<T>& inputs_;
  const std::vector<T>& targets_;
  std::shared_ptr<IModel<T>> model_;
  std::shared_ptr<ILossFunction<T>> loss_function_;
};

#endif