#include "io.h"

std::pair<std::vector<double>, std::vector<double>> CSVDataIO::importDataFromSource() 
{
    std::string filename;
    std::cout << "Enter the filename of CSV (leave blank for manual input): ";
    std::getline(std::cin, filename);

    if (filename.empty()) {
        return manualInput();
    } else {
        inputFile = filename;
        return importData(filename);
    }
}


std::pair<std::vector<double>, std::vector<double>> CSVDataIO::importData(const std::string& filename) {
    std::vector<double> column1;
    std::vector<double> column2;

    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file: " + filename);
    }

    std::string line;
    int lineNumber = 1;
    while (std::getline(file, line)) {
        std::istringstream tokenStream(line);
        std::string token;

        // Read the first column
        if (std::getline(tokenStream, token, ',')) {
            try {
                double value = std::stod(token);
                column1.push_back(value);
            } catch (const std::exception& e) {
                // If it's the first or second line, it might be a header, so just skip it.
                if (lineNumber <= 2) {
                    ++lineNumber;
                    continue;
                } else {
                    throw std::runtime_error("Invalid value in column 1 at line " + std::to_string(lineNumber));
                }
            }
        } else {
            throw std::runtime_error("Missing value in column 1 at line " + std::to_string(lineNumber));
        }

        // Read the second column
        if (std::getline(tokenStream, token, ',')) {
            try {
                double value = std::stod(token);
                column2.push_back(value);
            } catch (const std::exception& e) {
                // If it's the first or second line, it might be a header, so just skip it.
                if (lineNumber <= 2) {
                    ++lineNumber;
                    continue;
                } else {
                    throw std::runtime_error("Invalid value in column 2 at line " + std::to_string(lineNumber));
                }
            }
        } else {
            throw std::runtime_error("Missing value in column 2 at line " + std::to_string(lineNumber));
        }

        ++lineNumber;
    }

    file.close();

    if (column1.size() != column2.size()) {
        throw std::runtime_error("Columns have different lengths");
    }
    if (column1.empty()) {
        throw std::runtime_error("No data entered");
    }

    return { column1, column2 };
}


std::pair<std::vector<double>, std::vector<double>> CSVDataIO::manualInput() {
    std::vector<double> column1;
    std::vector<double> column2;

    std::cout << "Enter the data (input(x) target(y)) separated by a space. Enter 'Q' to stop:" << std::endl;

    std::string input;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input == "Q") {
            break;
        }

        // Split the input into input and target
        std::size_t pos = input.find(' ');
        if (pos == std::string::npos) {
            std::cout << "Invalid input. Enter both input and target separated by a space." << std::endl;
            continue;
        }

        std::string inputStr = input.substr(0, pos);
        std::string targetStr = input.substr(pos + 1);

        // Validate and convert input
        try {
            size_t conversionIdx;
            double inputValue = std::stod(inputStr, &conversionIdx);
            if (conversionIdx != inputStr.size() || !std::isfinite(inputValue)) {
                throw std::invalid_argument("");
            }
            // Validate the input value
            column1.push_back(inputValue);
        } catch (const std::exception& e) {
            std::cout << "Invalid input value. Please enter a valid real number." << std::endl;
            continue;
        }

        // Validate and convert target
        try {
            size_t conversionIdx;
            double targetValue = std::stod(targetStr, &conversionIdx);
            if (conversionIdx != targetStr.size() || !std::isfinite(targetValue)) {
                throw std::invalid_argument("");
            }
            // Validate the target value
            column2.push_back(targetValue);
        } catch (const std::exception& e) {
            std::cout << "Invalid target value. Please enter a valid real number." << std::endl;
            column1.pop_back();  // Remove the corresponding input value
            continue;
        }
    }

    if (column1.size() != column2.size()) {
        throw std::runtime_error("Columns have different lengths");
    }

    if (column1.empty()) {
        throw std::runtime_error("No data entered");
    }

    return { column1, column2 };
}


double CSVDataIO::calculateRSquared(const std::vector<double>& observed, const std::vector<double>& predicted)
{
    size_t size = observed.size();
    
    // Calculate means of the vectors
    double mean1 = 0.0;
    double mean2 = 0.0;
    for (size_t i = 0; i < size; ++i) {
        mean1 += observed[i];
        mean2 += predicted[i];
    }
    mean1 /= static_cast<double>(size);
    mean2 /= static_cast<double>(size);
    
    // Calculate sum of squared differences
    double ssTot = 0.0;
    double ssRes = 0.0;
    for (size_t i = 0; i < size; ++i) {
        double diff1 = observed[i] - mean1;
        double diff2 = predicted[i] - mean2;
        ssTot += diff1 * diff1;
        ssRes += std::pow((diff2 - diff1),2.0);
    }
    
    // Calculate R-squared value
    double rSquared = 1.0 - (ssRes / ssTot);
    
    return rSquared;
}
void CSVDataIO::outputData(const std::vector<double>& observed, const std::vector<double>& predicted) {
    // Calculate R-squared
    double rSquared = calculateRSquared(observed, predicted);

    size_t size = observed.size();
    
    // Set precision for floating-point output
    std::cout << std::fixed << std::setprecision(5);
    
    // Print header
    std::cout << "Data Comparison:\n";
    std::cout << "R-Squared Value :"<<rSquared<<"\n";
    std::cout << "------------------------------\n";
    std::cout << "Observed      |    Target    \n";
    std::cout << "------------------------------\n";
    
    // Print values side by side
    for (size_t i = 0; i < size; ++i) {
        std::cout << std::setw(10) << observed[i] << "    |    " << std::setw(10) << predicted[i] << '\n';
    }
    
    std::cout << "------------------------------\n\n";
   
}

void CSVDataIO::save_file(const std::vector<double>& input, const std::vector<double>& targets, const std::vector<double>& predicted, const std::string& representation)
{
    std::string filename;

    if (inputFile.empty()) {
        std::cout << "Enter the filename to save the data: ";
        std::getline(std::cin, filename);
    } else {
        filename = inputFile;  // Use the input filename
    }
    std::ofstream file;
    file.open(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file: " + filename);
    }
    // Print the representation string
    // Print the header for the three columns
    file << "Input,Targets,Predicted Fit: ,"<< representation <<std::endl;

    // Determine the number of rows based on the input size
    size_t numRows = input.size();
    for (size_t i = 0; i < numRows; ++i) {
        // Print the values in each row
        file << input[i] << "," << targets[i] << "," << predicted[i] << std::endl;
    }
    file.close();

    std::cout << "Data saved to " << filename << std::endl;
}