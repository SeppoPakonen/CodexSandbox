// iChannel0 = Buffer A (this)
// iChannel3 = Keyboard

/*
Buffer A
update memories values and game logic
*/

#define F_PRESSED             ( texelFetch(iChannel3,ivec2(70,0),0).x )
#define R_PRESSED             ( texelFetch(iChannel3,ivec2(82,0),0).x )
#define SPACE_PRESSED         ( texelFetch(iChannel3,ivec2(32,0),0).x )
#define SHIFT_PRESSED         ( texelFetch(iChannel3,ivec2(16,0),0).x )

const int numOfTiles = 5;
void generateTerrain( inout vec4 fragColor, ivec2 iFragCoord )
{
    bool tileGenerated = texelFetch(MEMORY_CHANNEL, tilesSeted, 0).x != 1.0;
    if(tileGenerated)
    {
        if(iFragCoord.x == 0 && tilesAdress.y == iFragCoord.y)
        {
            //Tile(vec3(  0.0,   0.0,   0.0), PLANE     , 7.1, GRASS_COLOR, rotY_0),
            fragColor.xyz = vec3(  0.0,   0.0,   0.0);
        }
        if(tilesAdress.y == iFragCoord.y)
        {
            fragColor = vec4(0.5,0.5,0.5,1.0);
        }
    }
}

bool isPointInTriangle(const vec3 P, const vec3 A, const vec3 B, const vec3 C)
{
    vec3 v0 = C - A;
    vec3 v1 = B - A;
    vec3 v2 = P - A;
    
    float dot00 = dot(v0,v0);
    float dot01 = dot(v0,v1);
    float dot02 = dot(v0,v2);
    float dot11 = dot(v1,v1);
    float dot12 = dot(v1,v2);

    float denom = dot00 * dot11 - dot01 * dot01;
    float u = (dot11 * dot02 - dot01 * dot12) / denom;
    float v = (dot00 * dot12 - dot01 * dot02) / denom;

    return (u >= 0.) && (v >= 0.) && (u + v <= 1.);
}

int detectAndResolveCollisionWithTriangle(inout vec3 sphereCenter, const float sphereRadius, inout vec3 sphereVelocity, const vec3 A, const vec3 B, const vec3 C)
{
    const float e = 0.5; // elasticity
    vec3 S = sphereCenter;
    vec3 V = cross(B-A, C-A);
    vec3 N = V / sqrt(dot(V,V));
    float d = -(N.x * A.x + N.y * A.y + N.z * A.z);
    d = - dot(N,A);
    float ds = (N.x * S.x + N.y * S.y + N.z * S.z + d) / (square(N.x) + square(N.y) + square(N.z));;
    vec3 T = -ds * (N/length(N));
    vec3 P_proj = S + T;

    if(length(P_proj-S) > BALL_SIZE)
        return 0;
    
    if (isPointInTriangle(P_proj, A, B, C))  // there is a collision
    {
        N = N*(-sign(dot(P_proj-S, N))); // reverse the normal if the collision is coming from the back, therefore you can collide from both sides of a triangle
        
        sphereCenter = P_proj + N * BALL_SIZE; // offset the ball position
        
        {   // change the sphere velocity
            vec3 vn = dot(sphereVelocity, N) * N;
            vec3 vt = sphereVelocity - vn;
            vec3 vnPrime = -e * vn;
            vec3 vPrime = vnPrime + vt;
            sphereVelocity = vPrime;
        }
        
        // avoid the infinite bouncing problem
        if(P_proj.y < S.y)
            sphereVelocity.y = step( G * iTimeDelta * 5., sphereVelocity.y) * sphereVelocity.y;
        return 1;
    }
    return 0;
}

void detectAndResolveCollision(inout vec3 pos, inout vec3 speed)
{
    #if 1 - UPDATE_PHYSICS
        return;
    #endif
    for(int i = 0 ; i < tiles.length() ; i++)
    {
        int condition = (length(pos.xyz-tiles[i].position) - 7.1+BALL_SIZE > 0. ? 0 : 1);
        for (int j = tiles[i].type.x; j < tiles[i].type.y * condition;j++) 
        {
            vec3 v0 = verticesTiles[trianglesTile[j].x]*tiles[i].rotationMatrix + tiles[i].position;
            vec3 v1 = verticesTiles[trianglesTile[j].y]*tiles[i].rotationMatrix + tiles[i].position;
            vec3 v2 = verticesTiles[trianglesTile[j].z]*tiles[i].rotationMatrix + tiles[i].position;
            detectAndResolveCollisionWithTriangle(pos.xyz, BALL_SIZE, speed.xyz, v0,v1,v2);
        }
    }
} 

