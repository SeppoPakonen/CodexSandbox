#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "game.h"
#include "physics.h"
#include "renderer.h"

// Define hash functions here
float hash(float x) {
    return fmodf(sinf(x * 0.0007f) * 29835.24389f, 1.0f);
}

float hash_vec2(Vec2 v) {
    return hash(v.x * 23.17f + v.y * 17.23f);
}

// Game state
static Block blocks[BMAX_X][BMAX_Y];
static Cell cells[BMAX_X][BMAX_Y][BLOCK_BUFFER_X * BLOCK_BUFFER_Y];
static int game_initialized = 0;
static int game_width = 800;
static int game_height = 600;
static int renderer_mode = RENDERER_OLD_SCHOOL;

int init_game() {
    // Initialize the game
    memset(blocks, 0, sizeof(blocks));
    memset(cells, 0, sizeof(cells));
    
    // Initialize the renderer
    if (init_renderer(game_width, game_height, renderer_mode) != 0) {
        return -1;
    }
    
    // Generate initial terrain
    generate_terrain();
    
    game_initialized = 1;
    return 0;
}

void update_game() {
    if (!game_initialized) return;
    
    // Update all blocks and cells
    for (int x = 0; x < BMAX_X; x++) {
        for (int y = 0; y < BMAX_Y; y++) {
            // Update block connections and properties
            Block *b = &blocks[x][y];
            
            // Update connections
            int conX = 0, conY = 0, conns = 0, dmin = MAX_DIST, dir = 0, buildings = 0;
            
            // Process neighboring blocks to determine connections
            for (int i = 0; i < 4; i++) {  // 4 directions: North, East, South, West
                int nx = x, ny = y;
                
                switch (i) {
                    case 0: nx++; break;  // East
                    case 1: ny++; break;  // North
                    case 2: nx--; break;  // West
                    case 3: ny--; break;  // South
                }
                
                if (nx >= 0 && nx < BMAX_X && ny >= 0 && ny < BMAX_Y) {
                    Block *nb = &blocks[nx][ny];
                    
                    int bit = (int)powf(2.0f, (float)i);
                    int bitn = (int)powf(2.0f, (float)((i + 2) % 4));
                    
                    // Check if both blocks allow connection in this direction
                    if ((b->aconns & bit) > 0 && (nb->aconns & bitn) > 0) {
                        if ((nb->btype > 0) && i % 2 == 0) conY++;  // East/West connection
                        if ((nb->btype > 0) && i % 2 == 1) conX++;  // North/South connection
                        if (nb->btype > 0 || nb->ltype == 3) conns |= bit;
                        if (nb->btype == 0 && nb->ltype > 0) buildings++;
                    }
                    
                    // Find minimum distance to center
                    if (nb->dist < dmin) {
                        dmin = nb->dist;
                        dir = i;
                    }
                }
            }
            
            // Update distance from center
            if (b->ltype == 3) {  // Center block
                b->dist = 0;
            } else if (dmin < b->dist && b->btype > 0) {
                b->dist = dmin + 1;
                b->dir = dir;
            } else if (b->dist < dmin) {
                b->dist = MAX_DIST;
            }
            
            // Update block type based on connections (simplified)
            if (conX + conY < 1 && b->btype != 0) {
                b->btype = 0;  // Remove unconnected blocks
            } else if (conX + conY == 3 && b->btype != 0) {
                b->btype = 2;  // Semaphore
            } else if (conX + conY == 4 && b->btype != 0) {
                b->btype = 5;  // Overpass
            } else if (conX + conY == 2 && b->btype != 0) {
                b->btype = 4;  // Road
            }
            
            b->conns = conns;
            
            // Update semaphore state (changes every 5 seconds)
            if (b->btype == 2) {
                // In a real implementation, we'd use actual time
                // For now, we'll just toggle based on frame count
                static int frame_count = 0;
                b->semaphore = (frame_count / 300) % 2;  // Toggle every ~5 seconds at 60fps
                frame_count++;
            }
            
            // Update vehicles in the block
            for (int pos = 0; pos < BLOCK_BUFFER_X * BLOCK_BUFFER_Y; pos++) {
                Cell *c = &cells[x][y][pos];
                
                // Set road state
                c->road = is_road(b, pos);
                c->shift = is_shift(b, pos);
                
                // Update vehicle physics
                update_vehicle_physics(c, b, b->btype);
            }
        }
    }
    
    // Update counters
    update_counters();
    
    // Handle input
    handle_input();
}

void render_game() {
    if (!game_initialized) return;
    
    if (renderer_mode == RENDERER_OLD_SCHOOL) {
        render_city_oldschool();
    } else {
        render_city_modern();
    }
}

void cleanup_game() {
    cleanup_renderer();
    game_initialized = 0;
}

void handle_input() {
    // Placeholder for input handling
}

void handle_mouse_click(int x, int y, int button) {
    // Placeholder for mouse click handling
}

void handle_key_press(int key) {
    // Placeholder for key press handling
}

void reset_game() {
    init_game();
}

int check_win_condition() {
    // Check if all taxis reached the center
    // Simplified win condition: all blocks are connected and there are no unconnected blocks
    int unconnected_count = 0;
    
    for (int x = 0; x < BMAX_X; x++) {
        for (int y = 0; y < BMAX_Y; y++) {
            Block *b = &blocks[x][y];
            if (b->th >= 1 && b->btype > 0 && b->dist >= MAX_DIST) {
                unconnected_count++;
            }
        }
    }
    
    return unconnected_count == 0;
}

