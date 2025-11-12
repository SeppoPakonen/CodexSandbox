#version 330 core

out vec4 FragColor;
in vec2 TexCoord;

uniform vec3 iResolution;
uniform float iTime;
uniform vec4 iMouse;
uniform sampler2D iChannel1; // Heightmap texture
uniform sampler2D iChannel2; // Ground texture

// Classic raycasting renderer - simplified version of original Shadertoy
void main()
{
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = TexCoord;
    
    // Calculate ray direction based on field of view
    vec2 screenPos = uv * 2.0 - 1.0; // Convert to [-1, 1] range
    screenPos.x *= iResolution.x / iResolution.y; // Aspect ratio correction
    
    // Player position and direction (these would be passed in as uniforms)
    vec3 playerPos = vec3(0.0, 0.0, 0.0);  // This should come from game state
    vec3 playerDir = vec3(0.0, 0.0, -1.0); // This should come from game state
    vec3 playerPlane = vec3(0.66, 0.0, 0.0); // Camera plane
    
    // Calculate ray direction for current pixel
    vec3 rayDir = vec3(screenPos.x * playerPlane.x + playerDir.x,
                       0.0,
                       screenPos.y * playerPlane.y + playerDir.z);
    
    // Raycasting variables
    ivec2 mapPos = ivec2(floor(playerPos.xz));
    vec2 deltaDist = vec2(length(vec2(rayDir.x, rayDir.z)), length(vec2(rayDir.z, rayDir.x)));
    ivec2 step;
    vec2 sideDist;
    
    // Determine step and initial sideDist
    if (rayDir.x < 0) {
        step.x = -1;
        sideDist.x = (playerPos.x - float(mapPos.x)) * deltaDist.x;
    } else {
        step.x = 1;
        sideDist.x = (float(mapPos.x + 1) - playerPos.x) * deltaDist.x;
    }
    
    if (rayDir.z < 0) {
        step.y = -1;
        sideDist.y = (playerPos.z - float(mapPos.y)) * deltaDist.y;
    } else {
        step.y = 1;
        sideDist.y = (float(mapPos.y + 1) - playerPos.z) * deltaDist.y;
    }

    // Perform DDA (Digital Differential Analysis)
    float dist = 0.0;
    int hit = 0;
    int side = 0; // 0 for x-side, 1 for y-side
    
    // This is a simplified version - in a real implementation, you would have a map to check
    // For now, we'll use the heightmap as our "world" and visualize it
    for (int i = 0; i < 64; ++i) {
        if (sideDist.x < sideDist.y) {
            sideDist.x += deltaDist.x;
            mapPos.x += step.x;
            side = 0;
        } else {
            sideDist.y += deltaDist.y;
            mapPos.y += step.y;
            side = 1;
        }
        
        // Sample heightmap at current position
        float height = texture(iChannel1, vec2(mapPos) * 0.001).r * 10.0;
        float playerHeight = playerPos.y + height;
        
        // Check if we've hit a wall (simplified)
        if (playerHeight > 0.0) { 
            hit = 1;
            break;
        }
    }
    
    // Calculate distance projected on camera direction
    if (side == 0) {
        dist = (float(mapPos.x) - playerPos.x + (1.0 - float(step.x)) * 0.5) / rayDir.x;
    } else {
        dist = (float(mapPos.y) - playerPos.z + (1.0 - float(step.y)) * 0.5) / rayDir.z;
    }

    // Calculate wall height
    float wallHeight = iResolution.y / dist;
    
    // Calculate shading based on distance and side
    float shade = 1.0 / (1.0 + dist * 0.1); // Simple distance-based shading
    if (side == 1) shade *= 0.8; // Darker shade for y-side walls

    // Create a simple color based on distance
    vec3 col = vec3(0.5, 0.7, 1.0) * shade; // Blueish color with shading
    
    // For the ground and ceiling, we'll use a different approach
    float wallEnd = 0.5 + wallHeight * 0.5;
    if (uv.y > wallEnd) {
        // Ground rendering
        vec2 groundPos = playerPos.xz + rayDir.xz * dist;
        float groundTex = texture(iChannel2, groundPos * 0.01).r;
        col = vec3(0.3, 0.2, 0.1) * (0.7 + 0.3 * groundTex); // Brown ground
    }
    
    FragColor = vec4(col, 1.0);
}