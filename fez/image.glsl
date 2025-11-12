// NOTE:
// iChannel0 = Buffer A

void mainImage(out vec4 fragColor, in vec2 fragCoord) {
    // Normalized pixel coordinates (from 0 to 1)
    vec2 uv = fragCoord - floor(iResolution.xy * 0.5);
    
    vec4 playerData = texelFetch(iChannel0, ivec2(2,0), 0);
    
    // dynamic scaling that always is pixel perfect
    float scale = clamp(round(iResolution.y / 384.0), 1.0, 8.0);
    uv /= scale;
    uv.y += 24.0;

    // animation
    //float rotation = mod(abs(sin(iTime * PI) * 0.5) + iTime * PI * 0.5, PI * 2.0);

    float rotation = texelFetch(iChannel0, ivec2(1,0), 0).x;

    // move the camera back a bunch (avoids clipping)
    vec3 rayPos = vec3(uv, -float(MAX_STEPS) * 8.0);
    rotate(rayPos.xz, PI * 0.5 - rotation);
    vec3 rayDir = vec3(cos(rotation), 0.0, sin(rotation));

    HitInfo info = castRay(rayPos, rayDir);
    
    vec3 tileUV = mod(info.pos, vec3(1.0));
    vec3 col = vec3(0.0);
    
    // sky
    if (info.tile.ID == 0) {
        col = vec3(0, 0.8, 1.0);
        //if (mod(playerData.w, 2.0) == 0.0) col *= 0.5;
    } else {        
        // apply tile texture
        //col = vec3(1.0);
        col = blockTexture(vec2(tileUV.x + tileUV.z, tileUV.y) * 16.0);
        
        // add grass texture if tile above is air
        if (world(floor(info.pos + rayDir * 0.001) + vec3(0,1,0)).ID == 0) {
            vec4 grass = grassTexture(vec2(tileUV.x + tileUV.z, tileUV.y) * 16.0);
            if (grass.a == 1.0)
                col = grass.rgb;
        }
        
        // shadows under tiles
        if (world(floor(info.pos - rayDir * 0.001) + vec3(0,1,0)).ID != 0) {
            col *= 1.0 - mod(info.pos.y, 1.0) * 0.5;
        }
        // basic directional shading
        col *= mix(1.0, 0.8, info.normal.z);
    }

    float playerDepth = dot(playerData.xyz * 0.0625, rayDir);
    float worldDepth = dot(info.pos, rayDir);
    
    //col = vec3(0.2 - worldDepth * 0.1);
    
    tileUV = rayPos - playerData.xyz;
    rotate(tileUV.xz, rotation);
    vec4 playerColor = playerTexture(tileUV.zy - 8.0);
    

    
    
    if (playerColor.a == 1.0 && abs(tileUV.z) < 8.0 && abs(tileUV.y) < 8.0) {
        if (mod(playerData.w, 2.0) == 1.0 || playerDepth <= worldDepth) {
            col = playerColor.xyz;
        } else {
            col *= 0.5;
        }
        
    }
    
   
    
   
    // Output to screen
    fragColor = vec4(col,1.0);
}
