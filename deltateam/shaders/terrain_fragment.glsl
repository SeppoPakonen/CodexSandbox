#version 330 core

out vec4 FragColor;
in vec2 TexCoord;

uniform vec3 iResolution;
uniform float iTime;
uniform vec4 iMouse;
uniform sampler2D iChannel0; // Buffer for camera position/rotation
uniform sampler2D iChannel1; // Heightmap texture
uniform sampler2D iChannel2; // Ground texture
uniform sampler2D iChannel3; // Keyboard input texture

// Function to sample keyboard input (simulated)
float ReadKey(int key) {
    return texture(iChannel3, vec2((float(key) + 0.5) / 256.0, 0.25)).x;
}

// Heightmap function from Shadertoy
float heightmap(vec2 pos) {
    return min(1.4 - pow(texture(iChannel1, pos * 0.002, 3.0).r, 0.5), 1.0) * 35.0;
}

// Scene function from Shadertoy
vec2 scene(vec3 point) {
    float hmp = heightmap(point.xz);
    float d = point.y + hmp;
    return vec2(d, 0.0);
}

// Ray marching function from Shadertoy
float RayMarch(vec3 ro, vec3 rd, int steps) {
    float totalDistance = 0.0;
    vec3 intersection;
    float distance;
    
    for(int i = 0; i < steps; ++i) {
        intersection = ro + rd * totalDistance;
        vec2 s = scene(intersection);
        distance = s.x;
        totalDistance += distance;

        if(distance <= 0.002 || totalDistance >= 530.0) {
            break;
        }
    }
    
    return totalDistance;
}

// Set camera function from Shadertoy
mat3 setCamera(vec3 ro, vec3 ta, float cr) {
    vec3 cw = normalize(ta - ro);
    vec3 cp = vec3(sin(cr), cos(cr), 0.0);
    vec3 cu = normalize(cross(cw, cp));
    vec3 cv = normalize(cross(cu, cw));
    return mat3(cu, cv, cw);
}

void main() {
    // Resolution of the output (640x360 as in original)
    const vec2 res = vec2(640.0, 360.0);
    vec2 fragCoord = TexCoord * iResolution.xy;
    
    if(fragCoord.x < res.x && fragCoord.y < res.y) {
        // Get player position and rotation from buffer
        vec4 playerPos = texture(iChannel0, vec2(0.5, 0.5) / iResolution.xy, -100.0);
        vec3 origin = playerPos.xyz;
        vec3 wp_origin = vec3(-1.2, 0.8, 0.0);
        
        // Adjust field of view based on player movement
        float fov = (playerPos.w < 0.2) ? 8.0 : 1.5;
        
        // Calculate UV coordinates
        vec2 uv = (fragCoord.xy / res * 2.0) - 1.0;
        uv.x *= res.x / res.y;
        
        // Get camera rotation
        vec4 camRot = texture(iChannel0, vec2(1.5, 0.5) / iResolution.xy, -100.0) * 6.28318530718;
        
        // Calculate camera direction
        vec3 camdir = vec3(0.0, 0.0, -1.0);
        camdir.zy = camdir.zy * cos(camRot.x) + sin(camRot.x) * vec2(1, -1) * camdir.yz;
        camdir.xz = camdir.xz * cos(camRot.y) + sin(camRot.y) * vec2(1, -1) * camdir.zx;
        
        // Set up camera
        vec3 ro = origin;
        vec3 ta = origin + camdir;
        
        // Camera-to-world transformation
        mat3 ca = setCamera(ro, ta, 0.0);
        
        // Ray direction
        vec3 direction = normalize(vec3(uv.xy, fov));
        
        float distance;
        float totalDistance = 0.5; // NEAR_CLIP
        float m = 0.0;
        
        vec3 Col = vec3(1.0);
        
        // Render weapon (first person view)
        for(int i = 0; i < 48; ++i) {
            vec3 intersection = wp_origin + direction * totalDistance;
            
            // Simplified weapon model (this would need to be fully implemented based on Shadertoy code)
            float weapon_dist = length(intersection - wp_origin) - 0.5; // Placeholder
            distance = weapon_dist;
            m = 228.0; // Material ID
            
            totalDistance += distance;
            
            if(distance <= 0.02 || totalDistance >= 6.8) {
                break;
            }
        }
        
        float lt = clamp(-3.0 * wp_origin.x + 0.7 * wp_origin.y, 0.3, 1.0);
        
        if(m > 100.0) {
            lt *= lt + texture(iChannel2, 1.52 * vec2(wp_origin.x + wp_origin.y, wp_origin.z - wp_origin.x)).x;
        } else {
            lt *= 3.0;
        }
        
        Col = 0.45 + 0.3 * sin(vec3(0.05, 0.08, 0.10) * (m - 1.0));
        Col *= lt;
        
        // If weapon wasn't hit, render terrain
        if(totalDistance >= 6.5) {
            totalDistance = 0.5; // NEAR_CLIP
            direction = ca * direction;
            
            for(int i = 0; i < 128; ++i) { // RAY_STEPS
                vec3 intersection = origin + direction * totalDistance;
                intersection = floor(intersection * 12.0) / 12.0; // Quantize position
                vec2 s = scene(intersection);
                distance = s.x;
                float gtex = texture(iChannel2, intersection.xz).r;
                totalDistance += distance - gtex * 0.4;

                if(distance <= 0.002 || totalDistance >= 530.0) { // FAR_CLIP
                    break;
                }
            }
            
            // Apply terrain coloring
            float tex = texture(iChannel1, intersection.xz * 0.045).r;
            float tex2 = texture(iChannel2, vec2(intersection.y * 0.2 - 0.4 * tex, 0.66) * 0.05).r;
            Col = vec3((0.4 + 0.6 * tex2), (0.3 + 0.7 * tex2), (0.2 + 0.8 * tex2)) * 
                  mix(vec3(0.4, 0.5, 0.1), vec3(1.0, 0.8, 0.2), 
                      clamp(intersection.y * 0.06 + 1.2 + 1.2 * (1.0 - tex), 0.0, 1.0));
            
            // Apply fog effect
            float fog = clamp(pow(max((1.0 - (totalDistance * 0.0015)), 0.0), 2.2), 0.0, 1.0);
            
            Col = mix(vec3(0.12, 0.38, 0.6) + vec3(0.34, 0.58, 0.79) * clamp(pow(1.0 - direction.y, 2.2), 0.0, 1.0), Col, fog);
        }
        
        // Draw simple crosshair (HUD)
        vec2 csu = abs(vec2(fragCoord.x - 320.0, fragCoord.y - 180.0));
        float hud = (1.0 - step(1.0, csu.x)) * (step(12.0, csu.y) - step(16.0 + fov * 2.0, csu.y)) + 
                    (1.0 - step(1.0, csu.y)) * (step(12.0, csu.x) - step(16.0 + fov * 2.0, csu.x));
        
        Col = mix(Col, vec3(0.0, 1.0, 0.0), hud);
        
        FragColor = vec4(Col, 1.0);
    } else {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}