#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "jsonReading.h"

cJSON *get_JSON_data_in(char* file)
{
    FILE *fp = fopen(file, "rb");
    if (!fp) {
        printf("cJSON Error: Unable to open the file: %s\n", file);
        return NULL;
    }

    // Get file size
    fseek(fp, 0, SEEK_END);
    long filesize = ftell(fp);
    rewind(fp);

    // Allocate buffer +1 for null terminator
    char *buffer = malloc(filesize + 1);
    if (!buffer) {
        printf("cJSON Error: Memory allocation failed\n");
        fclose(fp);
        return NULL;
    }

    // Read file into buffer
    size_t read_size = fread(buffer, 1, filesize, fp);
    fclose(fp);
    buffer[read_size] = '\0';  // Null terminate

    // Parse JSON
    cJSON *json = cJSON_Parse(buffer);
    if (!json) {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr)
            printf("at file: %s, cJSON Error: %s\n", file, error_ptr);
    }

    free(buffer);  // buffer no longer needed after parsing
    return json;
}

// don't forget to free the returned array and each string after use
char** get_expected_boards(cJSON* json, const char* object_name, int* out_count)
{
    *out_count = 0;

    cJSON *tests = cJSON_GetObjectItemCaseSensitive(json, object_name);
    if (!cJSON_IsArray(tests)) {
        printf("Error: %s is not an array\n", object_name);
        return NULL;
    }

    int count = cJSON_GetArraySize(tests);
    char **boards = malloc(count * sizeof(char*));
    if (!boards) return NULL;

    int i = 0;
    cJSON *test;
    cJSON_ArrayForEach(test, tests) {
        cJSON *expected = cJSON_GetObjectItemCaseSensitive(test, "expected");
        if (!cJSON_IsArray(expected)) {
            boards[i++] = NULL;
            continue;
        }

        // Estimate size (8 rows * ~20 chars per row + newlines)
        size_t buf_size = 1024;
        char *board_str = malloc(buf_size);
        if (!board_str) {
            // clean up allocated boards so far
            for (int j = 0; j < i; j++) free(boards[j]);
            free(boards);
            return NULL;
        }
        board_str[0] = '\0';

        cJSON *row;
        cJSON_ArrayForEach(row, expected) {
            if (cJSON_IsString(row) && row->valuestring) {
                strcat(board_str, row->valuestring);
            }
        }

        boards[i++] = board_str;
    }

    *out_count = count;
    return boards; // caller frees each boards[i] and boards
}

char** get_castling_rights(cJSON* json, const char* object_name, int* out_count)
{
    *out_count = 0;

    cJSON *tests = cJSON_GetObjectItemCaseSensitive(json, object_name);
    if (!cJSON_IsArray(tests)) {
        printf("Error: %s is not an array\n", object_name);
        return NULL;
    }

    int count = cJSON_GetArraySize(tests);
    char **rights = malloc(count * sizeof(char*));
    if (!rights) return NULL;

    int i = 0;
    cJSON *test;
    cJSON_ArrayForEach(test, tests) {
        cJSON *castling = cJSON_GetObjectItemCaseSensitive(test, "castling_rights");
        if (cJSON_IsString(castling) && castling->valuestring) {
            rights[i] = strdup(castling->valuestring);
        } else {
            rights[i] = NULL; // no castling rights for this test
        }
        i++;
    }

    *out_count = count;
    return rights; // caller frees each rights[i] and rights
}
