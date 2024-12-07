// JSON/XML Simple Library (JXSL). Header file for class that contains main functions to operate with JSON/XML files. Created by Oleksandr Shchur.

#ifndef JXSL_H
#define JXSL_H

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

class JXSL {
private:
    std::string filename;
    bool is_json;

    // Helper functions
    bool open_file(const std::string& mode, std::fstream& file) const;
    std::string read_file() const;
    void write_file(const std::string& content) const;

    std::string to_json() const;
    std::string to_xml() const;
    void parse_json(const std::string& content);
    void parse_xml(const std::string& content);

    std::unordered_map<std::string, std::string> data;

public:
    JXSL(const std::string& filename);

    // Core functionalities
    bool create_file(const std::string& format);
    bool find_keys(std::vector<std::string>& keys) const;
    bool iterate_keys() const;
    bool read_data(const std::string& key, std::string& value) const;
    bool add_data(const std::string& key, const std::string& value);
    bool edit_data(const std::string& key, const std::string& new_value);
    bool delete_data(const std::string& key);

    // Utility
    void display_data() const;
};

#endif
