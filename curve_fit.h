#ifndef CURV_H
#define CURV_H
template <typename LossFunc, typename Model, typename Optimiser>
class CurveFitProgram {
public:
  CurveFitProgram(size_t iter, double lr) : 
    loss_func(std::make_shared<LossFunc>()),
    model(std::make_shared<Model>()),
    optimiser(std::make_shared<Optimiser>()),
    max_iterations(iter),
    learning_rate(lr)
    {
        if (max_iterations <= 0 || learning_rate <= 0) {
            throw std::invalid_argument("Invalid parameters for CurveFitProgram");
        }
    }
    void run() {
        importData();
        initialiseParams();
        fitCurve();
        exportData();
    }
    void importData() {
        std::pair<std::vector<double>, std::vector<double>> importedData = csvIO.importDataFromSource();
        inputs = importedData.first;
        targets = importedData.second;
    }
    void testData(){
      inputs = {1,2,3,4,5};
      targets = {1.5,4.2,9.3,15.8,25.1};
    }

    void initialiseParams() {
        initial_params.resize(model->num_params(), value{1}); //Initialises all parameters to 1
        std::cout << "============================================" << std::endl;
        std::cout << "Initial Representation: " << model->representation(initial_params) <<std::endl <<std::endl;
        csvIO.outputData(model->output_model(initial_params, inputs), targets);
    }

    void fitCurve() {
        optimised_params = optimiser->fit(initial_params, max_iterations, learning_rate, inputs, targets, model, loss_func);
        std::cout<< "Final Representation: " << model->representation(optimised_params) <<std::endl <<std::endl;
        csvIO.outputData(model->output_model(optimised_params, inputs), targets);
    }

    void exportData() {
        csvIO.save_file(inputs, targets,model->output_model(optimised_params, inputs), model->representation(optimised_params));
    }

private:
    CSVDataIO csvIO;
    std::vector<double> inputs, targets;
    std::shared_ptr<LossFunc> loss_func;
    std::shared_ptr<Model> model;
    std::shared_ptr<Optimiser> optimiser;
    std::vector<value> initial_params, optimised_params;
    size_t max_iterations;
    double learning_rate;  
};
#endif