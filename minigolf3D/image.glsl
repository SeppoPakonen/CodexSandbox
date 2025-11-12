// iChannel0 = Buffer A
// iChannel1 = Buffer B

/*

CONTROLS :

    - CLICK            : move camera
    - SPACE            : load shot
    - SPACE + shift    : unload shot
    - R                : to restart

you can change values in defines in the "Common" tab

*/


/*
Image
uses bufferB to display the scene and the UI on top
*/

// Fonction principale pour le raytracing
void mainImage(out vec4 fragColor, in vec2 fragCoord) {
    vec4 currentDiv = texelFetch(MEMORY_CHANNEL, dividingAdress, 0);

    float dividing = currentDiv.r;
    fragCoord.x = fragCoord.x / dividing;
    fragCoord.y = fragCoord.y / dividing;
    fragColor = texelFetch(iChannel1, ivec2(fragCoord), 0); 
    
    vec2 uv = ((fragCoord+ vec2(0.)) - iResolution.xy / dividing * 0.5) / (iResolution.y / dividing);
    float r = length(uv);
    float theta = atan(-uv.x, uv.y);
    vec4 gameState = texelFetch(MEMORY_CHANNEL, gameStateAdress, 0);
    float progress = gameState.a / TIME_TO_MAX_SPEED * 6.283185;
    if (theta < 0.0) theta += 6.283185;
    float radius = (BALL_SIZE * 0.05 + 0.01) / 0.6 + 0.015;
    float mask = smoothstep(radius, radius - 0.01, r) * step(theta, progress);
    radius = radius-0.01*progress*0.15;
    float maskIn = smoothstep(radius/1.1, (radius - 0.01) /1.1, r) * step(theta, progress);
    vec3 color = (1.0-step(0.00001,mask-maskIn))*fragColor.rgb + vec3(step(0.00001,mask-maskIn)* mix(vec3(0.,1.,0.),vec3(1.0, 0., 0.2),gameState.a / TIME_TO_MAX_SPEED));
    fragColor = vec4(color, 1.0);

    #if SHOW_MEMORY
    {
        vec4 memValue = texelFetch(MEMORY_CHANNEL, ivec2(fragCoord), 0);
        fragColor = length(memValue) > 0.0 ? memValue : fragColor ;
    }
    #endif

}

