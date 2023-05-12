// PHYS 30762 Programming in C++
// Assignment Final Project
// Sebastian Castedo 12 April 2023 

#include "value.h"
#include "loss_function.h"
#include "optimiser.h"
#include "model.h"
#include "io.h"
#include "curve_fit.h"
// #include "likelihood_interface.h"

// int main()
// {
// //     value a{2.0};
// //     value b{-3.0};
// //     value c{10.0};
// //     value e{a*b};
// //     value d{e/c};
// //     value f{2.0};
// //     value L{d*f};
// //     L.backward();

// //     std::cout << a << " | " << a.get_ptr() << "\n"
// //               << b << " | " << b.get_ptr() << "\n"
// //               << c << " | " << c.get_ptr() << "\n"
// //               << d << " | " << d.get_ptr() << "\n"
// //               << e << " | " << e.get_ptr() << "\n"
// //               << f << " | " << f.get_ptr() << "\n"
// //               << L << " | " << L.get_ptr() << "\n";
// //     std::cout << "\n";
// // Want to implement 2x^2 +3x +1

//     // value a{3};
//     // value b{2*log(a)};
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
    CSVDataIO csvIO;
    std::pair<std::vector<double>, std::vector<double>> importedData = csvIO.importDataFromSource();
    std::vector<double>& inputs = importedData.first;
    std::vector<double>& targets = importedData.second;
    // const std::vector<double> inputs = {1,2,3,4,5};
    // const std::vector<double> targets = {1.5,4.2,9.3,15.8,25.1};
    
    auto loss_func = std::make_shared<HuberLoss<>>(); // Define Loss Function
    // auto model = std::make_shared<PolynomialModel<double>>(2); // Define Model used
    auto model = std::make_shared<ExponentialModel<double>>(); // Define Model used

    std::vector<value> initial_params(model->num_params(), value{1}); //Generate Initial parameters
    size_t max_iterations = 500; // Hyper parameter
    double learning_rate = 0.1; // Hyper parameter



    // Initial Model
    std::cout<< "============================================" << std::endl;
    std::cout<< "Initial Representation: " << model->representation(initial_params) <<std::endl <<std::endl;
    csvIO.outputData(model->output_model(initial_params, inputs), targets);

    // for (size_t i=0; i<inputs.size();i++)
    // {
    //     value initial_model =  (*model)(initial_params, inputs[i]);
    //     std::cout<< initial_model <<std::endl;
    // }
    
      

    std::shared_ptr<AdamFit<double>> adam_method = std::make_shared<AdamFit<double>>();
    CurveFitter<double> curve_fitter(inputs, targets, model, loss_func, adam_method);   
    std::vector<value> optimized_params = curve_fitter.fit(initial_params, max_iterations, learning_rate);
    
    std::cout<< "Final Representation: " << model->representation(optimized_params) <<std::endl <<std::endl;
    csvIO.outputData(model->output_model(optimized_params, inputs), targets);
    csvIO.save_file(inputs, targets,model->output_model(optimized_params, inputs), model->representation(optimized_params));

    return 0;
}