void firstFrame( inout vec4 fragColor, vec2 fragCoord )
{
    ivec2 iFragCoord = ivec2(fragCoord);
    fragColor = vec4(0.0); // ensure everything is at 0.0 for firstframe
    if(iFragCoord == charPosAdress)
    {
        fragColor = vec4(BALL_INITIAL_POS,0.0);
    }
    if(iFragCoord == camAnglesAdress)
    {
        fragColor = vec4(0.,0.4,0.,0.0);
    }
}

void updateGameState(inout vec4 fragColor, inout ivec2 iFragCoord)
{
    if(iFragCoord != gameStateAdress)
    {
        return;
    }
    
    // r : ; g : ; b : SPACE was pressed ; a : time spent with SPACE press
    vec4 gameState = texelFetch(MEMORY_CHANNEL, gameStateAdress, 0);
    
    // if SPACE is pressed then it loads the shot
    fragColor.b = SPACE_PRESSED;
    fragColor.a = gameState.a - sign(SHIFT_PRESSED*2.0-1.) * iTimeDelta; // if SHIFT is pressed then the speed is decreased
    fragColor.a = min(TIME_TO_MAX_SPEED, fragColor.a);
    fragColor.a = max(0., fragColor.a) * SPACE_PRESSED;
        
    // if F is pressed then it shoots the ball at max speed
    fragColor.b = fragColor.b + F_PRESSED;
    fragColor.a = fragColor.a + F_PRESSED * TIME_TO_MAX_SPEED;
}

void updateSphereRotation(inout vec4 fragColor, inout ivec2 iFragCoord)
{
    if(iFragCoord != sphereRotationAdress)
    {
        return;
    }
    vec4 currentSpeed = texelFetch(MEMORY_CHANNEL, charSpeedAdress, 0);
    fragColor.x = texelFetch(MEMORY_CHANNEL, sphereRotationAdress, 0).x;
    vec2 dir = normalize(currentSpeed.xz);
    float speed = length(currentSpeed.xz);
    float angleX = fragColor.x + (speed/BALL_SIZE)/2. * iTimeDelta;
    float angleY = atan(dir.y, dir.x);
    if(speed > epsilon)
    {
        fragColor.x = angleX;
        fragColor.y = angleY;
    }

}

void updateSpherePosition(inout vec4 fragColor, inout ivec2 iFragCoord)
{
    if(iFragCoord != charPosAdress)
    {
        return;
    }

    vec4 currentPos = texelFetch(MEMORY_CHANNEL, charPosAdress, 0);
    vec4 currentSpeed = texelFetch(MEMORY_CHANNEL, charSpeedAdress, 0);
    
    detectAndResolveCollision(currentPos.xyz, currentSpeed.xyz);
    vec4 nextPos = currentPos + currentSpeed * iTimeDelta;
    
    fragColor = nextPos;

    vec4 savedPos = texelFetch(MEMORY_CHANNEL, savedPosAdress, 0);
    fragColor = fragColor + (-fragColor+savedPos+vec4(0.,0.1,0.,0.)) * step(currentPos.y, SEA_LEVEL - BALL_SIZE*2.); // if the ball gets under the sea the the position is set to the last checkpoint position
    fragColor = fragColor + (-fragColor+vec4(BALL_INITIAL_POS,0.)) * R_PRESSED;  // if R is pressed then the game restart          
}

void updateSphereSpeed(inout vec4 fragColor, inout ivec2 iFragCoord)
{
    if(iFragCoord != charSpeedAdress)
    {
        return;
    }

    vec4 currentPos = texelFetch(MEMORY_CHANNEL, charPosAdress, 0);
    vec4 currentSpeed = texelFetch(MEMORY_CHANNEL, charSpeedAdress, 0);
    vec4 currentAccel = texelFetch(MEMORY_CHANNEL, charAccelerationAdress, 0);

    vec3 camForward = texelFetch(MEMORY_CHANNEL, charDirectionAdress, 0).xyz;
    vec4 nextSpeed = currentSpeed;

    vec4 gameState = texelFetch(MEMORY_CHANNEL, gameStateAdress, 0);
    if(SPACE_PRESSED==0. && gameState.b == 1.)
    {
        nextSpeed.xz = 
        normalize(camForward.xz) * 
        min(MAX_SPEED,mix(0., MAX_SPEED, gameState.a / TIME_TO_MAX_SPEED));
    }

    nextSpeed = nextSpeed + currentAccel * iTimeDelta;

    detectAndResolveCollision(currentPos.xyz, nextSpeed.xyz);


    // reset speed if it touches the water or if you press R
    nextSpeed = nextSpeed * step(SEA_LEVEL - BALL_SIZE*2., currentPos.y);
    nextSpeed = nextSpeed * step(R_PRESSED, 0.);

    fragColor = nextSpeed;

}

