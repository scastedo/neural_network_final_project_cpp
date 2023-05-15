// PHYS 30762 Programming in C++
// Assignment Final Project
// Sebastian Castedo 12 April 2023 
// Choose the model, loss function and optimisers for CurveFitProgram
// Make sure to include the <> after each of these choices.
// Adjust the hyperparameters of max iterations and learning rate
// Commented second main function that can be used to test the value class for educational purposes.
#include "value.h"
#include "loss_function.h"
#include "optimiser.h"
#include "model.h"
#include "io.h"
#include "curve_fit.h"


int main()
{
    static const size_t max_iterations{2000}; //Vary this hyperparameter if you require the algorithm to run for longer
    static const double learning_rate{0.01}; //Vary this hyperparameter to control how fast the algorithm converges
    CurveFitProgram<loss_functions::HuberLoss<>,models::PolynomialModel<>,optimisers::AdamFit<>> program_test(max_iterations,learning_rate);
    program_test.importData(); //Can also call testData() function
    program_test.initialiseParams();
    program_test.fitCurve();
    program_test.exportData();
    return 0;
}


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