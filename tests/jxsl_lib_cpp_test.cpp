#include "D:/Programming 2 course/Project/JXSL_CPP/jxsl_lib_cpp.h"
#include <iostream>
#include <fstream>
#include <string>

void run_tests_console();
void run_tests_file(const std::string& filename);
void log_to_file(const std::string& log_file, const std::string& message);

int run() {
    int mode;
    std::cout << "Select test mode:\n";
    std::cout << "1 - Input from console\n";
    std::cout << "2 - Input from test file\n";
    std::cout << "Enter choice: ";
    std::cin >> mode;

    if (mode == 1) {
        run_tests_console();
    } else if (mode == 2) {
        std::string filename;
        std::cout << "Enter the test file name: ";
        std::cin >> filename;
        run_tests_file(filename);
    } else {
        std::cout << "Invalid choice. Exiting.\n";
        return 1;
    }

    return 0;
}

// Test functionality using console input
void run_tests_console() {
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
            case 1:  // Add data
                std::cout << "Enter key: ";
                std::cin >> key;
                std::cout << "Enter value: ";
                std::cin >> value;
                if (handler.add_data(key, value)) {
                    std::cout << "Data added successfully.\n";
                } else {
                    std::cout << "Failed to add data. Key may already exist.\n";
                }
                break;

            case 2:  // Edit data
                std::cout << "Enter key to edit: ";
                std::cin >> key;
                std::cout << "Enter new value: ";
                std::cin >> value;
                if (handler.edit_data(key, value)) {
                    std::cout << "Data edited successfully.\n";
                } else {
                    std::cout << "Failed to edit data. Key not found.\n";
                }
                break;

            case 3:  // Delete data
                std::cout << "Enter key to delete: ";
                std::cin >> key;
                if (handler.delete_data(key)) {
                    std::cout << "Data deleted successfully.\n";
                } else {
                    std::cout << "Failed to delete data. Key not found.\n";
                }
                break;

            case 4:  // Read data
                std::cout << "Enter key to read: ";
                std::cin >> key;
                if (handler.read_data(key, value)) {
                    std::cout << "Value: " << value << "\n";
                } else {
                    std::cout << "Key not found.\n";
                }
                break;

            case 5:  // Display all data
                handler.display_data();
                break;

            case 6:  // Exit
                std::cout << "Exiting console tests.\n";
                return;

            default:
                std::cout << "Invalid choice.\n";
        }
    }
}

// Test functionality using a test file
void run_tests_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open test file " << filename << ".\n";
        return;
    }

    const std::string log_file = "test_results.log";
    std::ofstream log(log_file, std::ios::trunc);
    if (!log.is_open()) {
        std::cerr << "Error: Could not open log file " << log_file << ".\n";
        return;
    }

    std::string operation, filetype, key, value;
    while (file >> operation >> filetype >> key >> value) {
        std::string handler_file = (filetype == "json") ? "test.json" : "test.xml";
        JXSL handler(handler_file);

        if (operation == "add") {
            if (handler.add_data(key, value)) {
                log_to_file(log_file, "Added key '" + key + "' with value '" + value + "' to " + filetype + ".");
            } else {
                log_to_file(log_file, "Failed to add key '" + key + "' to " + filetype + ".");
            }
        } else if (operation == "edit") {
            if (handler.edit_data(key, value)) {
                log_to_file(log_file, "Edited key '" + key + "' to value '" + value + "' in " + filetype + ".");
            } else {
                log_to_file(log_file, "Failed to edit key '" + key + "' in " + filetype + ".");
            }
        } else if (operation == "delete") {
            if (handler.delete_data(key)) {
                log_to_file(log_file, "Deleted key '" + key + "' from " + filetype + ".");
            } else {
                log_to_file(log_file, "Failed to delete key '" + key + "' in " + filetype + ".");
            }
        } else if (operation == "read") {
            if (handler.read_data(key, value)) {
                log_to_file(log_file, "Read key '" + key + "' in " + filetype + ": Value = '" + value + "'.");
            } else {
                log_to_file(log_file, "Failed to read key '" + key + "' in " + filetype + ".");
            }
        } else {
            log_to_file(log_file, "Unsupported operation '" + operation + "'.");
        }
    }

    file.close();
    log.close();
    std::cout << "Test file processed. Results logged to " << log_file << ".\n";
}

// Log messages to a log file
void log_to_file(const std::string& log_file, const std::string& message) {
    std::ofstream log(log_file, std::ios::app);
    if (!log.is_open()) {
        std::cerr << "Error: Could not write to log file " << log_file << ".\n";
        return;
    }
    log << message << "\n";
    log.close();
}
