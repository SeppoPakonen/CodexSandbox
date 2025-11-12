#version 330 core

// Fragment shader for modern renderer
// Based on the Shadertoy soccer2d implementation
out vec4 FragColor;

in vec2 TexCoord;

uniform float iTime;
uniform vec2 iResolution;
uniform vec4 iMouse;
uniform sampler2D iChannel0; // Buffer A
uniform sampler2D iChannel1; // Buffer B
uniform sampler2D iChannel2; // Font texture
uniform sampler2D iChannel3; // Keyboard texture

#define PC vec2(0.95, 0.52) // pitch size
#define tk 0.006 // line thickness

// Include the required functions from the Shadertoy code
float sdBox(vec2 p, vec2 b)
{
    vec2 d = abs(p) - b;
    return length(max(d, 0.0)) + min(max(d.x, d.y), 0.0);
}

float sdSegment(in vec2 p, in vec2 a, in vec2 b)
{
    vec2 pa = p - a, ba = b - a;
    float h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0);
    return length(pa - ba * h);
}

// From Shadertoy: https://www.shadertoy.com/view/3tyfzV
void drawChar(sampler2D ch, inout vec3 color, in vec3 charColor, in vec2 p, in vec2 pos, in vec2 size, in int char) {
    p = (p - pos) / size + 0.5;
    if (all(lessThan(abs(p - 0.5), vec2(0.5)))) {
        float val = texture(ch, p / 16.0 + fract(vec2(char, 15 - char / 16) / 16.0)).r;
        color = mix(color, charColor, val);
    }
}

#define coord(p) (p * min(iResolution.y / PC.y / 2.0, iResolution.x / PC.x / 2.0) / 1.05 + iResolution.xy * 0.5)
#define position(U) ((U - iResolution.xy * 0.5) / min(iResolution.y / PC.y / 2.0, iResolution.x / PC.x / 2.0) * 1.05)

#define score(s,p,b,d) s * (b.x) * (0.03 + smoothstep(0.05, 0.2, abs(d.w - d.y)) * 2.0) * smoothstep(PC.y * 0.95, PC.y * 0.6, abs(p.y))

float pitch(vec2 uv)
{
    float d = 1e5;
    uv = abs(uv);
    d = min(d, abs(length(uv) - 0.18));
    d = min(d, abs(sdBox(uv - vec2(PC.x - 0.34, 0), vec2(0.17, 0.4))));
    d = min(d, abs(sdBox(uv - vec2(PC.x / 2.0, 0), vec2(PC.x / 2.0, PC.y))));
    d = min(d, abs(sdBox(uv - vec2(PC.x - 0.17, 0), vec2(0.17, 0.4))));
    d = min(d, abs(sdBox(uv - vec2(PC.x - 0.05, 0), vec2(0.05, 0.15))));
    d = min(d, length(uv - vec2(PC.x - 0.25, 0.0)));
    return d;
}

void main()
{
    vec2 U = TexCoord * iResolution.xy; // Convert normalized coordinates to screen space
    vec2 uv = position(U);
    vec4 ball = texelFetch(iChannel0, ivec2(0), 0);
    vec4 zBall = texelFetch(iChannel1, ivec2(int(coord(ball.xy).x), int(coord(ball.xy).y)), 0);

    // Render field
    vec3 col = vec3(0.2, 0.5, 0.1) * (0.9 + 0.1 * step(0.1, mod(uv.x, 0.2))) * (0.9 + 0.1 * step(0.1, mod(uv.y, 0.2)));
    col = mix(col, vec3(1), smoothstep(tk, 0.0, pitch(uv)));
    col = mix(col, vec3(0.7), smoothstep(tk, 0.0, sdBox(abs(uv) - vec2(PC.x + 0.02, 0), vec2(0.02, 0.07))));

    // Draw scores
    ivec4 mode = ivec4(texelFetch(iChannel0, ivec2(24, 0), 0));
    ivec4 sc = ivec4(texelFetch(iChannel0, ivec2(25, 0), 0));
    // drawChar(iChannel2, col, vec3(0.,0.7,0), uv, vec2(-0.05,0.45), vec2(0.1), 48 + sc.x);
    // drawChar(iChannel2, col, vec3(0.,0.7,0), uv, vec2(0.05,0.45), vec2(0.1), 48 + sc.y);

    // Render players and ball
    for(int i = 0; i <= 22; i++)
    {
        vec4 pl = texelFetch(iChannel0, ivec2(i, 0), 0);
        vec2 pd = (uv - pl.xy) * mat2(cos(pl.z), sin(pl.z), -sin(pl.z), cos(pl.z));

        float fl = abs(mod(pl.w * 0.5, 0.04) - 0.02);
        if(i > 0 && sdBox(uv - pl.xy, vec2(0.03)) < 0.0 ) {
            vec3 c = ((i % 11) == 1 ? vec3(0, 1, 0) : (i < 12 ? vec3(1, 0, 0) : vec3(0, 0.7, 1)));
            col = mix(col, vec3(0), smoothstep(tk, 0.0, sdBox(pd + vec2(-fl + 0.005, 0.01), vec2(0.005, 0.002))));
            col = mix(col, vec3(0), smoothstep(tk, 0.0, sdBox(pd + vec2(-0.015 + fl, -0.01), vec2(0.005, 0.002))));
            col = mix(col, c, smoothstep(tk, 0.0, sdBox(pd, vec2(0.006, 0.012))));
            col = mix(col, c, smoothstep(tk, 0.0, sdBox(pd + vec2(-0.006 + fl * 0.5, 0.014), vec2(0.005, 0.002))));
            col = mix(col, c, smoothstep(tk, 0.0, sdBox(pd + vec2(-fl * 0.5 + 0.002, -0.014), vec2(0.005, 0.002))));

            vec3 head = vec3(0.4, 0.2, 0.2);
            if(iMouse.z > 0.0 && zBall.x < 11.5 && int(zBall.x) == i) head = vec3(1, 1, 0);
            if(iMouse.z > 0.0 && zBall.z < 11.5 && int(zBall.z) == i) head = vec3(1, 1, 0);
            col = mix(col, head, smoothstep(tk, 0.0, -0.007 + length(uv - pl.xy)));
        }
        else if(i == 0 && length(uv - pl.xy) < 0.01) {
            // Render ball with a more detailed approach
            col = mix(col, vec3(1.0, 1.0, 0.0), 0.8); // Yellow ball
        }
    }

    // Draw mouse cursor if active
    if(iMouse.z > 0.0) {
        col = mix(col, vec3(1, 1, 0), smoothstep(0.001, 0.0, -0.005 * iResolution.y + length(U - iMouse.xy)));
    }

    FragColor = vec4(col, 1.0);
}