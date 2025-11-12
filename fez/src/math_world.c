#include "../include/game.h"

void rotate_vec2(float* x, float* y, float angle) {
    float cos_a = cosf(angle);
    float sin_a = sinf(angle);
    float new_x = cos_a * (*x) + sin_a * (*y);
    float new_y = -sin_a * (*x) + cos_a * (*y);
    *x = new_x;
    *y = new_y;
}

Vec3 vec3_add(Vec3 a, Vec3 b) {
    return (Vec3){a.x + b.x, a.y + b.y, a.z + b.z};
}

Vec3 vec3_sub(Vec3 a, Vec3 b) {
    return (Vec3){a.x - b.x, a.y - b.y, a.z - b.z};
}

Vec3 vec3_mul_scalar(Vec3 v, float s) {
    return (Vec3){v.x * s, v.y * s, v.z * s};
}

float vec3_dot(Vec3 a, Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float vec3_length(Vec3 v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vec3 vec3_normalize(Vec3 v) {
    float len = vec3_length(v);
    if(len > 0.0f) {
        return (Vec3){v.x / len, v.y / len, v.z / len};
    }
    return (Vec3){0.0f, 0.0f, 0.0f};
}

float mod_func(float x, float y) {
    return fmodf(x, y);
}

// World generation function based on the GLSL version
Tile world(Vec3 p) {
    p.x = floorf(p.x) + 0.5f;
    p.y = floorf(p.y) + 0.5f;
    p.z = floorf(p.z) + 0.5f;

    // Cube 1: near origin
    if (fabsf(p.y + 6.0f) < 1.0f && fabsf(p.x - 1.0f) < 2.0f && fabsf(p.z - 4.0f) < 5.0f) 
        return (Tile){1};

    // Cube 2
    if (fabsf(p.y + 4.0f) < 2.0f && fabsf(p.x - 3.0f) < 2.0f && fabsf(p.z - 6.0f) < 2.0f) 
        return (Tile){1};

    // Cube 3
    if (fabsf(p.y - 4.0f) < 1.0f && fabsf(p.x + 4.5f) < 1.5f && fabsf(p.z - 5.0f) < 1.0f) 
        return (Tile){1};

    // Cube 4
    if (fabsf(p.y - 0.5f) < 6.5f && fabsf(p.x - 5.0f) < 2.0f && fabsf(p.z + 1.5f) < 4.0f) 
        return (Tile){1};

    // Cube 5
    if (fabsf(p.y - 4.0f) < 1.0f && fabsf(p.x - 4.0f) < 1.0f && fabsf(p.z + 3.0f) < 2.0f) 
        return (Tile){1};

    // Cube 6
    if (fabsf(p.y + 1.0f) < 1.0f && fabsf(p.x - 7.0f) < 2.0f && fabsf(p.z + 4.0f) < 1.0f) 
        return (Tile){1};

    // Sphere
    if (sqrtf(p.x*p.x + p.y*p.y + p.z*p.z + 16*p.x + 18*p.z + 145) < 2.5f) 
        return (Tile){1};

    return (Tile){0};
}

// Ray casting function - simplified C version of the GLSL algorithm
HitInfo cast_ray(Vec3 ray_pos, Vec3 ray_dir) {
    Tile tile;
    Vec3 scaled_ray_pos = vec3_mul_scalar(ray_pos, 0.0625f);

    // DDA raycasting algorithm
    Vec3 map_pos = {(float)floorf(scaled_ray_pos.x), (float)floorf(scaled_ray_pos.y), (float)floorf(scaled_ray_pos.z)};
    Vec3 delta_dist = {
        fabsf(sqrtf(ray_dir.x * ray_dir.x + ray_dir.y * ray_dir.y + ray_dir.z * ray_dir.z) / ray_dir.x),
        fabsf(sqrtf(ray_dir.x * ray_dir.x + ray_dir.y * ray_dir.y + ray_dir.z * ray_dir.z) / ray_dir.y),
        fabsf(sqrtf(ray_dir.x * ray_dir.x + ray_dir.y * ray_dir.y + ray_dir.z * ray_dir.z) / ray_dir.z)
    };
    Vec3 ray_step = {
        (ray_dir.x >= 0.0f) ? 1.0f : -1.0f,
        (ray_dir.y >= 0.0f) ? 1.0f : -1.0f,
        (ray_dir.z >= 0.0f) ? 1.0f : -1.0f
    };
    Vec3 side_dist = {
        (ray_step.x * (map_pos.x - scaled_ray_pos.x) + (ray_step.x * 0.5f) + 0.5f) * delta_dist.x,
        (ray_step.y * (map_pos.y - scaled_ray_pos.y) + (ray_step.y * 0.5f) + 0.5f) * delta_dist.y,
        (ray_step.z * (map_pos.z - scaled_ray_pos.z) + (ray_step.z * 0.5f) + 0.5f) * delta_dist.z
    };

    Vec3 mask = {0.0f, 0.0f, 0.0f};
    
    for (int i = 0; i < MAX_STEPS; i++) {
        tile = world(map_pos);
        if (tile.ID != 0) break;
        
        // Find which face was hit
        if (side_dist.x < side_dist.y && side_dist.x < side_dist.z) {
            side_dist.x += delta_dist.x;
            map_pos.x += ray_step.x;
            mask = (Vec3){1.0f, 0.0f, 0.0f};
        } else if (side_dist.y < side_dist.z) {
            side_dist.y += delta_dist.y;
            map_pos.y += ray_step.y;
            mask = (Vec3){0.0f, 1.0f, 0.0f};
        } else {
            side_dist.z += delta_dist.z;
            map_pos.z += ray_step.z;
            mask = (Vec3){0.0f, 0.0f, 1.0f};
        }
    }

    // Calculate intersection point
    Vec3 hit_pos = scaled_ray_pos;
    if (mask.x > 0.5f) {
        hit_pos.x = (map_pos.x + (ray_dir.x < 0.0f ? 1.0f : 0.0f));
        hit_pos.y = scaled_ray_pos.y + (hit_pos.x - scaled_ray_pos.x) * ray_dir.y / ray_dir.x;
        hit_pos.z = scaled_ray_pos.z + (hit_pos.x - scaled_ray_pos.x) * ray_dir.z / ray_dir.x;
    } else if (mask.y > 0.5f) {
        hit_pos.y = (map_pos.y + (ray_dir.y < 0.0f ? 1.0f : 0.0f));
        hit_pos.x = scaled_ray_pos.x + (hit_pos.y - scaled_ray_pos.y) * ray_dir.x / ray_dir.y;
        hit_pos.z = scaled_ray_pos.z + (hit_pos.y - scaled_ray_pos.y) * ray_dir.z / ray_dir.y;
    } else if (mask.z > 0.5f) {
        hit_pos.z = (map_pos.z + (ray_dir.z < 0.0f ? 1.0f : 0.0f));
        hit_pos.x = scaled_ray_pos.x + (hit_pos.z - scaled_ray_pos.z) * ray_dir.x / ray_dir.z;
        hit_pos.y = scaled_ray_pos.y + (hit_pos.z - scaled_ray_pos.z) * ray_dir.y / ray_dir.z;
    }
    
    return (HitInfo){tile, hit_pos, mask};

    return (HitInfo){tile, ray_pos, mask};
}

// Simple texture functions - just return basic colors for now
Vec3 get_block_texture(Vec2 uv) {
    // Return a simple color based on UV coordinates
    float x = fmodf(fabsf(uv.x), 16.0f);
    float y = fmodf(fabsf(uv.y), 16.0f);
    
    // Simple pattern based on position
    if ((int)x % 4 == 0 || (int)y % 4 == 0) {
        return (Vec3){0.8f, 0.5f, 0.25f};  // Brown
    } else {
        return (Vec3){0.5f, 0.25f, 0.25f};  // Dark red
    }
}

Vec4 get_grass_texture(Vec2 uv) {
    float x = fmodf(fabsf(uv.x), 16.0f);
    float y = fmodf(fabsf(uv.y), 16.0f);
    
    // Simple green pattern
    if ((int)x % 3 == 0 && (int)y % 3 == 0) {
        return (Vec4){0.4f, 0.8f, 0.0f, 1.0f};  // Green
    } else if ((int)x % 5 == 0) {
        return (Vec4){0.8f, 1.0f, 0.0f, 1.0f};  // Bright green
    } else {
        return (Vec4){0.3f, 0.7f, 0.0f, 1.0f};  // Darker green
    }
}

Vec4 get_player_texture(Vec2 uv) {
    // Return a simple player color
    float x = fmodf(fabsf(uv.x), 16.0f);
    float y = fmodf(fabsf(uv.y), 16.0f);
    
    if (x < 8.0f && y < 4.0f) {
        return (Vec4){0.8f, 0.8f, 0.7f, 1.0f};  // Skin tone
    } else if (x > 4.0f && x < 12.0f && y > 4.0f && y < 8.0f) {
        return (Vec4){1.0f, 1.0f, 1.0f, 1.0f};  // White
    } else {
        return (Vec4){0.0f, 0.0f, 0.0f, 1.0f};  // Black
    }
}