// JSON/XML Simple Library (JXSL). Class that contains main functions to operate with JSON/XML files with deffered recording optimization.
#include "jxsl_lib_cpp.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

constexpr int FLUSH_THRESHOLD = 10;

JXSL::JXSL(const std::string& filename)
    : filename(filename), isJson(filename.find(".json") != std::string::npos), pendingChanges(0) {
    const std::string content = readFile();
    if (isJson) {
        parseJson(content);
    } else {
        parseXml(content);
    }
}

// deferred data recording
void JXSL::flushToFile() {
    if (pendingChanges == 0) return; // if there is no changes - do nothing
    std::cout << "Flushing changes to file...\n";

    const std::string content = isJson ? toJson() : toXml();
    writeFile(content);
    pendingChanges = 0; // restore change counter
}

// file operations
bool JXSL::createFile(const std::string& format) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to create file: " << filename << "\n";
        return false;
    }

    if (format == "JSON") {
        file << "{}";
    } else if (format == "XML") {
        file << "<root></root>";
    } else {
        std::cerr << "Error: Unsupported file format: " << format << "\n";
        return false;
    }
    return true;
}

std::string JXSL::readFile() const {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file: " << filename << "\n";
        return {};
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void JXSL::writeFile(const std::string& content) const {
    std::ofstream file(filename, std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to write to file: " << filename << "\n";
        return;
    }
    file << content;
}

// Core functionalities
bool JXSL::findKeys(std::vector<std::string>& keys) const {
    keys.reserve(data.size());
    for (const auto& [key, _] : data) {
        keys.push_back(key);
    }
    return !keys.empty();
}

bool JXSL::iterateKeys() const {
    if (data.empty()) {
        std::cout << "No keys available.\n";
        return false;
    }
    for (const auto& [key, _] : data) {
        std::cout << "Key: " << key << "\n";
    }
    return true;
}

bool JXSL::readData(const std::string& key, std::string& value) const {
    const auto it = data.find(key);
    if (it != data.end()) {
        value = it->second;
        return true;
    }
    return false;
}

bool JXSL::addData(const std::string& key, const std::string& value) {
    if (data.find(key) != data.end()) {
        std::cerr << "Error: Key already exists: " << key << "\n";
        return false;
    }

    data[key] = value;
    pendingChanges++;

    if (pendingChanges >= FLUSH_THRESHOLD) {
        flushToFile();
    }
    return true;
}

bool JXSL::editData(const std::string& key, const std::string& newValue) {
    auto it = data.find(key);
    if (it == data.end()) {
        std::cerr << "Error: Key not found: " << key << "\n";
        return false;
    }

    it->second = newValue;
    pendingChanges++;

    if (pendingChanges >= FLUSH_THRESHOLD) {
        flushToFile();
    }
    return true;
}

bool JXSL::deleteData(const std::string& key) {
    if (data.erase(key) == 0) {
        std::cerr << "Error: Key not found: " << key << "\n";
        return false;
    }

    pendingChanges++;

    if (pendingChanges >= FLUSH_THRESHOLD) {
        flushToFile();
    }
    return true;
}

// JSON/XML Parsing and Conversion
void JXSL::parseJson(const std::string& content) {
    data.clear();
    const size_t start = content.find('{');
    const size_t end = content.find('}');
    if (start == std::string::npos || end == std::string::npos) return;

    std::string body = content.substr(start + 1, end - start - 1);
    std::istringstream ss(body);
    std::string line;
    while (std::getline(ss, line, ',')) {
        const size_t colon = line.find(':');
        if (colon != std::string::npos) {
            std::string key = line.substr(0, colon);
            std::string value = line.substr(colon + 1);
            trimQuotes(key);
            trimQuotes(value);
            data[key] = value;
        }
    }
}

void JXSL::parseXml(const std::string& content) {
    data.clear();
    const size_t start = content.find("<root>");
    const size_t end = content.find("</root>");
    if (start == std::string::npos || end == std::string::npos) return;

    std::string body = content.substr(start + 6, end - start - 6);
    std::istringstream ss(body);
    std::string line;
    while (std::getline(ss, line, '>')) {
        const size_t closeTag = line.find('<');
        if (closeTag != std::string::npos) {
            std::string key = line.substr(0, closeTag);
            std::string value = stripXmlTags(line, key);
            data[key] = value;
        }
    }
}

std::string JXSL::toJson() const {
    std::ostringstream ss;
    ss << "{\n";
    for (auto it = data.begin(); it != data.end(); ++it) {
        ss << "    \"" << it->first << "\": \"" << it->second << "\"";
        if (std::next(it) != data.end()) ss << ",";
        ss << "\n";
    }
    ss << "}";
    return ss.str();
}

std::string JXSL::toXml() const {
    std::ostringstream ss;
    ss << "<root>\n";
    for (const auto& [key, value] : data) {
        ss << "    <" << key << ">" << value << "</" << key << ">\n";
    }
    ss << "</root>";
    return ss.str();
}

// Utility
void JXSL::trimQuotes(std::string& str) {
    str.erase(remove(str.begin(), str.end(), '"'), str.end());
    str.erase(0, str.find_first_not_of(" \t\n"));
    str.erase(str.find_last_not_of(" \t\n") + 1);
}

std::string JXSL::stripXmlTags(const std::string& line, const std::string& key) {
    return line.substr(key.length() + 1);
}

void JXSL::displayData() const {
    std::cout << (isJson ? toJson() : toXml()) << "\n";
}

