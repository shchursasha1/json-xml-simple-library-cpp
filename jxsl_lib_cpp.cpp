// JSON/XML Simple Library (JXSL). Class that contains main functions to operate with JSON/XML files. Created by Oleksandr Shchur.

#include "jxsl_lib_cpp.h"
#include <fstream>
#include <sstream>
#include <algorithm>

// Constructor
JXSL::JXSL(const std::string& filename) : filename(filename) {
    is_json = filename.find(".json") != std::string::npos;
    std::string content = read_file();
    if (is_json) {
        parse_json(content);
    } else {
        parse_xml(content);
    }
}

// File operations
bool JXSL::open_file(const std::string& mode, std::fstream& file) const {
    file.open(filename, mode == "read" ? std::ios::in : std::ios::out | std::ios::in);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << "\n";
        return false;
    }
    return true;
}

std::string JXSL::read_file() const {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void JXSL::write_file(const std::string& content) const {
    std::ofstream file(filename, std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "Error writing file: " << filename << "\n";
        return;
    }
    file << content;
}

// Core functionalities
bool JXSL::create_file(const std::string& format) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;

    if (format == "JSON") {
        file << "{}";
    } else if (format == "XML") {
        file << "<root></root>";
    } else {
        std::cerr << "Unsupported format: " << format << "\n";
        return false;
    }
    file.close();
    return true;
}

bool JXSL::find_keys(std::vector<std::string>& keys) const {
    for (const auto& [key, value] : data) {
        keys.push_back(key);
    }
    return !keys.empty();
}

bool JXSL::iterate_keys() const {
    for (const auto& [key, _] : data) {
        std::cout << "Key: " << key << "\n";
    }
    return !data.empty();
}

bool JXSL::read_data(const std::string& key, std::string& value) const {
    auto it = data.find(key);
    if (it != data.end()) {
        value = it->second;
        return true;
    }
    return false;
}

bool JXSL::add_data(const std::string& key, const std::string& value) {
    if (data.find(key) != data.end()) return false;
    data[key] = value;
    write_file(is_json ? to_json() : to_xml());
    return true;
}

bool JXSL::edit_data(const std::string& key, const std::string& new_value) {
    auto it = data.find(key);
    if (it == data.end()) return false;
    data[key] = new_value;
    write_file(is_json ? to_json() : to_xml());
    return true;
}

bool JXSL::delete_data(const std::string& key) {
    if (data.erase(key) == 0) return false;
    write_file(is_json ? to_json() : to_xml());
    return true;
}

// JSON/XML Parsing and Conversion
void JXSL::parse_json(const std::string& content) {
    data.clear();
    size_t start = content.find('{');
    size_t end = content.find('}');
    if (start == std::string::npos || end == std::string::npos) return;

    std::string body = content.substr(start + 1, end - start - 1);
    std::stringstream ss(body);
    std::string line;
    while (std::getline(ss, line, ',')) {
        size_t colon = line.find(':');
        if (colon != std::string::npos) {
            std::string key = line.substr(0, colon);
            std::string value = line.substr(colon + 1);
            key.erase(std::remove(key.begin(), key.end(), '"'), key.end());
            value.erase(std::remove(value.begin(), value.end(), '"'), value.end());
            data[key] = value;
        }
    }
}

void JXSL::parse_xml(const std::string& content) {
    data.clear();
    size_t start = content.find("<root>");
    size_t end = content.find("</root>");
    if (start == std::string::npos || end == std::string::npos) return;

    std::string body = content.substr(start + 6, end - start - 6);
    std::stringstream ss(body);
    std::string line;
    while (std::getline(ss, line, '>')) {
        size_t close_tag = line.find('<');
        if (close_tag != std::string::npos) {
            std::string key = line.substr(0, close_tag);
            std::string value = line.substr(close_tag + 1);
            data[key] = value;
        }
    }
}

std::string JXSL::to_json() const {
    std::stringstream ss;
    ss << "{\n";
    for (auto it = data.begin(); it != data.end(); ++it) {
        ss << "    \"" << it->first << "\": \"" << it->second << "\"";
        if (std::next(it) != data.end()) ss << ",";
        ss << "\n";
    }
    ss << "}";
    return ss.str();
}

std::string JXSL::to_xml() const {
    std::stringstream ss;
    ss << "<root>\n";
    for (const auto& [key, value] : data) {
        ss << "    <" << key << ">" << value << "</" << key << ">\n";
    }
    ss << "</root>";
    return ss.str();
}

// Utility
void JXSL::display_data() const {
    std::cout << (is_json ? to_json() : to_xml()) << "\n";
}