void updateSphereAcceleration(inout vec4 fragColor, inout ivec2 iFragCoord)
{
    #if 1-UPDATE_PHYSICS
        fragColor = vec4(0.);
        return;
    #endif 
    if(iFragCoord != charAccelerationAdress)
    {
        return;
    }
    vec4 currentSpeed = texelFetch(MEMORY_CHANNEL, charSpeedAdress, 0);
    const float surfaceCoef = 0.07; 
    vec4 surfaceFriction = step(0., length(currentSpeed)) * - currentSpeed *  ( BALL_MASS * G /* TODO add * cos(teta) relativ to horizon*/ ) * surfaceCoef;
    fragColor = surfaceFriction + vec4(0.,-G,0., 0.);
}

void updateDividing(inout vec4 fragColor, inout ivec2 iFragCoord)
{
    if(iFragCoord != dividingAdress)
    {
        return;
    }
    vec4 currentDiv = texelFetch(MEMORY_CHANNEL, dividingAdress, 0);            
    fragColor.r =  currentDiv.r;
    fragColor.r = floor(iResolution.x / min(iResolution.x, MAX_RESOLUTION));
}

void updateMouseLastPos(inout vec4 fragColor, inout ivec2 iFragCoord)
{
    if(iFragCoord != mouseLastPosAdress)
    {
        return;
    }
    
    // save mouse last position
    fragColor.xy = iMouse.xy;
    fragColor.z = step(0.0, iMouse.z);
}

void updateCamAngles(inout vec4 fragColor, inout ivec2 iFragCoord)
{
    bool mouseWasClicked = texelFetch(MEMORY_CHANNEL, mouseLastPosAdress, 0).z > 0.0;
    if(!(iFragCoord == camAnglesAdress && mouseWasClicked))
    {
        return;
    }
    vec2 mouseLastPos = texelFetch(MEMORY_CHANNEL, mouseLastPosAdress, 0).xy;

    float xoffset = (iMouse.x - mouseLastPos.x) / iResolution.x;
    float yoffset = (mouseLastPos.y - iMouse.y) / iResolution.y;
    xoffset *= MOUSE_SENSITIVITY;
    yoffset *= MOUSE_SENSITIVITY;
    fragColor.x += 
    #if INVERSE_X_AXE
    -
    #endif
    xoffset;
    fragColor.y += 
    #if INVERSE_Y_AXE
    -
    #endif
    yoffset;

    fragColor.y = clamp(fragColor.y, 0., 1.553340);
}

void updateDirection(inout vec4 fragColor, inout ivec2 iFragCoord)
{
    if(iFragCoord != charDirectionAdress)
    {
        return;
    }
    vec3 currentPos = texelFetch(MEMORY_CHANNEL, charPosAdress, 0).xyz;
    vec3 cameraPos = texelFetch(MEMORY_CHANNEL, camPosAdress, 0).xyz;
    fragColor.xyz =  normalize(currentPos - cameraPos);
}

void updateCamPos(inout vec4 fragColor, inout ivec2 iFragCoord)
{
    if(iFragCoord != camPosAdress)
    {
        return;
    }
    vec3 charPos = texelFetch(MEMORY_CHANNEL, charPosAdress, 0).xyz;
    vec4 camAngles = texelFetch(MEMORY_CHANNEL, camAnglesAdress, 0);
    vec3 cameraPos = charPos 
        + vec3(0.,1.5,0.) 
        + vec3(cos(camAngles.x)*cos(camAngles.y),sin(camAngles.y),sin(camAngles.x)*cos(camAngles.y))
        *(10.);

    fragColor.xyz = cameraPos;
}

void updateSavedPos(inout vec4 fragColor, inout ivec2 iFragCoord)
{
    if(iFragCoord != savedPosAdress)
    {
        return;
    }
    vec4 currentPos = texelFetch(MEMORY_CHANNEL, charPosAdress, 0);
    vec4 currentSpeed = texelFetch(MEMORY_CHANNEL, charSpeedAdress, 0);
    if(length(currentSpeed.xz) < 2.0 && length(currentSpeed.y) <= epsilon)
    {
        fragColor = currentPos;
    }
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    if(iFrame == 0)
    {
        firstFrame( fragColor, fragCoord );
        return;
    }
    
    ivec2 iFragCoord = ivec2(fragCoord);
    fragColor = texelFetch(MEMORY_CHANNEL, iFragCoord, 0);// default -> nextValue = currentValue

    generateTerrain          (fragColor, iFragCoord);
    updateGameState          (fragColor, iFragCoord);
    updateSphereRotation     (fragColor, iFragCoord);
    updateSpherePosition     (fragColor, iFragCoord);
    updateSphereSpeed        (fragColor, iFragCoord);
    updateSphereAcceleration (fragColor, iFragCoord);
    updateDividing           (fragColor, iFragCoord);
    updateMouseLastPos       (fragColor, iFragCoord);
    updateCamAngles          (fragColor, iFragCoord);
    updateDirection          (fragColor, iFragCoord);
    updateCamPos             (fragColor, iFragCoord);
    updateSavedPos           (fragColor, iFragCoord);
}