void update_counters() {
    // Update vehicle counters for each block
    for (int x = 0; x < BMAX_X; x++) {
        for (int y = 0; y < BMAX_Y; y++) {
            Block *b = &blocks[x][y];
            b->counter = 0.0f;
            
            // Count vehicles in the block
            for (int pos = 1; pos < 36; pos++) {  // Starting from 1 to skip block coord
                Cell *c = &cells[x][y][pos];
                if (c->road && c->v[0].vtype > 0) {
                    b->counter += 1.0f;
                }
            }
        }
    }
}

Block* get_block(int x, int y) {
    if (x >= 0 && x < BMAX_X && y >= 0 && y < BMAX_Y) {
        return &blocks[x][y];
    }
    return NULL;
}

Cell* get_cell(int x, int y, int z) {
    if (x >= 0 && x < BMAX_X && y >= 0 && y < BMAX_Y && z >= 0 && z < BLOCK_BUFFER_X * BLOCK_BUFFER_Y) {
        return &cells[x][y][z];
    }
    return NULL;
}

void set_block_type(int x, int y, int btype) {
    Block *b = get_block(x, y);
    if (b) {
        b->btype = btype;
    }
}

void set_block_ltype(int x, int y, int ltype) {
    Block *b = get_block(x, y);
    if (b) {
        b->ltype = ltype;
    }
}

// Simple noise function for terrain generation
float noise(float x, float y) {
    int i = (int)(x * 100 + y * 57);
    i = (i << 13) ^ i;
    return (1.0f - ((i * (i * i * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
}

void generate_terrain() {
    // Generate initial blocks with terrain
    for (int x = 0; x < BMAX_X; x++) {
        for (int y = 0; y < BMAX_Y; y++) {
            // Calculate terrain height based on distance from center and noise
            float dist_from_center = sqrtf((x - BMAX_X/2.0f)*(x - BMAX_X/2.0f) + (y - BMAX_Y/2.0f)*(y - BMAX_Y/2.0f));
            float max_dist = sqrtf((BMAX_X/2.0f)*(BMAX_X/2.0f) + (BMAX_Y/2.0f)*(BMAX_Y/2.0f));
            
            float height_factor = 1.0f - dist_from_center / max_dist;
            int terrain_height = (int)(height_factor * THEIGHT);
            
            // Add some noise to make it more interesting
            terrain_height += (int)(noise(x*0.1f, y*0.1f) * 20.0f);
            terrain_height = terrain_height > 0 ? terrain_height : 0;
            
            // Determine block type based on terrain and position
            int block_type = 0;  // Default: empty
            int land_type = 0;   // Default: empty
            
            // Center area - special area
            if (x > 7 && x < 11 && y > 7 && y < 11) {
                land_type = 3;  // Center
                terrain_height = terrain_height > 10 ? terrain_height : 10;
                block_type = 0;  // No block type in center, just land
            } 
            // Around center - roundabout
            else if (x > 6 && x < 12 && y > 6 && y < 12) {
                block_type = 1;  // Roundabout
            }
            // Higher terrain areas - buildings
            else if (terrain_height > 50) {
                block_type = 0;  // No road on high terrain by default
                land_type = 2;   // Building
            }
            // Elsewhere - potentially roads
            else {
                // Randomly decide if it's a road, building, or empty
                float rand_val = hash_vec2((Vec2){x*100.0f, y*100.0f});
                if (rand_val < 0.3f) {
                    block_type = 0;
                    land_type = 0;
                } else if (rand_val < 0.5f) {
                    block_type = 4;  // Road
                } else if (rand_val < 0.6f) {
                    block_type = 2;  // Semaphore
                } else {
                    block_type = 0;
                    land_type = 2;   // Building
                }
            }
            
            // Initialize the block
            init_block(&blocks[x][y], (Vec2){x, y}, block_type, land_type, 
                      50 + (int)(hash_vec2((Vec2){x, y}) * (HEIGHT - 50)), terrain_height);
            
            // Initialize cells within the block
            for (int pos = 0; pos < BLOCK_BUFFER_X * BLOCK_BUFFER_Y; pos++) {
                cells[x][y][pos].block_pos.x = x;
                cells[x][y][pos].block_pos.y = y;
                cells[x][y][pos].pos = pos;
                cells[x][y][pos].road = is_road(&blocks[x][y], pos);
                cells[x][y][pos].shift = is_shift(&blocks[x][y], pos);
                
                // Initialize vehicles randomly based on density
                for (int lane = 0; lane < LANES; lane++) {
                    if (hash_vec2((Vec2){x, y + pos + lane*10.0f}) < VEHICLE_DENSITY) {
                        // Add a vehicle
                        init_vehicle(&cells[x][y][pos].v[lane], 
                                    (int)(1.5f + hash_vec2((Vec2){x*10.0f, y*10.0f + pos})*1.4f), 
                                    3.0f/VSTEP, -0.52f, 0, 0, lane);
                    } else {
                        init_vehicle(&cells[x][y][pos].v[lane], 0, 0.0f, 0.0f, 0, 0, lane);
                    }
                }
            }
        }
    }
}