#ifndef IO_H
#define IO_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <limits>
#include <cmath>
#include <cctype>
#include <iomanip>

// Interface for IO operations
class IDataIO {
public:
    virtual std::pair<std::vector<double>, std::vector<double>> importDataFromSource() = 0;
    virtual void outputData(const std::vector<double>& data, const std::vector<double>& target) = 0;
    virtual void save_file(const std::vector<double>& input, const std::vector<double>& observed, const std::vector<double>& predicted, const std::string& representation) = 0;
};

// CSVDataIO class implementing the IDataIO interface
class CSVDataIO : public IDataIO {
private:
    std::string inputFile;  // Store the input filename
public:
    std::pair<std::vector<double>, std::vector<double>> importDataFromSource() override;
    std::pair<std::vector<double>, std::vector<double>> importData(const std::string& filename);
    std::pair<std::vector<double>, std::vector<double>> manualInput();
    // Function to calculate the R-squared value
    double calculateRSquared(const std::vector<double>& observed, const std::vector<double>& predicted);
// Function to print vectors side by side and the R-squared value
    void outputData(const std::vector<double>& data, const std::vector<double>& target) override;
    void save_file(const std::vector<double>& input, const std::vector<double>& observed, const std::vector<double>& predicted, const std::string& representation) override;
};
#endif

 
 
