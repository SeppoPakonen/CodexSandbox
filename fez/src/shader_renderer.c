#include "../include/game.h"
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern SDL_Window* g_window;

// Vertex shader source
static const char* vertex_shader_source = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

// Fragment shader based on the image.glsl
static const char* fragment_shader_source = 
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec2 iResolution;\n"
    "uniform float iTime;\n"
    "uniform int iFrame;\n"
    "uniform sampler2D iChannel0; // Buffer A\n"
    "\n"
    "const float PI = acos(0.0) * 2.0;\n"
    "const int MAX_STEPS = 48;\n"
    "const float EPSILON = 0.001;\n"
    "\n"
    "struct Tile {\n"
    "    int ID;\n"
    "};\n"
    "\n"
    "struct HitInfo {\n"
    "    Tile tile;\n"
    "    vec3 pos;\n"
    "    vec3 normal;\n"
    "};\n"
    "\n"
    "// Rotation function\n"
    "void rotate(inout vec2 p, float a) {\n"
    "    p = cos(a)*p + sin(a)*vec2(p.y, -p.x);\n"
    "}\n"
    "\n"
    "// World function - simplified\n"
    "Tile world(vec3 p) {\n"
    "    p = floor(p) + 0.5;\n"
    "\n"
    "    if (abs(p.y + 6.0) < 1.0 && abs(p.x - 1.0) < 2.0 && abs(p.z - 4.0) < 5.0) return Tile(1);\n"
    "    if (abs(p.y + 4.0) < 2.0 && abs(p.x - 3.0) < 2.0 && abs(p.z - 6.0) < 2.0) return Tile(1);\n"
    "    if (abs(p.y - 4.0) < 1.0 && abs(p.x + 4.5) < 1.5 && abs(p.z - 5.0) < 1.0) return Tile(1);\n"
    "    if (abs(p.y - 0.5) < 6.5 && abs(p.x - 5.0) < 2.0 && abs(p.z + 1.5) < 4.0) return Tile(1);\n"
    "    if (abs(p.y - 4.0) < 1.0 && abs(p.x - 4.0) < 1.0 && abs(p.z + 3.0) < 2.0) return Tile(1);\n"
    "    if (abs(p.y + 1.0) < 1.0 && abs(p.x - 7.0) < 2.0 && abs(p.z + 4.0) < 1.0) return Tile(1);\n"
    "    if (length(p + vec3(8,0,9)) < 2.5) return Tile(1);\n"
    "\n"
    "    return Tile(0);\n"
    "}\n"
    "\n"
    "HitInfo castRay(vec3 rayPos, vec3 rayDir) {\n"
    "    Tile tile;\n"
    "    rayPos *= 0.0625;\n"
    "\n"
    "    vec3 mapPos = floor(rayPos);\n"
    "    vec3 deltaDist = abs(vec3(length(rayDir)) / rayDir);\n"
    "    vec3 rayStep = sign(rayDir);\n"
    "    vec3 sideDist = (sign(rayDir) * (mapPos - rayPos) + (sign(rayDir) * 0.5) + 0.5) * deltaDist;\n"
    "    vec3 mask;\n"
    "\n"
    "    for (int i = 0; i < MAX_STEPS; i++) {\n"
    "        tile = world(mapPos);\n"
    "        if (tile.ID != 0) break;\n"
    "        mask = step(sideDist.xyz, sideDist.yzx) * step(sideDist.xyz, sideDist.zxy) + 0.000000001;\n"
    "        sideDist += mask * deltaDist;\n"
    "        mapPos += mask * rayStep;\n"
    "    }\n"
    "\n"
    "    rayPos = rayDir / dot(mask * rayDir, vec3(1.0)) * dot(mask * (mapPos + vec3(lessThan(rayDir, vec3(0.0))) - rayPos), vec3(1.0)) + rayPos;\n"
    "    rayPos = rayDir / dot(mask * rayDir, vec3(1.0)) * dot(mask * (mapPos + vec3(lessThan(rayDir, vec3(0.0))) - rayPos), vec3(1.0)) + rayPos;\n"
    "\n"
    "    return HitInfo(tile, rayPos, mask);\n"
    "}\n"
    "\n"
    "vec3 blockTexture(vec2 p) {\n"
    "    vec3[] palette = vec3[](vec3(0.5, 0.25, 0.25), vec3(0.8, 0.5, 0.25), vec3(1,0.75,0.15));\n"
    "    int[] pixels = int[](\n"
    "        1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,\n"
    "        0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,2,\n"
    "        0,2,1,1,1,1,1,1,1,1,1,1,1,1,0,2,\n"
    "        0,2,1,1,2,2,2,2,2,2,2,2,2,1,0,2,\n"
    "        0,2,1,0,1,1,1,1,1,1,1,1,2,1,0,2,\n"
    "        0,2,1,0,1,1,1,1,1,1,1,1,2,1,0,2,\n"
    "        0,2,1,0,1,1,1,1,1,1,1,1,2,1,0,2,\n"
    "        0,2,1,0,1,1,1,1,1,1,1,1,2,1,0,2,\n"
    "        0,2,1,0,1,1,1,1,1,1,1,1,2,1,0,2,\n"
    "        0,2,1,0,1,1,1,1,1,1,1,1,2,1,0,2,\n"
    "        0,2,1,0,1,1,1,1,1,1,1,1,2,1,0,2,\n"
    "        0,2,1,0,1,1,1,1,1,1,1,1,2,1,0,2,\n"
    "        0,2,1,0,0,0,0,0,0,0,0,0,1,1,0,2,\n"
    "        0,2,1,1,1,1,1,1,1,1,1,1,1,1,0,2,\n"
    "        0,2,2,2,2,2,2,2,2,2,2,2,2,2,1,2,\n"
    "        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1\n"
    "    );\n"
    "    return palette[pixels[int(mod(floor(p.x), 16.0) + mod(floor(16.0-p.y), 16.0) * 16.0)]];\n"
    "}\n"
    "\n"
    "vec4 grassTexture(vec2 p) {\n"
    "    vec4[] palette = vec4[](vec4(0), vec4(0.8, 1.0, 0.0, 1), vec4(0.4, 0.8, 0.0, 1), vec4(0.3, 0.7, 0.0, 1), vec4(0.5, 0.25, 0.25, 1));\n"
    "    int[] pixels = int[](\n"
    "        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,\n"
    "        2,1,1,1,2,2,2,2,1,1,2,2,2,2,2,2,\n"
    "        2,2,2,2,2,3,3,2,2,2,2,3,3,3,3,3,\n"
    "        3,3,3,3,3,4,4,3,3,3,3,4,4,4,4,4,\n"
    "        4,4,4,4,4,0,0,4,4,4,4,0,0,0,0,0,\n"
    "        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n"
    "        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n"
    "        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n"
    "        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n"
    "        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n"
    "        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n"
    "        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n"
    "        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n"
    "        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n"
    "        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n"
    "        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0\n"
    "    );\n"
    "    return palette[pixels[int(mod(floor(p.x), 16.0) + mod(floor(16.0-p.y), 16.0) * 16.0)]];\n"
    "}\n"
    "\n"
    "vec4 playerTexture(vec2 p) {\n"
    "    vec4[] palette = vec4[](vec4(0), vec4(1), vec4(0.8,0.8,0.7,1), vec4(0.7,0.7,0.5,1), vec4(0, 0, 0, 1));\n"
    "    int[] pixels = int[](\n"
    "        0,3,2,1,1,1,1,1,1,1,1,1,0,0,0,0,\n"
    "        3,2,2,1,1,1,1,1,1,1,1,1,1,0,0,0,\n"
    "        3,2,1,1,1,1,1,1,1,1,1,1,1,0,0,0,\n"
    "        3,2,1,4,1,1,1,1,1,1,1,4,1,0,0,0,\n"
    "        3,2,1,1,1,1,1,1,1,1,1,1,1,0,0,0,\n"
    "        3,2,2,1,1,1,1,1,1,1,1,1,1,0,0,0,\n"
    "        0,3,3,2,1,1,1,1,1,1,1,1,0,0,0,0,\n"
    "        0,0,0,0,3,3,2,1,0,0,0,0,0,0,0,0,\n"
    "        0,0,0,3,2,1,1,1,1,0,0,0,0,0,0,0,\n"
    "        0,0,3,1,1,1,1,1,1,0,0,0,0,0,0,0,\n"
    "        0,3,1,3,1,1,1,1,1,3,0,0,0,0,0,0,\n"
    "        0,0,0,3,2,1,1,1,1,0,0,0,0,0,0,0,\n"
    "        0,0,0,3,2,1,1,1,1,0,0,0,0,0,0,0,\n"
    "        0,0,0,3,2,2,1,1,1,0,0,0,0,0,0,0,\n"
    "        0,0,0,3,2,0,0,3,2,0,0,0,0,0,0,0,\n"
    "        0,0,0,3,0,0,0,0,3,0,0,0,0,0,0,0\n"
    "    );\n"
    "    return palette[pixels[int(mod(floor(p.x), 16.0) + mod(floor(16.0-p.y), 16.0) * 16.0)]];\n"
    "}\n"
    "\n"
    "void main() {\n"
    "    vec2 fragCoord = gl_FragCoord.xy;\n"
    "    vec2 uv = fragCoord - floor(iResolution.xy * 0.5);\n"
    "\n"
    "    vec4 playerData = texture(iChannel0, vec2(2.0, 0.0) / vec2(4.0, 1.0)); // Assuming 4x1 buffer\n"
    "\n"
    "    float scale = clamp(round(iResolution.y / 384.0), 1.0, 8.0);\n"
    "    uv /= scale;\n"
    "    uv.y += 24.0;\n"
    "\n"
    "    float rotation = texture(iChannel0, vec2(1.0, 0.0) / vec2(4.0, 1.0)).x;\n"
    "\n"
    "    vec3 rayPos = vec3(uv, -float(MAX_STEPS) * 8.0);\n"
    "    rotate(rayPos.xz, PI * 0.5 - rotation);\n"
    "    vec3 rayDir = vec3(cos(rotation), 0.0, sin(rotation));\n"
    "\n"
    "    HitInfo info = castRay(rayPos, rayDir);\n"
    "\n"
    "    vec3 tileUV = mod(info.pos, vec3(1.0));\n"
    "    vec3 col = vec3(0.0);\n"
    "\n"
    "    if (info.tile.ID == 0) {\n"
    "        col = vec3(0, 0.8, 1.0);\n"
    "    } else {\n"
    "        col = blockTexture(vec2(tileUV.x + tileUV.z, tileUV.y) * 16.0);\n"
    "\n"
    "        if (world(floor(info.pos + rayDir * 0.001) + vec3(0,1,0)).ID == 0) {\n"
    "            vec4 grass = grassTexture(vec2(tileUV.x + tileUV.z, tileUV.y) * 16.0);\n"
    "            if (grass.a == 1.0)\n"
    "                col = grass.rgb;\n"
    "        }\n"
    "\n"
    "        if (world(floor(info.pos - rayDir * 0.001) + vec3(0,1,0)).ID != 0) {\n"
    "            col *= 1.0 - mod(info.pos.y, 1.0) * 0.5;\n"
    "        }\n"
    "        col *= mix(1.0, 0.8, info.normal.z);\n"
    "    }\n"
    "\n"
    "    float playerDepth = dot(playerData.xyz * 0.0625, rayDir);\n"
    "    float worldDepth = dot(info.pos, rayDir);\n"
    "\n"
    "    vec3 tileUV2 = rayPos - playerData.xyz;\n"
    "    rotate(tileUV2.xz, rotation);\n"
    "    vec4 playerColor = playerTexture(tileUV2.zy - 8.0);\n"
    "\n"
    "    if (playerColor.a == 1.0 && abs(tileUV2.z) < 8.0 && abs(tileUV2.y) < 8.0) {\n"
    "        if (mod(playerData.w, 2.0) == 1.0 || playerDepth <= worldDepth) {\n"
    "            col = playerColor.xyz;\n"
    "        } else {\n"
    "            col *= 0.5;\n"
    "        }\n"
    "    }\n"
    "\n"
    "    FragColor = vec4(col, 1.0);\n"
    "}\0";

