// This buffer contains the map
// Red channel: 0 if ground, 1 if air


#define TEX_CHANNEL iChannel0
#define TEX_SCALE vec2(0.01, 0.02)

#define STATE_CHANNEL iChannel1


float rand(ivec2 ico){
    vec2 co = vec2(ico);
    co += SEED;
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}


bool get_tile(ivec2 co) {
    bool up = rand(co + ivec2(0, 1)) > 0.5;
    bool down = rand(co + ivec2(0, -1)) > 0.5;
    bool left = rand(co + ivec2(-1, 0)) > 0.5;
    bool right = rand(co + ivec2(1, 0)) > 0.5;
    
    bool here = rand(co) > 0.5;
    
    return !((up == down) && (left == right) && (up != right)) && here;
}


float get_map_sdf(vec2 pixel_coord) {
    
    
    // Calculate the tile X and Y IDs for this pixel
    vec2 inner_coord = mod(pixel_coord, 1.0);
    
    ivec2 tile_ids = ivec2(pixel_coord - inner_coord);
    
    float a = float(get_tile(tile_ids));
    float b = float(get_tile(tile_ids + ivec2(1.0, 0.0)));
    float c = float(get_tile(tile_ids + ivec2(0.0, 1.0)));
    float d = float(get_tile(tile_ids + ivec2(1.0, 1.0)));
    
    return mix(
        mix(a, b, inner_coord.x),
        mix(c, d, inner_coord.x),
    	inner_coord.y
    );
}


void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = fragCoord/iResolution.xy;
    float zoom = get_zoom(STATE_CHANNEL);

    // Coords to center of the screen
    vec2 offset = get_view_position(STATE_CHANNEL);
    
    // Apply offset to get world coordinates of pixel
    vec2 pixel_coord = offset + (uv * vec2(1.0, aspect));
    vec2 world_coords = pixel_coord * zoom;

    
    float raw_sdf = 0.6 - get_map_sdf(world_coords);
    float tex = texture(TEX_CHANNEL, world_coords * TEX_SCALE).r;
    
    float detailed_sdf = (raw_sdf) - tex * 0.6;
    
    
    float is_ground = float(detailed_sdf > 0.0);
    
    vec2 norm = normalize(vec2(
        dFdx(detailed_sdf),
        dFdy(detailed_sdf)
    )) + vec2(0.5);

    // Output to screen
    fragColor = vec4(
        detailed_sdf,
        norm.x,
        norm.y,
        0.0
    );
}
