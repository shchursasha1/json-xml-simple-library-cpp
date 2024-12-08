// Test file for testing common functions of JSON/XML Simple Library with logging. Created by Oleksandr Shchur.

#include "jxsl_lib.h"
#include <stdio.h>
#include <string.h>

void run_tests_console();
void run_tests_file(const char* filename);

void log_to_file(const char* log_file, const char* message);

int main() {
    int mode;
    printf("Select test mode:\n");
    printf("1 - Input from console\n");
    printf("2 - Input from test file\n");
    printf("Enter choice: ");
    scanf("%d", &mode);

    if (mode == 1) {
        run_tests_console();
    } else if (mode == 2) {
        char filename[256];
        printf("Enter the test file name: ");
        scanf("%s", filename);
        run_tests_file(filename);
    } else {
        printf("Invalid choice. Exiting.\n");
        return 1;
    }

    return 0;
}

// Test functionality using console input
void run_tests_console() {
    char filename[256];
    printf("Enter the filename to test (JSON or XML): ");
    scanf("%s", filename);

    while (1) {
        printf("\nSelect operation:\n");
        printf("1 - Add data\n");
        printf("2 - Edit data\n");
        printf("3 - Delete data\n");
        printf("4 - Read data\n");
        printf("5 - Exit\n");
        printf("Enter choice: ");
        int choice;
        scanf("%d", &choice);

        char key[256], value[256];
        switch (choice) {
            case 1:  // Add data
                printf("Enter key: ");
                scanf("%s", key);
                printf("Enter value: ");
                scanf("%s", value);
                if (strstr(filename, ".json")) {
                    add_data_json(filename, key, value);
                } else if (strstr(filename, ".xml")) {
                    add_data_xml(filename, key, value);
                } else {
                    printf("Unsupported file type.\n");
                }
                printf("Data added successfully.\n");
                break;

            case 2:  // Edit data
                printf("Enter key to edit: ");
                scanf("%s", key);
                printf("Enter new value: ");
                scanf("%s", value);
                if (strstr(filename, ".json")) {
                    if (edit_data_json(filename, key, value)) {
                        printf("Data edited successfully.\n");
                    } else {
                        printf("Failed to edit data. Key not found.\n");
                    }
                } else if (strstr(filename, ".xml")) {
                    if (edit_data_xml(filename, key, value)) {
                        printf("Data edited successfully.\n");
                    } else {
                        printf("Failed to edit data. Key not found.\n");
                    }
                } else {
                    printf("Unsupported file type.\n");
                }
                break;

            case 3:  // Delete data
                printf("Enter key to delete: ");
                scanf("%s", key);
                if (strstr(filename, ".json")) {
                    if (delete_data_json(filename, key)) {
                        printf("Data deleted successfully.\n");
                    } else {
                        printf("Failed to delete data. Key not found.\n");
                    }
                } else if (strstr(filename, ".xml")) {
                    if (delete_data_xml(filename, key)) {
                        printf("Data deleted successfully.\n");
                    } else {
                        printf("Failed to delete data. Key not found.\n");
                    }
                } else {
                    printf("Unsupported file type.\n");
                }
                break;

            case 4:  // Read data
                printf("Enter key to read: ");
                scanf("%s", key);
                if (strstr(filename, ".json") || strstr(filename, ".xml")) {
                    char result[256];
                    if (read_data(filename, key, result)) {
                        printf("Value: %s\n", result);
                    } else {
                        printf("Key not found.\n");
                    }
                } else {
                    printf("Unsupported file type.\n");
                }
                break;

            case 5:  // Exit
                printf("Exiting console tests.\n");
                return;

            default:
                printf("Invalid choice.\n");
        }
    }
}

// Test functionality using a test file
void run_tests_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open test file %s.\n", filename);
        return;
    }

    const char* log_file = "test_results.log";
    FILE* log = fopen(log_file, "w");
    if (!log) {
        printf("Error: Could not open log file %s.\n", log_file);
        fclose(file);
        return;
    }

    char operation[256], filetype[256], key[256], value[256];
    while (fscanf(file, "%s %s %s %s", operation, filetype, key, value) != EOF) {
        if (strcmp(operation, "add") == 0) {
            if (strcmp(filetype, "json") == 0) {
                add_data_json("test.json", key, value);
                fprintf(log, "Added key '%s' with value '%s' to JSON.\n", key, value);
            } else if (strcmp(filetype, "xml") == 0) {
                add_data_xml("test.xml", key, value);
                fprintf(log, "Added key '%s' with value '%s' to XML.\n", key, value);
            }
        } else if (strcmp(operation, "edit") == 0) {
            if (strcmp(filetype, "json") == 0) {
                if (edit_data_json("test.json", key, value)) {
                    fprintf(log, "Edited key '%s' to value '%s' in JSON.\n", key, value);
                } else {
                    fprintf(log, "Failed to edit key '%s' in JSON.\n", key);
                }
            } else if (strcmp(filetype, "xml") == 0) {
                if (edit_data_xml("test.xml", key, value)) {
                    fprintf(log, "Edited key '%s' to value '%s' in XML.\n", key, value);
                } else {
                    fprintf(log, "Failed to edit key '%s' in XML.\n", key);
                }
            }
        } else if (strcmp(operation, "delete") == 0) {
            if (strcmp(filetype, "json") == 0) {
                if (delete_data_json("test.json", key)) {
                    fprintf(log, "Deleted key '%s' from JSON.\n", key);
                } else {
                    fprintf(log, "Failed to delete key '%s' in JSON.\n", key);
                }
            } else if (strcmp(filetype, "xml") == 0) {
                if (delete_data_xml("test.xml", key)) {
                    fprintf(log, "Deleted key '%s' from XML.\n", key);
                } else {
                    fprintf(log, "Failed to delete key '%s' in XML.\n", key);
                }
            }
        } else if (strcmp(operation, "read") == 0) {
            char result[256];
            if (strcmp(filetype, "json") == 0) {
                if (read_data("test.json", key, result)) {
                    fprintf(log, "Read key '%s' in JSON: Value = '%s'.\n", key, result);
                } else {
                    fprintf(log, "Failed to read key '%s' in JSON.\n", key);
                }
            } else if (strcmp(filetype, "xml") == 0) {
                if (read_data("test.xml", key, result)) {
                    fprintf(log, "Read key '%s' in XML: Value = '%s'.\n", key, result);
                } else {
                    fprintf(log, "Failed to read key '%s' in XML.\n", key);
                }
            }
        }
    }

    fclose(file);
    fclose(log);
    printf("Test file processed. Results logged to %s.\n", log_file);
}
