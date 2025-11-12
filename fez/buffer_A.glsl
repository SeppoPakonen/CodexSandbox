// NOTE:
// iChannel0 = Buffer A (self)
// iChannel1 = Keyboard


const float EPSILON = 0.001;

void mainImage( out vec4 fragColor, in vec2 fragCoord) {
    fragCoord -= 0.5;
    fragColor = texelFetch(iChannel0, ivec2(fragCoord), 0);
    
    if (fragCoord.y > 1.0) discard;

    // target rotation
    if (fragCoord.x == 0.0) {
        vec3 playerPos = texelFetch(iChannel0, ivec2(2,0), 0).xyz;
        float rotation = (texelFetch(iChannel1, ivec2(68, 1), 0).x - texelFetch(iChannel1, ivec2(65, 1), 0).x) * PI * 0.5;
        
        fragColor.x += rotation;
        fragColor.x = mod(fragColor.x, 2.0 * PI);
        if (abs(rotation) > 0.0)
            fragColor.y = iTime;
    }
    
    // actual rotation
    if (fragCoord.x == 1.0) {
        if (iFrame == 0)
            fragColor.x = PI;
            
        vec4 target = texelFetch(iChannel0, ivec2(0), 0);
        
        fragColor.x = mod(fragColor.x, 2.0 * PI);
        
        // calculate shortest distance to rotate to target
        float difference = target.x - fragColor.x;
        float difference2 = difference - 2.0 * PI;
        float difference3 = difference + 2.0 * PI;
        if (abs(difference) < abs(difference2))
            if (abs(difference) < abs(difference3))
                difference = difference;
            else
                difference = difference3;
        else
            difference = difference2;
        
        // rotate
        fragColor.x += (min(iTime - target.y, iTime * 0.05) * difference) * iTimeDelta * 32.0;
        
        // snap to nearest cardinal direction
        if (mod(fragColor.x, PI * 0.5) < EPSILON || mod(fragColor.x, PI * 0.5) > PI * 0.5 - EPSILON)
            fragColor.x = mod(round(fragColor.x * 2.0 / PI) * 0.5 * PI, 2.0 * PI);
        
    }
    
    // player position vec4(x, y, z, w)
    // w % 2 == 1: playerIsInFront
    // w % 4 < 2: playerIsGrounded
    if (fragCoord.x == 2.0) {
        if (iFrame == 0)
            fragColor.xyz = vec3(0,-72,0);
    
        float rotation = texelFetch(iChannel0, ivec2(1,0), 0).x;
        
        // don't move player if rotating
        if (mod(rotation, PI * 0.5) < EPSILON && mod(rotation, PI * 0.5) < PI * 0.5 - EPSILON) {
        
            float leftKey = texelFetch(iChannel1, ivec2(37, 0), 0).x;
            float rightKey = texelFetch(iChannel1, ivec2(39, 0), 0).x;
            
            vec2 trig = vec2(cos(rotation), sin(rotation));
            vec3 playerVelocity = texelFetch(iChannel0, ivec2(3,0), 0).xyz;
            
            fragColor.x -= trig.y * (leftKey - rightKey) * (iTimeDelta * 80.0);
            fragColor.z += trig.x * (leftKey - rightKey) * (iTimeDelta * 80.0);

            fragColor.y += playerVelocity.y * iTimeDelta * 16.0;
            
            Tile tile; 
            vec3 samplePos = fragColor.xyz;
            
            // check if player is not behind tiles
            if (mod(fragColor.w, 2.0) == 1.0) {
                samplePos.xz -= trig * 8.0 * float(MAX_STEPS);
            } else {
                for (float steps = 0.0; steps < float(MAX_STEPS); steps++) {
                    tile = world(samplePos * 0.0625);
                    if (tile.ID != 0) {
                        fragColor.w = mod(fragColor.w, 4.0) < 3.0 ? 0.0 : 2.0;
                        break;
                    }
                    samplePos.xz -= trig * 8.0;
                }
                if (tile.ID == 0)
                    fragColor.w = mod(fragColor.w, 4.0) < 3.0 ? 1.0 : 3.0;
                else
                    samplePos = fragColor.xyz;
            }
            
            samplePos -= vec3(0,9,0);
            
            // ground check: raytrace
            for (float steps = 0.0; steps < float(MAX_STEPS); steps++) {
                tile = world(samplePos * 0.0625);
                if (tile.ID != 0) {
                    break;
                }
                samplePos.xz += trig * 16.0;
            }
            // hit
            if (tile.ID != 0 && world(samplePos * 0.0625 + vec3(0,1,0)).ID == 0 && playerVelocity.y < 0.0) {
                fragColor.w = mod(fragColor.w, 2.0) == 0.0 ? 2.0 : 3.0;
                // snap to ground
                fragColor.y = floor(fragColor.y * 0.0625) * 16.0 + 8.0;
                
                if (mod(rotation, PI) == 0.0)
                    fragColor.x = floor(samplePos.x * 0.0625) * 16.0 + 8.0;
                else
                    fragColor.z = floor(samplePos.z * 0.0625) * 16.0 + 8.0;
            } else {
                fragColor.w = mod(fragColor.w, 2.0) == 0.0 ? 0.0 : 1.0;

                    
            }

        } else {
            // player is rotating
            fragColor.w = mod(fragColor.w, 4.0) < 3.0 ? 0.0 : 2.0;
        }
        // respawn
        if (fragColor.y < -256.0) {
            fragColor.xyz = vec3(0,-64,0);
            fragColor.w = mod(fragColor.w, 4.0) < 3.0 ? 0.0 : 2.0;
        }
    }
    
    // player velocity
    if (fragCoord.x == 3.0) {
        vec4 temp = texelFetch(iChannel0, ivec2(2,0), 0);
        vec3 playerPos = temp.xyz;
        
        float rotation = texelFetch(iChannel0, ivec2(1,0), 0).x;
        if (mod(rotation, PI * 0.5) < EPSILON && mod(rotation, PI * 0.5) < PI * 0.5 - EPSILON) {        
            // gravity
            fragColor.y -= (iTimeDelta * 48.0);

            // check for ground
            if (mod(temp.w, 4.0) > 1.0) {
                fragColor.y = 0.0;
                
                // jump
                if (texelFetch(iChannel1, ivec2(32, 0), 0).x > 0.0 || texelFetch(iChannel1, ivec2(38, 0), 0).x > 0.0) {
                    fragColor.y = 18.0;
                }
            }   
        }
        
        if (playerPos.y + fragColor.y < -256.0)
            fragColor.xyz = vec3(0,-1,0);
    }
}

