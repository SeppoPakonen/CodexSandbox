#define STATE_CHANNEL iChannel0
#define MAP_CHANNEL iChannel1


float TRIANGLE_sign(vec2 p1, vec2 p2, vec2 p3)
    // Pinched from https://www.shadertoy.com/view/4s3fzj
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool TRIANGLE(vec2 vertices[3], vec2 uv)
    // Pinched from https://www.shadertoy.com/view/4s3fzj
{
    bool b1 = TRIANGLE_sign(uv, vertices[0], vertices[1]) < 0.0f;
    bool b2 = TRIANGLE_sign(uv, vertices[1], vertices[2]) < 0.0f;
    bool b3 = TRIANGLE_sign(uv, vertices[2], vertices[0]) < 0.0f;
    
    return (b1 == b2) && (b2 == b3);
}




float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}




void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = fragCoord/iResolution.xy;
    float zoom = get_zoom(STATE_CHANNEL);

    // Coords to center of the screen
    vec2 offset = get_view_position(STATE_CHANNEL);
    
    // Apply offset to get world coordinates of pixel
    vec2 pixel_coord = offset + (uv * vec2(1.0, iResolution.y / iResolution.x));
    vec2 world_coords = pixel_coord * zoom;
    
    vec2 norm_uv = uv * 2.0 - vec2(1.0);
    norm_uv *= vec2(1.0, aspect);
    
    
    mat2 ori = get_ship_matrix(STATE_CHANNEL);
    vec2 VERTS[3] = vec2[](
        ori * SHIP_NOSE,
        ori * SHIP_LEFT_WING,
        ori * SHIP_RIGHT_WING
    );
    bool ship = TRIANGLE(VERTS, norm_uv);
    
    vec4 ship_position = get_ship_world_position(STATE_CHANNEL);
    
    vec4 map = texture(MAP_CHANNEL, uv);
    float map_sdf = map.r;
    
    float light_strength = 1.0;
    float light_rand = rand(vec2(iTime)) * ship_position.w;
    light_strength += light_rand * 0.2;
    
    float light = light_strength / (length(norm_uv - ori * vec2(0.0, -0.02) ) * 8.0 + 1.0);
    float spot = pow(clamp(dot(normalize(norm_uv), ori * vec2(0.0, -1.0)), 0.0, 1.0), 2.0);
    light = light * 0.5 + spot * (0.3 + light_rand * 0.1 + ship_position.w * 0.2); 
    
    // Background
    vec4 background_texture = texture(iChannel2, mix(world_coords, uv, 0.5));
    float background_light = light + dot(
        background_texture.xy - vec2(0.5),
        normalize(norm_uv)
    ) * 0.1;
    vec4 background = vec4(0.4, 0.2, 0.1, 1.0) * background_light + background_light;
    
    
    // Rock edges
    vec4 rock_texture = texture(iChannel3, world_coords) * 0.7;
    float rock_light = light * dot(
        clamp(vec2(0.5) - map.gb, -1.0, 1.0) + rock_texture.rg - vec2(0.5),
        normalize(norm_uv)
    ) + map_sdf;
    
    vec4 rock = rock_texture * rock_light + vec4(rock_light);

    fragColor = background;
    fragColor = mix(fragColor, rock, float(map_sdf < 0.0));
    
    
    fragColor.rgb *= vec3(!ship);
    
}