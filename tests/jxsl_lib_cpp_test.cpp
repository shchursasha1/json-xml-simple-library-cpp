#include "jxsl_lib_cpp.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

// Function declarations
void runTestsConsole();
void runTestsFile(const std::string& testFilename);
void logMessage(const std::string& logFilename, const std::string& message);

int main() {
    int mode;

    // Display test mode options
    std::cout << "Select test mode:\n";
    std::cout << "1 - Input from console\n";
    std::cout << "2 - Input from test file\n";
    std::cout << "Enter choice: ";
    std::cin >> mode;

    // Process user choice
    if (mode == 1) {
        runTestsConsole();
    } else if (mode == 2) {
        std::string testFilename;
        std::cout << "Enter the test file name: ";
        std::cin >> testFilename;
        runTestsFile(testFilename);
    } else {
        std::cerr << "Invalid choice. Exiting.\n";
        return 1;
    }

    return 0;
}

// Console-based testing
void runTestsConsole() {
    std::string filename;
    std::cout << "Enter the filename to test (JSON or XML): ";
    std::cin >> filename;

    JXSL handler(filename);

    while (true) {
        std::cout << "\nSelect operation:\n";
        std::cout << "1 - Add data\n";
        std::cout << "2 - Edit data\n";
        std::cout << "3 - Delete data\n";
        std::cout << "4 - Read data\n";
        std::cout << "5 - Display all data\n";
        std::cout << "6 - Exit\n";
        std::cout << "Enter choice: ";

        int choice;
        std::cin >> choice;

        std::string key, value;
        switch (choice) {
            case 1: // Add data
                std::cout << "Enter key: ";
                std::cin >> key;
                std::cout << "Enter value: ";
                std::cin >> value;
                if (handler.addData(key, value)) {
                    std::cout << "Data added successfully.\n";
                } else {
                    std::cerr << "Error: Key already exists.\n";
                }
                break;

            case 2: // Edit data
                std::cout << "Enter key to edit: ";
                std::cin >> key;
                std::cout << "Enter new value: ";
                std::cin >> value;
                if (handler.editData(key, value)) {
                    std::cout << "Data edited successfully.\n";
                } else {
                    std::cerr << "Error: Key not found.\n";
                }
                break;

            case 3: // Delete data
                std::cout << "Enter key to delete: ";
                std::cin >> key;
                if (handler.deleteData(key)) {
                    std::cout << "Data deleted successfully.\n";
                } else {
                    std::cerr << "Error: Key not found.\n";
                }
                break;

            case 4: // Read data
                std::cout << "Enter key to read: ";
                std::cin >> key;
                if (handler.readData(key, value)) {
                    std::cout << "Value: " << value << "\n";
                } else {
                    std::cerr << "Error: Key not found.\n";
                }
                break;

            case 5: // Display all data
                handler.displayData();
                break;

            case 6: // Exit
                std::cout << "Exiting console tests.\n";
                return;

            default:
                std::cerr << "Invalid choice. Try again.\n";
        }
    }
}

// File-based testing
void runTestsFile(const std::string& testFilename) {
    std::ifstream testFile(testFilename);
    if (!testFile.is_open()) {
        std::cerr << "Error: Could not open test file: " << testFilename << "\n";
        return;
    }

    const std::string logFilename = "test_results.log";
    std::ofstream logFile(logFilename, std::ios::trunc);
    if (!logFile.is_open()) {
        std::cerr << "Error: Could not create log file: " << logFilename << "\n";
        return;
    }

    std::string operation, fileType, key, value;
    while (testFile >> operation >> fileType >> key >> value) {
        std::string handlerFilename = (fileType == "json") ? "test.json" : "test.xml";
        JXSL handler(handlerFilename);

        std::ostringstream logMessageStream;
        if (operation == "add") {
            if (handler.addData(key, value)) {
                logMessageStream << "Added key '" << key << "' with value '" << value << "' to " << fileType << ".";
            } else {
                logMessageStream << "Failed to add key '" << key << "' to " << fileType << ".";
            }
        } else if (operation == "edit") {
            if (handler.editData(key, value)) {
                logMessageStream << "Edited key '" << key << "' to value '" << value << "' in " << fileType << ".";
            } else {
                logMessageStream << "Failed to edit key '" << key << "' in " << fileType << ".";
            }
        } else if (operation == "delete") {
            if (handler.deleteData(key)) {
                logMessageStream << "Deleted key '" << key << "' from " << fileType << ".";
            } else {
                logMessageStream << "Failed to delete key '" << key << "' in " << fileType << ".";
            }
        } else if (operation == "read") {
            if (handler.readData(key, value)) {
                logMessageStream << "Read key '" << key << "' in " << fileType << ": Value = '" << value << "'.";
            } else {
                logMessageStream << "Failed to read key '" << key << "' in " << fileType << ".";
            }
        } else {
            logMessageStream << "Unsupported operation: '" << operation << "'.";
        }
        logMessage(logFilename, logMessageStream.str());
    }

    testFile.close();
    logFile.close();
    std::cout << "Test file processed. Results logged to " << logFilename << ".\n";
}

// Log messages to a log file
void logMessage(const std::string& logFilename, const std::string& message) {
    std::ofstream logFile(logFilename, std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Error: Could not write to log file: " << logFilename << "\n";
        return;
    }
    logFile << message << "\n";
}
