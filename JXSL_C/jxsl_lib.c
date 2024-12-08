// JSON/XML Simple Library (JXSL). Contains main functions to operate with JSON/XML files. Created by Oleksandr Shchur.

#include "jxsl_lib.h"
#include <stdio.h>
#include <windows.h>
#include <unistd.h>
#include <string.h>

// a struct to optimize XML file editing with Indexing
typedef struct {
    char key[256];
    long position;
} XmlIndex;

#pragma region file_operations
// Helper function for file operations
static FILE* open_file(const char* filename, const char* mode) {
    FILE* file = fopen(filename, mode);
    if (!file) {
        perror("Error opening file");
    }
    return file;
}


// Create a JSON or XML file
bool create_file(const char* filename, const char* format) {
    FILE* file = open_file(filename, "w");
    if (!file) return false;

    if (strcmp(format, "JSON") == 0) {
        fprintf(file, "{}"); // Empty JSON structure
    } else if (strcmp(format, "XML") == 0) {
        fprintf(file, "<root></root>"); // Empty XML structure
    } else {
        fclose(file);
        fprintf(stderr, "Unsupported format: %s\n", format);
        return false;
    }
    fclose(file);
    return true;
}
#pragma endregion

#pragma region iterators
// Function to find keys in a JSON or XML file
bool find_keys(const char* filename, char** keys, int* num_keys) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return false;
    }

    char buffer[1024];
    int key_count = 0;
    bool is_json = strstr(filename, ".json") != NULL;

    while (fscanf(file, "%s", buffer) != EOF) {
        if (is_json) {
            // For JSON: Look for "key": patterns
            if (strchr(buffer, ':')) {
                char* key = strtok(buffer, ":");
                if (key && key[0] == '\"' && key[strlen(key) - 1] == '\"') {
                    key[strlen(key) - 1] = '\0'; // Remove trailing quote
                    keys[key_count++] = strdup(key + 1); // Skip leading quote
                }
            }
        } else {
            // For XML: Look for <key> patterns
            if (buffer[0] == '<' && buffer[1] != '/') {
                char* key_end = strchr(buffer, '>');
                if (key_end) {
                    *key_end = '\0'; // Null-terminate the key
                    keys[key_count++] = strdup(buffer + 1); // Skip leading '<'
                }
            }
        }

        if (key_count >= *num_keys) break; // Prevent overflow
    }

    *num_keys = key_count;
    fclose(file);
    return true;
}

// Function to iterate through all keys in a JSON or XML file
bool iterate_keys(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return false;
    }

    char buffer[1024];
    bool is_json = strstr(filename, ".json") != NULL;

    printf("Iterating keys:\n");
    while (fscanf(file, "%s", buffer) != EOF) {
        if (is_json) {
            // For JSON: Look for "key": patterns
            if (strchr(buffer, ':')) {
                char* key = strtok(buffer, ":");
                if (key && key[0] == '\"' && key[strlen(key) - 1] == '\"') {
                    key[strlen(key) - 1] = '\0'; // Remove trailing quote
                    printf("Key: %s\n", key + 1); // Skip leading quote
                }
            }
        } else {
            // For XML: Look for <key> patterns
            if (buffer[0] == '<' && buffer[1] != '/') {
                char* key_end = strchr(buffer, '>');
                if (key_end) {
                    *key_end = '\0'; // Null-terminate the key
                    printf("Key: %s\n", buffer + 1); // Skip leading '<'
                }
            }
        }
    }

    fclose(file);
    return true;
}
#pragma endregion

#pragma region read_data
// Read data by a given key
bool read_data(const char* filename, const char* key, char* value) {
    // Determine file type based on extension
    const char* ext = strrchr(filename, '.');
    if (!ext) {
        fprintf(stderr, "Error: File extension not found.\n");
        return false;
    }

    if (strcmp(ext, ".json") == 0) {
        return read_data_json(filename, key, value);
    } else if (strcmp(ext, ".xml") == 0) {
        return read_data_xml(filename, key, value);
    } else {
        fprintf(stderr, "Error: Unsupported file type '%s'.\n", ext);
        return false;
    }
}

// Read data for JSON files
bool read_data_json(const char* filename, const char* key, char* value) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return false;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        // Check if the line contains the exact key
        char* key_pos = strstr(line, key);
        if (key_pos) {
            // Ensure it's the exact key by checking delimiters
            char* colon_pos = strchr(key_pos, ':');
            if (colon_pos) {
                char* value_start = colon_pos + 1;
                while (*value_start == ' ' || *value_start == '"') value_start++; // Skip spaces and quotes

                char* value_end = strchr(value_start, '"');
                if (value_end) {
                    *value_end = '\0';
                    strcpy(value, value_start);
                    fclose(file);
                    return true;
                }
            }
        }
    }

    fclose(file);
    return false; // Key not found
}

