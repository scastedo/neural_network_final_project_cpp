#ifndef OPT_H
#define OPT_H
#include "value.h"
#include "model.h"
#include <iomanip>


template <typename T = double>
class IFitMethod 
{
  public:
      virtual std::vector<value> fit(const std::vector<value>& params, size_t iterations, double learning_rate, const std::vector<T>& inputs, const std::vector<T>& targets, std::shared_ptr<IModel<T>> model, std::shared_ptr<ILossFunction<T>> loss_function) = 0;
  protected:
      void print_progress_bar(size_t total, size_t current, double loss) const
      {
        const size_t bar_length = 50;  // The number of boxes for 100%
        static size_t last_percentage = 0;
        
        size_t current_percentage = (current * 100) / total;

        // Print the progress bar only if the progress has increased by at least 2%,
        // or if it's the final iteration.
        if ((current_percentage >= last_percentage + 2) || (current == total))
        {
            last_percentage = current_percentage;
            size_t boxes = (current * bar_length) / total;

            std::cout << "\r[";
            for (size_t i = 0; i < bar_length; ++i)
            {
                if (i < boxes)
                {
                    std::cout << "#";
                } else {
                    std::cout << " ";
                }
            }
          std::cout << "] " << std::fixed << std::setprecision(2) << (100.0 * current) / total << "%" <<  " | Current Loss: " << loss << std::flush;
        }
        if (current == total)
        {
          std::cout<< std::endl;
          std::cout<< "********************" << std::endl;
          std::cout<< "****COMPLETED!!!****" << std::endl;
          std::cout<< "********************" << std::endl;
        }
      }
};

template <typename T = double>
class GradientDescentFit: public IFitMethod<T> {
public:
    std::vector<value> fit(const std::vector<value>& initial_params, size_t iterations, double learning_rate, const std::vector<T>& inputs, const std::vector<T>& targets, std::shared_ptr<IModel<T>> model, std::shared_ptr<ILossFunction<T>> loss_function) override {
        std::vector<value> params = initial_params;
    value loss{0};
    for (size_t i = 0; i < iterations; ++i) 
    {
      for (auto &param : params){param.zero_grad_all();}
      loss = value{0};
      
      //Forward Pass and Calculate Loss
      for(size_t j = 0; j<inputs.size();j++)
      {
        value model_output = (*model)(params, inputs[j]);
        loss += (*loss_function)(model_output, targets[j]);
      }
      loss /= (static_cast<double>(inputs.size()));
      loss = pow(loss,0.5);

      // Backward Pass and Calculate gradient
      loss.backward();
      // std::cout<< loss << std::endl;

      // Update parameters
      for (size_t k = 0; k < params.size(); ++k)
      {
        params[k] +=  - learning_rate * params[k].get_grad();
      }
      this->print_progress_bar(iterations, i, loss.get_data());
    }
    this->print_progress_bar(iterations, iterations, loss.get_data()); // Print at 100%
    return params;
    }
};

template <typename T = double>
class AdamFit: public IFitMethod<T> {
public:
    std::vector<value> fit(const std::vector<value>& initial_params, size_t iterations, double learning_rate, const std::vector<T>& inputs, const std::vector<T>& targets, std::shared_ptr<IModel<T>> model, std::shared_ptr<ILossFunction<T>> loss_function) override {
       std::vector<value> params = initial_params;
    std::vector<double> mt(params.size(), 0.0);
    std::vector<double> vt(params.size(), 0.0);
    double beta1 = 0.9, beta2 = 0.999, epsilon = 1e-8;
    value loss{0};  // Declare loss here.

    for (size_t i = 0; i < iterations; ++i)
    {
      for (auto &param : params){param.zero_grad_all();}
      loss = value{0};
      for(size_t j = 0; j<inputs.size();j++)
      {
        value model_output = (*model)(params, inputs[j]);
        loss += (*loss_function)(model_output, targets[j]);
      }

      // Compute gradients
      loss.backward();

      // Update parameters with ADAM
      for (size_t k = 0; k < params.size(); ++k) {
        double gk = params[k].get_grad() / static_cast<double>(inputs.size());
        mt[k] = beta1 * mt[k] + (1.0 - beta1) * gk;
        vt[k] = beta2 * vt[k] + (1.0 - beta2) * gk * gk;
        double mhat = mt[k] / (1.0 - std::pow(beta1, i+1));
        double vhat = vt[k] / (1.0 - std::pow(beta2, i+1));
        params[k] += - learning_rate * mhat / (std::sqrt(vhat) + epsilon);
      }
      this->print_progress_bar(iterations, i, loss.get_data());
    }
    this->print_progress_bar(iterations, iterations, loss.get_data()); // Print at 100%
    return params;
    }
};

#endif