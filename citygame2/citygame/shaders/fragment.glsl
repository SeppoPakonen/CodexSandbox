#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform float u_time;
uniform vec2 u_resolution;
uniform sampler2D u_texture;

void main()
{
    // Create a simple pattern for the city
    vec2 uv = TexCoord;
    
    // Create grid pattern for city blocks
    vec2 grid = abs(fract(uv * 20.0) - 0.5) / fwidth(uv * 20.0);
    float gridLine = min(grid.x, grid.y);
    float gridFactor = 1.0 - min(gridLine, 1.0);
    
    // Base color (grass/ground)
    vec3 color = vec3(0.2, 0.6, 0.2); // Green for grass
    
    // Add roads (horizontal and vertical lines)
    if (gridFactor > 0.9) {
        color = vec3(0.4); // Gray for roads
    }
    
    // Add some buildings for visual interest
    vec2 blockPos = floor(uv * 20.0);
    float buildingHeight = sin(blockPos.x * 0.5 + u_time * 0.1) * sin(blockPos.y * 0.3 + u_time * 0.15);
    buildingHeight = abs(buildingHeight) * 10.0;
    
    // If this is not a road, maybe draw a building
    if (gridFactor <= 0.9) {
        // Create some "buildings" based on grid position
        if (mod(blockPos.x, 4.0) < 2.0 && mod(blockPos.y, 4.0) < 2.0) {
            // This creates "buildings" in a 2x2 pattern
            float height = mod(blockPos.x + blockPos.y, 5.0);
            if (height > 2.0) {
                // Taller buildings
                color = mix(vec3(0.4, 0.4, 0.5), vec3(0.3, 0.3, 0.4), 
                           fract(uv.y * 10.0 + u_time * 0.5));
            } else {
                // Shorter buildings
                color = vec3(0.5, 0.4, 0.3); // Brownish
            }
        }
    }
    
    FragColor = vec4(color, 1.0);
}