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

// Interface for IO operations
class IDataIO {
public:
    virtual std::pair<std::vector<double>, std::vector<double>> importDataFromSource() = 0;
    virtual void outputData(const std::vector<std::vector<std::string>>& data) = 0;
};

// CSVDataIO class implementing the IDataIO interface
class CSVDataIO : public IDataIO {
public:
    std::pair<std::vector<double>, std::vector<double>> importDataFromSource() override;
    std::pair<std::vector<double>, std::vector<double>> importData(const std::string& filename);
    std::pair<std::vector<double>, std::vector<double>> manualInput();

    void outputData(const std::vector<std::vector<std::string>>& data) override;
};
#endif

 
 