// Helper function to read shader source from a string
unsigned int compile_shader(const char* source, GLenum type) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    
    // Check for compilation errors
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("Shader compilation failed: %s\n", infoLog);
    }
    
    return shader;
}

// Create shader program
unsigned int create_shader_program() {
    unsigned int vertex_shader = compile_shader(vertex_shader_source, GL_VERTEX_SHADER);
    unsigned int fragment_shader = compile_shader(fragment_shader_source, GL_FRAGMENT_SHADER);
    
    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    
    // Check for linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
        printf("Shader program linking failed: %s\n", infoLog);
    }
    
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    
    return shader_program;
}

// Shader-based renderer implementation
void render_shader(GameState* state) {
    // Create shader program (in a real implementation, this would be cached)
    static unsigned int shader_program = 0;
    if(shader_program == 0) {
        shader_program = create_shader_program();
    }
    
    // Use the shader program
    glUseProgram(shader_program);
    
    // Set uniforms
    int resolution_loc = glGetUniformLocation(shader_program, "iResolution");
    int time_loc = glGetUniformLocation(shader_program, "iTime");
    int frame_loc = glGetUniformLocation(shader_program, "iFrame");
    
    if(resolution_loc != -1) {
        glUniform2f(resolution_loc, SCREEN_WIDTH, SCREEN_HEIGHT);
    }
    
    if(time_loc != -1) {
        glUniform1f(time_loc, state->time);
    }
    
    if(frame_loc != -1) {
        glUniform1i(frame_loc, (int)state->frame_count);
    }
    
    // Set up a basic quad that covers the screen
    float vertices[] = {
        -1.0f,  1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         1.0f,  1.0f, 0.0f
    };
    
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Draw the quad
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
    // Cleanup
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    
    // Swap buffers to display the rendered frame
    SDL_GL_SwapWindow(g_window);
}