#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform vec3 iResolution;
uniform float iTime;
uniform vec4 iMouse;
uniform int iFrame;
uniform sampler2D iChannel0;
uniform sampler2D iChannel1;
uniform sampler2D iChannel2;
uniform sampler2D iChannel3;

// Shared constants and functions from the Shadertoy code
const float ballRad = .025;
const float bounce = .5;
const float collisionThreshold = .001;
const float holeRad = .08;

vec3 vrand(uint seed) {
    uvec4 Hash = seed * uvec4(seed, seed * 16807U, seed * 48271U, seed * 31713U);
    return vec3(Hash.xyz & 0x7fffffffU) / float(0x7fffffffU);
}

float GetNoise(vec3 pos) {
    float noise = (sin(pos.x / 3.) + sin(pos.z / 3.)) * 3. / 2.;
    float noise2 = (sin(pos.z / 6.283) + sin((-pos.x * sqrt(.75) + pos.z * sqrt(.25)) / 6.283)) / 2.;
    float noise3 = (sin(pos.x / 28.) + sin((pos.z * sqrt(.75) - pos.x * sqrt(.25)) / 28.)) / 2.;
    float noise4 = (sin(pos.x / 100. + sin((pos.x + pos.z) / 61.8)) + sin(pos.z / 100. + sin((pos.x - pos.z) / 77.7)) + 2.) / 4.;
    
    return mix(noise * (.5 - noise2), noise2 * 6.283 + noise3 * 28., .5)
        * (1. - pow(noise4, 2.));
}

float HoleSDF(vec3 pos) {
    vec3 dhole = pos - vec3(0, 0, 10); // Placeholder hole position
    dhole.y = min(dhole.y, 0.);
    return holeRad - length(dhole);
}

float SDF(vec3 pos, bool includeDynamic) {
    const float maxgrad = 1.;
    
    float noise = GetNoise(pos);
    vec3 holePos = vec3(0, 0, 10); // Placeholder hole position
    noise = mix(noise, GetNoise(holePos), pow(smoothstep(8., 2., length(pos - holePos)), 2.));
    
    float f = (pos.y + noise) / sqrt(1. + maxgrad * maxgrad);
    
    if (includeDynamic) {
        // Ball SDF would be here
    }

    f = max(f, HoleSDF(pos));
    
    return f;
}

vec3 GetNormal(vec3 pos, float eps, bool i) {
    vec2 d = vec2(-1, 1) * eps;
    return normalize(
        SDF(pos + d.xxx, i) / d.xxx +
        SDF(pos + d.xyy, i) / d.xyy +
        SDF(pos + d.yxy, i) / d.yxy +
        SDF(pos + d.yyx, i) / d.yyx
    );
}

void mainImage(out vec4 fragColor, in vec2 fragCoord) {
    vec2 uv = fragCoord / iResolution.xy;
    vec3 ray;
    ray.xy = (fragCoord - iResolution.xy * .5) / iResolution.y;
    ray.z = 1.;
    ray = normalize(ray);
    
    vec3 camPos = vec3(0, 1, -3);
    vec3 ballPos = vec3(0, 0.1, 0); // Initial ball position
    vec3 camk = normalize(normalize(ballPos - camPos) + vec3(0, .2, 0));
    vec3 cami = normalize(cross(vec3(0, 1, 0), camk));
    vec3 camj = cross(camk, cami);
    
    ray = ray.x * cami + ray.y * camj + ray.z * camk;
    
    vec3 pos = camPos;
    
    float epsilon = .001;
    float h = 1.;
    for (int i = 0; i < 200; i++) {
        h = SDF(pos, true);
        if (h < epsilon)
            break;
        pos += h * ray;
    }
    
    if (h < 7.) {
        vec3 n = GetNormal(pos, .001, true);
        
        vec3 sunCol = vec3(.8);
        vec3 sunDir = normalize(vec3(3, 2, 1));
        
        float l = max(.0, dot(n, sunDir));
        
        // Measure occlusion
        float occD = max(h, .02);
        vec3 occPos = pos + n * occD;
        float vis = (SDF(occPos, true) - h) / occD;
        
        float occD2 = max(h, 1.);
        vec3 occPos2 = pos + n * occD2;
        float vis2 = (SDF(occPos2, true) - h) / occD2;
        if (vis2 < vis) {
            vis = vis2;
            occD = occD2;
            occPos = occPos2;
        }

        vec3 occNorm = GetNormal(occPos, occD, true);
        vec3 occLight = sunCol * mix(.2, 1., smoothstep(-.1, 1., dot(occNorm, sunDir)));
        vec3 occAlbedo = vec3(0, 1, 0);
        vec3 ao = mix(occAlbedo * occLight * .3, vec3(.3, .5, 1) * .3, vis);

        vec3 light = ao + sunCol * l;
        
        float v = (sin(pos.x * 80.) + sin(pos.z * 80.)) / 2.;
        vec3 albedo = vec3(0, 1. + .02 * v, 0);
        if (HoleSDF(pos) > -epsilon) albedo = vec3(.05);
        
        fragColor = vec4(albedo * light, 1.0);

        // Fog
        float fog = exp2(-length(pos - camPos) / 500.);
        vec3 skyCol = exp2(-max(.0, ray.y) / vec3(.1, .3, .6));
        fragColor.rgb = mix(skyCol, fragColor.rgb, fog);
    } else {
        vec3 skyCol = exp2(-max(.0, ray.y) / vec3(.1, .3, .6));
        fragColor = vec4(skyCol, 1.0);
    }
}

void main() {
    mainImage(FragColor, TexCoord * iResolution.xy);
}