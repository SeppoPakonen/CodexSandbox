#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* load_shader_from_file(const char* filepath) {
    FILE* file = fopen(filepath, "r");
    if (!file) {
        printf("Could not open shader file: %s\n", filepath);
        return NULL;
    }
    
    // Get file size
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    // Allocate memory and read file
    char* shader_code = malloc(length + 1);
    if (shader_code) {
        fread(shader_code, 1, length, file);
        shader_code[length] = '\0';
    }
    
    fclose(file);
    return shader_code;
}