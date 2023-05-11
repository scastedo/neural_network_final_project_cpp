#include "io.h"

std::pair<std::vector<double>, std::vector<double>> CSVDataIO::importDataFromSource() 
{
    std::string filename;
    std::cout << "Enter the filename (leave blank for manual input): ";
    std::getline(std::cin, filename);

    if (filename.empty()) {
        return manualInput();
    } else {
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
                throw std::runtime_error("Invalid value in column 1 at line " + std::to_string(lineNumber));
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
                throw std::runtime_error("Invalid value in column 2 at line " + std::to_string(lineNumber));
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

    return { column1, column2 };
}


std::pair<std::vector<double>, std::vector<double>> CSVDataIO::manualInput() {
    std::vector<double> column1;
    std::vector<double> column2;

    std::cout << "Enter the data (input target). Enter 'Q' to stop:" << std::endl;

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

void CSVDataIO::outputData(const std::vector<std::vector<std::string>>& data)
{
    for (const auto& row : data) {
        for (const auto& cell : row) {
            std::cout << cell << "\t";
        }
        std::cout << std::endl;
    }
}