// Read data for XML files
bool read_data_xml(const char* filename, const char* key, char* value) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return false;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, key)) {
            char* value_start = strchr(line, '>');
            if (value_start) {
                value_start++;
                char* value_end = strchr(value_start, '<');
                if (value_end) {
                    *value_end = '\0';
                    strcpy(value, value_start);
                    fclose(file);
                    return true;
                }
            }
        }
    }

    fclose(file);
    return false; // Key not found
}

#pragma endregion

#pragma region write_data
// Add data to a JSON file in valid JSON format
bool add_data_json(const char* filename, const char* key, const char* value) {
    FILE* file = fopen(filename, "r");
    if (!file) return false;

    // Get the file size
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    // Allocate memory for the file content
    char* buffer = malloc(file_size + 1);
    if (!buffer) {
        fclose(file);
        perror("Error allocating memory");
        return false;
    }

    // Read the file content
    fread(buffer, sizeof(char), file_size, file);
    buffer[file_size] = '\0'; // Null-terminate the buffer
    fclose(file);

    // Check if the file is empty or contains just '{}'
    bool is_empty_json = (strcmp(buffer, "{}") == 0 || buffer[0] == '\0');

    // Open the file in write mode
    file = fopen(filename, "w");
    if (!file) {
        free(buffer);
        return false;
    }

    if (is_empty_json) {
        // Add the first key-value pair
        fprintf(file, "{\n    \"%s\": \"%s\"\n}", key, value);
    } else {
        // Remove the trailing '}' and append the new key-value pair
        char* closing_brace = strrchr(buffer, '}');
        if (closing_brace) *closing_brace = '\0'; // Truncate the buffer
        fprintf(file, "%s,\n    \"%s\": \"%s\"\n}", buffer, key, value);
    }

    free(buffer);
    fclose(file);
    return true;
}


// Add data to a XML file in valid XML format
bool add_data_xml(const char* filename, const char* key, const char* value) {
    FILE* file = open_file(filename, "r");
    if (!file) return false;

    // Read the entire file content into memory
    char buffer[4096] = {0};
    fread(buffer, sizeof(char), sizeof(buffer) - 1, file);
    fclose(file);

    // Check if the file is empty or contains just "<root></root>"
    bool is_empty_xml = (strcmp(buffer, "<root></root>") == 0 || buffer[0] == '\0');

    // Open the file in write mode
    file = open_file(filename, "w");
    if (!file) return false;

    if (is_empty_xml) {
        // If the file is empty or contains only "<root></root>", add the first key-value pair
        fprintf(file, "<root>\n    <%s>%s</%s>\n</root>", key, value, key);
    } else {
        // Otherwise, remove the closing "</root>" and append the new key-value pair
        char* closing_tag = strstr(buffer, "</root>");
        if (closing_tag) {
            *closing_tag = '\0'; // Truncate the buffer before "</root>"
            fprintf(file, "%s    <%s>%s</%s>\n</root>", buffer, key, value, key);
        }
    }

    fclose(file);
    return true;
}
#pragma endregion

#pragma region edit_data
bool edit_data(const char* filename, const char* key, const char* new_value) {
    // Determine file type based on extension
    const char* ext = strrchr(filename, '.');
    if (!ext) {
        fprintf(stderr, "Error: File extension not found.\n");
        return false;
    }

    if (strcmp(ext, ".json") == 0) {
        return edit_data_json(filename, key, new_value);
    } else if (strcmp(ext, ".xml") == 0) {
        return edit_data_xml(filename, key, new_value);
    } else {
        fprintf(stderr, "Error: Unsupported file type '%s'.\n", ext);
        return false;
    }
}

