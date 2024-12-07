#include "D:/Programming 2 course/Project/JXSL_CPP/jxsl_lib_cpp.h"
// include C++ implementation
extern "C" {
#include "D:/Programming 2 course/Project/JXSL_C/jxsl_lib.h"
}   // include C implementation
#include <iostream>
#include <string>
#include <cstring>

void compare_add(const std::string& filename, const std::string& key, const std::string& value);
void compare_edit(const std::string& filename, const std::string& key, const std::string& new_value);
void compare_delete(const std::string& filename, const std::string& key);
void compare_read(const std::string& filename, const std::string& key);

int main() {
    // Test file names
    const std::string json_file = "test.json";
    const std::string xml_file = "test.xml";

    // JSON Tests
    std::cout << "Testing JSON...\n";
    compare_add(json_file, "key1", "value1");
    compare_edit(json_file, "key1", "new_value1");
    compare_read(json_file, "key1");
    compare_delete(json_file, "key1");

    // XML Tests
    std::cout << "\nTesting XML...\n";
    compare_add(xml_file, "key2", "value2");
    compare_edit(xml_file, "key2", "new_value2");
    compare_read(xml_file, "key2");
    compare_delete(xml_file, "key2");

    return 0;
}

// Compare add functionality
void compare_add(const std::string& filename, const std::string& key, const std::string& value) {
    JXSL cpp_handler(filename);
    bool cpp_result = cpp_handler.add_data(key, value);

    bool c_result;
    if (filename.find(".json") != std::string::npos) {
        c_result = add_data_json(filename.c_str(), key.c_str(), value.c_str());
    } else {
        c_result = add_data_xml(filename.c_str(), key.c_str(), value.c_str());
    }

    std::cout << "[Add] Key: " << key << ", Value: " << value
              << "\nC++ Result: " << cpp_result
              << ", C Result: " << c_result
              << (cpp_result == c_result ? " [PASS]" : " [FAIL]") << "\n";
}

// Compare edit functionality
void compare_edit(const std::string& filename, const std::string& key, const std::string& new_value) {
    JXSL cpp_handler(filename);
    bool cpp_result = cpp_handler.edit_data(key, new_value);

    bool c_result;
    if (filename.find(".json") != std::string::npos) {
        c_result = edit_data_json(filename.c_str(), key.c_str(), new_value.c_str());
    } else {
        c_result = edit_data_xml(filename.c_str(), key.c_str(), new_value.c_str());
    }

    std::cout << "[Edit] Key: " << key << ", New Value: " << new_value
              << "\nC++ Result: " << cpp_result
              << ", C Result: " << c_result
              << (cpp_result == c_result ? " [PASS]" : " [FAIL]") << "\n";
}

// Compare delete functionality
void compare_delete(const std::string& filename, const std::string& key) {
    JXSL cpp_handler(filename);
    bool cpp_result = cpp_handler.delete_data(key);

    bool c_result;
    if (filename.find(".json") != std::string::npos) {
        c_result = delete_data_json(filename.c_str(), key.c_str());
    } else {
        c_result = delete_data_xml(filename.c_str(), key.c_str());
    }

    std::cout << "[Delete] Key: " << key
              << "\nC++ Result: " << cpp_result
              << ", C Result: " << c_result
              << (cpp_result == c_result ? " [PASS]" : " [FAIL]") << "\n";
}

// Compare read functionality
void compare_read(const std::string& filename, const std::string& key) {
    JXSL cpp_handler(filename);
    std::string cpp_value;
    bool cpp_result = cpp_handler.read_data(key, cpp_value);

    char c_value[256] = {0};
    bool c_result = read_data(filename.c_str(), key.c_str(), c_value);

    std::cout << "[Read] Key: " << key
              << "\nC++ Result: " << cpp_result << ", Value: " << cpp_value
              << "\nC Result: " << c_result << ", Value: " << c_value
              << (cpp_result == c_result && cpp_value == c_value ? " [PASS]" : " [FAIL]") << "\n";
}
