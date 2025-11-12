#define SHIP_SIZE 0.05
#define SHIP_DAMPING vec4(3.0, 3.0, 10.0, 0.0)
#define SHIP_THRUST 3.0
#define SHIP_ANGULAR_THRUST 48.0
#define GRAVITY 0.60
#define GROUND_FRICTION 0.2

#define SEED 10.0

#define KEY_LEFT  37
#define KEY_UP    38
#define KEY_RIGHT 39
#define KEY_DOWN  40


#define A_SHIP_POSITION ivec2(1.0, 1.0) // red green = location blue = rotation
#define A_SHIP_VELOCITY ivec2(1.0, 2.0) // red green = linear blue = angular


#define SHIP_NOSE vec2(0.0, 1.0) * SHIP_SIZE
#define SHIP_LEFT_WING vec2(0.5, -0.3) * SHIP_SIZE
#define SHIP_RIGHT_WING vec2(-0.5, -0.3) * SHIP_SIZE


#define aspect (iResolution.y / iResolution.x)



float get_zoom(sampler2D state_buffer) {
    return 4.0;
}


vec4 get_ship_world_position(sampler2D state_buffer) {
    return texelFetch(state_buffer, A_SHIP_POSITION, 0);
}
vec4 get_ship_velocity(sampler2D state_buffer) {
    return texelFetch(state_buffer, A_SHIP_VELOCITY, 0);
}


mat2 get_ship_matrix(sampler2D state_buffer) {
    // Returns ship vertices rotated into world space
    vec4 world_pos = get_ship_world_position(state_buffer);
    float angle = world_pos.z;
    float c = cos(angle);
    float s = sin(angle);
    
    return mat2(
        c, s,
        -s, c
    );
}

vec2 get_view_position(sampler2D state_buffer) {
    return get_ship_world_position(state_buffer).xy;
}