// Edit data by a given key (optimized with mapping to avoid rewriting the file every time to edit)
bool edit_data_json(const char* filename, const char* key, const char* new_value) {
    HANDLE hFile = CreateFileA(
        filename, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        perror("Error opening file");
        return false;
    }

    // Map the file into memory
    HANDLE hMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
    if (!hMap) {
        CloseHandle(hFile);
        perror("Error creating file mapping");
        return false;
    }

    char* map = (char*)MapViewOfFile(hMap, FILE_MAP_WRITE, 0, 0, 0);
    if (!map) {
        CloseHandle(hMap);
        CloseHandle(hFile);
        perror("Error mapping file");
        return false;
    }

    // Locate the key in the mapped file
    char* pos = strstr(map, key);
    if (!pos) {
        UnmapViewOfFile(map);
        CloseHandle(hMap);
        CloseHandle(hFile);
        return false; // Key not found
    }

    // Locate the value position
    char* value_start = strchr(pos, ':');
    if (!value_start) {
        UnmapViewOfFile(map);
        CloseHandle(hMap);
        CloseHandle(hFile);
        return false;
    }

    // Replace the value (ensure it doesn't exceed allocated space)
    char* value_end = strchr(value_start, ',');
    if (!value_end) {
        value_end = strchr(value_start, '}');
    }
    if (!value_end) {
        UnmapViewOfFile(map);
        CloseHandle(hMap);
        CloseHandle(hFile);
        return false;
    }

    size_t value_len = value_end - value_start - 2; // Exclude ': '
    if (strlen(new_value) > value_len) {
        UnmapViewOfFile(map);
        CloseHandle(hMap);
        CloseHandle(hFile);
        return false; // New value too long to fit
    }

    // Overwrite the value
    memcpy(value_start + 2, new_value, strlen(new_value));
    memset(value_start + 2 + strlen(new_value), ' ', value_len - strlen(new_value));

    // Unmap and close handles
    UnmapViewOfFile(map);
    CloseHandle(hMap);
    CloseHandle(hFile);
    return true;
}

// Edit data by a given key (optimized with Indexing to avoid rewriting the file every time to edit)
bool edit_data_xml(const char* filename, const char* key, const char* new_value) {
    // Open file in read+write mode
    FILE* file = fopen(filename, "r+");
    if (!file) {
        perror("Error opening file");
        return false;
    }

    char line[1024];
    long pos = 0;
    bool updated = false;

    // Scan for the key
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, key)) {
            // Key found; overwrite the value in place
            pos = ftell(file) - strlen(line);
            fseek(file, pos, SEEK_SET);

            char* value_start = strchr(line, '>');
            char* value_end = strchr(line, '<');
            if (value_start && value_end && value_start < value_end) {
                value_start++; // Skip the '>'
                fprintf(file, "%s", new_value);
                updated = true;
                break;
            }
        }
    }

    fclose(file);
    return updated;
}
#pragma endregion

#pragma region delete_data
// Delete data by a given key
bool delete_data(const char* filename, const char* key) {
    // Determine file type based on extension
    const char* ext = strrchr(filename, '.');
    if (!ext) {
        fprintf(stderr, "Error: File extension not found.\n");
        return false;
    }

    if (strcmp(ext, ".json") == 0) {
        return delete_data_json(filename, key);
    } else if (strcmp(ext, ".xml") == 0) {
        return delete_data_xml(filename, key);
    } else {
        fprintf(stderr, "Error: Unsupported file type '%s'.\n", ext);
        return false;
    }
}

// Delete data for JSON files
bool delete_data_json(const char* filename, const char* key) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return false;
    }

    FILE* temp_file = fopen("temp.json", "w");
    if (!temp_file) {
        fclose(file);
        perror("Error opening temporary file");
        return false;
    }

    char line[1024];
    bool deleted = false;

    while (fgets(line, sizeof(line), file)) {
        if (!strstr(line, key)) {
            fputs(line, temp_file); // Copy lines not containing the key
        } else {
            deleted = true; // Skip the line containing the key
        }
    }

    fclose(file);
    fclose(temp_file);

    // Replace original file with the temporary file
    if (deleted) {
        remove(filename);
        rename("temp.json", filename);
    } else {
        remove("temp.json");
    }

    return deleted;
}

// Delete data for XML files
bool delete_data_xml(const char* filename, const char* key) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return false;
    }

    FILE* temp_file = fopen("temp.xml", "w");
    if (!temp_file) {
        fclose(file);
        perror("Error opening temporary file");
        return false;
    }

    char line[1024];
    bool deleted = false;

    while (fgets(line, sizeof(line), file)) {
        if (!strstr(line, key)) {
            fputs(line, temp_file); // Copy lines not containing the key
        } else {
            deleted = true; // Skip the line containing the key
        }
    }

    fclose(file);
    fclose(temp_file);

    // Replace original file with the temporary file
    if (deleted) {
        remove(filename);
        rename("temp.xml", filename);
    } else {
        remove("temp.xml");
    }

    return deleted;
}
#pragma endregion

