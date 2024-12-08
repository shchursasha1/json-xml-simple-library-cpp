// File for cross-testing the C and C++ implementations

#include "jxsl_lib_cpp.h"
// Include C implementation for cross testing
extern "C" {
#include "JXSL_C/jxsl_lib.h"
}
#include <iostream>
#include <string>
#include <iomanip>

// Function declarations
void testAdd(const std::string& filename, const std::string& key, const std::string& value);
void testEdit(const std::string& filename, const std::string& key, const std::string& newValue);
void testDelete(const std::string& filename, const std::string& key);
void testRead(const std::string& filename, const std::string& key);

void displayHeader(const std::string& testName);

int main() {
    const std::string jsonFile = "test.json";
    const std::string xmlFile = "test.xml";

    // Run tests for JSON
    displayHeader("Testing JSON");
    testAdd(jsonFile, "key1", "value1");
    testEdit(jsonFile, "key1", "new_value1");
    testRead(jsonFile, "key1");
    testDelete(jsonFile, "key1");

    // Run tests for XML
    displayHeader("Testing XML");
    testAdd(xmlFile, "key2", "value2");
    testEdit(xmlFile, "key2", "new_value2");
    testRead(xmlFile, "key2");
    testDelete(xmlFile, "key2");

    return 0;
}

// Test add functionality
void testAdd(const std::string& filename, const std::string& key, const std::string& value) {
    JXSL cppHandler(filename);
    bool cppResult = cppHandler.addData(key, value);

    bool cResult = (filename.find(".json") != std::string::npos)
                   ? add_data_json(filename.c_str(), key.c_str(), value.c_str())
                   : add_data_xml(filename.c_str(), key.c_str(), value.c_str());

    std::cout << std::left << std::setw(10) << "[Add]"
              << "Key: " << key << ", Value: " << value
              << "\n    C++ Result: " << cppResult
              << ", C Result: " << cResult
              << (cppResult == cResult ? " [PASS]" : " [FAIL]") << "\n";
}

// Test edit functionality
void testEdit(const std::string& filename, const std::string& key, const std::string& newValue) {
    JXSL cppHandler(filename);
    bool cppResult = cppHandler.editData(key, newValue);

    bool cResult = (filename.find(".json") != std::string::npos)
                   ? edit_data_json(filename.c_str(), key.c_str(), newValue.c_str())
                   : edit_data_xml(filename.c_str(), key.c_str(), newValue.c_str());

    std::cout << std::left << std::setw(10) << "[Edit]"
              << "Key: " << key << ", New Value: " << newValue
              << "\n    C++ Result: " << cppResult
              << ", C Result: " << cResult
              << (cppResult == cResult ? " [PASS]" : " [FAIL]") << "\n";
}

// Test delete functionality
void testDelete(const std::string& filename, const std::string& key) {
    JXSL cppHandler(filename);
    bool cppResult = cppHandler.deleteData(key);

    bool cResult = (filename.find(".json") != std::string::npos)
                   ? delete_data_json(filename.c_str(), key.c_str())
                   : delete_data_xml(filename.c_str(), key.c_str());

    std::cout << std::left << std::setw(10) << "[Delete]"
              << "Key: " << key
              << "\n    C++ Result: " << cppResult
              << ", C Result: " << cResult
              << (cppResult == cResult ? " [PASS]" : " [FAIL]") << "\n";
}

// Test read functionality
void testRead(const std::string& filename, const std::string& key) {
    JXSL cppHandler(filename);
    std::string cppValue;
    bool cppResult = cppHandler.readData(key, cppValue);

    char cValue[256] = {};
    bool cResult = read_data(filename.c_str(), key.c_str(), cValue);

    std::cout << std::left << std::setw(10) << "[Read]"
              << "Key: " << key
              << "\n    C++ Result: " << cppResult << ", Value: " << cppValue
              << "\n    C Result: " << cResult << ", Value: " << cValue
              << (cppResult == cResult && cppValue == cValue ? " [PASS]" : " [FAIL]") << "\n";
}

// Display a formatted test section header
void displayHeader(const std::string& testName) {
    std::cout << "\n" << std::string(40, '=') << "\n";
    std::cout << testName << "\n";
    std::cout << std::string(40, '=') << "\n";
}
