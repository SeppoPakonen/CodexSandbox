#ifndef RENDERER_H
#define RENDERER_H

#include "types.h"

// Renderer modes
#define RENDERER_OLD_SCHOOL 0
#define RENDERER_MODERN 1

// Renderer functions
int init_renderer(int width, int height, int mode);
void render_frame();
void cleanup_renderer();
void set_viewport(int width, int height);

// Old school renderer functions
void render_city_oldschool();
void render_block_oldschool(Block *block, int x, int y);
void render_road_oldschool(Block *block);
void render_building_oldschool(Block *block);

// Modern renderer functions
void render_city_modern();
void render_block_modern(Block *block, int x, int y);
void render_road_modern(Block *block);
void render_building_modern(Block *block);

// OpenGL shader functions
unsigned int load_shaders();
unsigned int compile_shader(const char* source, int type);
unsigned int create_program(unsigned int vertex_shader, unsigned int fragment_shader);

// Utility functions
void set_pixel(int x, int y, Vec3 color);
Vec3 get_block_color(Block *block);

#endif