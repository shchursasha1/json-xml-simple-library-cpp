// Header file for JSON/XML Simple Library (JXSL). Created by Oleksandr Shchur.

#ifndef JXSL_LIB_H
#define JXSL_LIB_H

#include <stdbool.h>

// create file JSON/XML
bool create_file(const char* filename, const char* format);

// find all keys
bool find_keys(const char* filename, char** keys, int* num_keys);

// read data by key
bool read_data(const char* filename, const char* key, char* value);

bool read_data_json(const char* filename, const char* key, char* value);

bool read_data_xml(const char* filename, const char* key, char* value);

// iterate through keys
bool iterate_keys(const char* filename);

bool find_key_chain(const char* filename, const char* data, char* key_chain);

bool add_data_json(const char* filename, const char* key_chain, const char* value);

bool add_data_xml(const char* filename, const char* key, const char* value);

bool edit_data(const char* filename, const char* key, const char* new_value);

bool edit_data_json(const char* filename, const char* key, const char* new_value);

bool edit_data_xml(const char* filename, const char* key, const char* new_value);

bool delete_data(const char* filename, const char* key);

bool delete_data_json(const char* filename, const char* key);

bool delete_data_xml(const char* filename, const char* key);

#endif
