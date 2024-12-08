// JSON/XML Simple Library (JXSL). Header file for class that contains main functions to operate with JSON/XML files.

#ifndef JXSL_LIB_CPP_H
#define JXSL_LIB_CPP_H

#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>

class JXSL {
public:
    explicit JXSL(const std::string& filename);

    // file operations
    bool createFile(const std::string& format);
    void flushToFile(); // rewrite file with all changes

    // core functionalities
    bool findKeys(std::vector<std::string>& keys) const;
    bool iterateKeys() const;
    bool readData(const std::string& key, std::string& value) const;
    bool addData(const std::string& key, const std::string& value);
    bool editData(const std::string& key, const std::string& newValue);
    bool deleteData(const std::string& key);
    void displayData() const;

private:
    std::string filename;
    bool isJson; // determining the file type
    std::unordered_map<std::string, std::string> data; // saving a key-value
    int pendingChanges; // change counter for deferred data recording
    // internal file utilities
    std::string readFile() const;
    void writeFile(const std::string& content) const;

    // parsing and serialization
    void parseJson(const std::string& content);
    void parseXml(const std::string& content);
    std::string toJson() const; // convert data to JSON
    std::string toXml() const; // convert data to XML

    // helper functions
    static void trimQuotes(std::string& str); // trim redundant quotes
    static std::string stripXmlTags(const std::string& line, const std::string& key); // extract data between tags
};

#endif // JXSL_LIB_CPP_H
