#ifndef JSON_READING_H
#define JSON_READING_H

#include <cjson/cJSON.h>

cJSON *get_JSON_data_in(char* file);
char** get_expected_boards(cJSON* json, const char* object_name, int* out_count);
char** get_castling_rights(cJSON* json, const char* object_name, int* out_count);

#endif // JSON_READING_H