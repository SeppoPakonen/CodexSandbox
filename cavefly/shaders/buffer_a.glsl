// Contains state

#define STATE_BUFFER iChannel0
#define MAP_BUFFER iChannel1
#define KEY_BUFFER iChannel2


float get_key(int key_code) {
    return texelFetch(KEY_BUFFER, ivec2(key_code,0), 0).x;
}





void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    ivec2 address = ivec2(fragCoord);
    vec2 uv = fragCoord/iResolution.xy;

    
    if (address == A_SHIP_POSITION || address == A_SHIP_VELOCITY) {
        // Physics
        vec4 position = texelFetch(STATE_BUFFER, A_SHIP_POSITION, 0);
        vec4 velocity = texelFetch(STATE_BUFFER, A_SHIP_VELOCITY, 0);
            
        
        float c = cos(position.z);
        float s = sin(position.z);
        mat2 ori = mat2(
            c, s,
            -s, c
        );

        vec4 acceleration = vec4(0.0);


        // Gravity
        acceleration.y -= GRAVITY;

        // Thrusters
        vec2 forwards = ori * vec2(0.0, 1.0);

        float thrust_keys = get_key(KEY_UP);// - get_key(KEY_DOWN));
        position.w = thrust_keys; // Indicate that thrust is being applied to the ship

        acceleration.z += (get_key(KEY_LEFT) - get_key(KEY_RIGHT)) * SHIP_ANGULAR_THRUST;
        acceleration.xy += forwards * SHIP_THRUST * thrust_keys;

        // Damping
        acceleration -= velocity * SHIP_DAMPING;

        // Collision with map
        vec4 top = texture(MAP_BUFFER, vec2(0.5) + ori*SHIP_NOSE * vec2(aspect, 1.0));
        vec4 left = texture(MAP_BUFFER, vec2(0.5) + ori*SHIP_LEFT_WING * vec2(aspect, 1.0));
        vec4 right = texture(MAP_BUFFER, vec2(0.5) + ori*SHIP_RIGHT_WING * vec2(aspect, 1.0));

        if (top.r < 0.0) {
            position.xy -= (top.gb - vec2(0.5)) * (top.r) / 8.0;
            velocity.xy *= 1.0 - GROUND_FRICTION;
        }
        if (left.r < 0.0) {
            position.xy -= (left.gb - vec2(0.5)) * (left.r) / 8.0; 
            velocity.xy *= 1.0 - GROUND_FRICTION;
        }
        if (right.r < 0.0) {
            position.xy -= (right.gb - vec2(0.5)) * (right.r) / 8.0; 
            velocity.xy *= 1.0 - GROUND_FRICTION;
        }
        

        velocity += acceleration * iTimeDelta;
        position += velocity * iTimeDelta;
        
        if (iTime < 0.1) {
            acceleration = vec4(0.0);
        	position = vec4(0.0, 0.2, 0.0, 0.0);
        	velocity = vec4(0.0);
        }
        
        // Save state
        if (address == A_SHIP_POSITION) {
            fragColor = position;
        }
        if (address == A_SHIP_VELOCITY) {
            fragColor = velocity;
        }
    
    
    }
    
    //fragColor = vec4(0.0);
    
}