#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform vec2 iResolution;
uniform vec2 iMouse;
uniform int iFrame;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;  // Keyboard state

// Define constants from common.glsl
#define VERTEX_MODE 65
#define EDGE_MODE 83
#define PIN_MODE 68
#define RESET 82
#define WAIT 30
#define STIFF 100.0
#define editMode (texelFetch(iChannel1, ivec2(32,2), 0).x <= 0.0)

float line(vec2 p, vec2 a, vec2 b) {
    vec2 pa = p - a, ba = b - a;
    float h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0);
    return length(pa - ba * h);
}

void main() {
    vec2 R = iResolution;
    vec2 U = gl_FragCoord.xy;
    vec2 Mouse = iMouse;
    
    U = U / R.y - R.xy / R.y / 2.0;
    int N = int(texelFetch(iChannel0, ivec2(0, 0), 0).x);
    int M = int(texelFetch(iChannel0, ivec2(0, 1), 0).x);

    // Background
    vec4 O = vec4(0, 0.5, 0.8, 1.0) * (editMode ? 1.0 + U.y * 0.8 : 0.9 + 0.1 * smoothstep(0.0, 0.001, sin(U.x * 62.8) * sin(U.y * 62.8)));
    
    // Add ground with texture effect
    vec4 ground_tex = texture(iChannel2, U * 2.0);
    O = mix(O, vec4(0.0, 0.8, 0.0, 1.0) - ground_tex, smoothstep(-0.001, 0.001, min(-U.y, -U.y * 0.1 + abs(U.x) - 0.498)));
    
    // Water effect
    O = mix(O, vec4(0.0, 0.2, 1.0, 1.0) * (0.7 + 0.03 * sin(U.y * 500.0 + sin(U.x * 50.0) * 5.0)), smoothstep(-0.001, 0.001, min(-0.35 - U.y - sin(U.x * 50.0) * 0.01, 0.5 + U.y * 0.1 - abs(U.x))));
    
    // More ground
    vec4 ground2_tex = texture(iChannel2, U * 3.0);
    O = mix(O, vec4(0.0, 0.8, 0.0, 1.0) - ground2_tex, smoothstep(-0.001, 0.001, min(-U.y - 0.3, -U.y * 0.1 - abs(U.x) + 0.02)));

    // Segments
    for(int i = 1; i <= M; i++){
        ivec2 l = ivec2(texelFetch(iChannel0, ivec2(i, 1), 0).xy);
        vec4 s = vec4(
            texelFetch(iChannel0, ivec2(l.x, 2), 0).xy,  // First node position
            texelFetch(iChannel0, ivec2(l.y, 2), 0).xy   // Second node position
        );
        float dm = line(Mouse, s.xy, s.zw);
        float d = line(U, s.xy, s.zw);
        vec2 seg_a = texelFetch(iChannel0, ivec2(l.x, 0), 0).xy;
        vec2 seg_b = texelFetch(iChannel0, ivec2(l.y, 0), 0).xy;
        float t = 1.0 - length(s.xy - s.zw) / length(seg_a - seg_b);
        O = mix(O, editMode ? vec4(1, 1, 0, 1) : mix(vec4(0, 1, 0, 1), vec4(10.0, 0, 0, 1), abs(t) * 2000.0 / STIFF), 
                smoothstep(0.001, -0.001, d - 0.005));
    }

    // Nodes
    if(editMode) {
        for(int i = 1; i <= N; i++){
            vec4 p = texelFetch(iChannel0, ivec2(i, 2), 0);
            vec3 node_data = texelFetch(iChannel0, ivec2(i, 0), 0).xyz;
            O = mix(O, node_data.z > 0.0 ? vec4(0, 0, 0, 1) : vec4(1, 0, 0, 1), smoothstep(0.001, -0.001, length(p.xy - U) - 0.01));
        }
    }
    
    FragColor = O;
}