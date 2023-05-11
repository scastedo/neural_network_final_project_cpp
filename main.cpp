// PHYS 30762 Programming in C++
// Assignment Final Project
// Sebastian Castedo 12 April 2023 

#include "value.h"
#include "autodiff.h"
#include "curve_fitter.h"
#include "model.h"
#include "optimiser.h"
// #include "likelihood_interface.h"

// int main()
// {
//     value a{2.0};
//     value b{-3.0};
//     value c{10.0};
//     value e{a*b};
//     value d{e/c};
//     value f{2.0};
//     value L{d*f};
//     L.backward();

//     std::cout << a << " | " << a.get_ptr() << "\n"
//               << b << " | " << b.get_ptr() << "\n"
//               << c << " | " << c.get_ptr() << "\n"
//               << d << " | " << d.get_ptr() << "\n"
//               << e << " | " << e.get_ptr() << "\n"
//               << f << " | " << f.get_ptr() << "\n"
//               << L << " | " << L.get_ptr() << "\n";
//     std::cout << "\n";
//     return 0;
// }


// int main()
// {
    
//     // Want to implement 2x^2 +3x +1

//     value a{3};
//     value b{2*pow(a,2)+3*a+1};
//     // value c{};
//     // value d{3*};
//     // value f{c+d+1};
//     b.backward();
//     std::cout << b << std::endl;
//     std::cout << a <<  std::endl;
//     // QuadraticFunction quad_func(2.0, 3.0, 1.0); // f(x) = 2x^2 + 3x + 1
//     // value x(1.0);
//     // value derivative = AutoDiff::compute_derivative(quad_func, x);
//     // std::cout << "Derivative of the function at x = " << x.get_data() << " is " << derivative.get_data() << std::endl;

//     return 0;
// }

int main()
{
    const std::vector<double> inputs = {1,2,3,4,5};
    const std::vector<double> targets = {1.5,4.2,9.3,15.8,25.1};
    const std::vector<value> initial_params = {value{1}, value{2}, value{3}, value{2}}; // Initial parameters for the quadratic model
    // int number{5};
    PolynomialModel<double> model(3);  
    // for (size_t i; i<number;i++){
    //     value temp{1};
    //     initial_params.push_back(temp);
    // }
     
    for (size_t i=0; i<inputs.size();i++){
        value model_check =  model(initial_params, inputs[i]);
        std::cout<< model_check <<std::endl;
    }   
    RMSE loss_func;
    
    size_t max_iterations = 1000;
    double learning_rate = 0.1;
    CurveFitter<double, PolynomialModel<double>, RMSE> curve_fitter(inputs, targets, model, loss_func);   
    std::vector<value> optimized_params = curve_fitter.adam_fit(initial_params, max_iterations, learning_rate);

    for (const auto& param : optimized_params) {
        std::cout << param.get_data() << ' ';
    }
    std::cout << std::endl;

    for (size_t i=0; i<inputs.size();i++){
        value model_check =  model(optimized_params, inputs[i]);
        std::cout<< model_check <<std::endl;
    }    
    return 0;
}


// int main()
// {
//     // Define the custom likelihood function using a lambda
//     auto likelihood_function = [](const std::vector<double>& position, const std::vector<double>& data) {
//         // Calculate and return the likelihood value based on position and data
//     };

//     auto likelihood = std::make_shared<LikelihoodInterface>(likelihood_function);

//     // Choose the integration method
//     double step_size = 0.1;
//     size_t num_leapfrog_steps = 10;
//     auto integration_method = std::make_shared<LeapfrogIntegrator>(step_size, num_leapfrog_steps);

//     // Import data
//     std::vector<double> data = {/*...*/};

//     // Create the AD_HMC_Sampler object
//     ad_hmc_sampler sampler(likelihood, integration_method, data, step_size);

//     // Run the HMC algorithm and get samples
//     // ...
    
// }