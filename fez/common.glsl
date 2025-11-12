// rotation matrix thing
void rotate(inout vec2 p, float a) {
    p = cos(a)*p + sin(a)*vec2(p.y, -p.x);
}

const float PI = acos(0.0) * 2.0;
const int MAX_STEPS = 48;

struct Tile {
    int ID;
};

Tile world(vec3 p) {
    p = floor(p) + 0.5;
    
    if (abs(p.y + 6.0) < 1.0 && abs(p.x - 1.0) < 2.0 && abs(p.z - 4.0) < 5.0) return Tile(1);
    
    if (abs(p.y + 4.0) < 2.0 && abs(p.x - 3.0) < 2.0 && abs(p.z - 6.0) < 2.0) return Tile(1);
    
    if (abs(p.y - 4.0) < 1.0 && abs(p.x + 4.5) < 1.5 && abs(p.z - 5.0) < 1.0) return Tile(1);
    
    if (abs(p.y - 0.5) < 6.5 && abs(p.x - 5.0) < 2.0 && abs(p.z + 1.5) < 4.0) return Tile(1);
            
    if (abs(p.y - 4.0) < 1.0 && abs(p.x - 4.0) < 1.0 && abs(p.z + 3.0) < 2.0) return Tile(1);

    if (abs(p.y + 1.0) < 1.0 && abs(p.x - 7.0) < 2.0 && abs(p.z + 4.0) < 1.0) return Tile(1);
    
    if (length(p.xyz + vec3(8,0,9)) < 2.5) return Tile(1);
    
    return Tile(0);
}

struct HitInfo {
    Tile tile;
    vec3 pos;
    vec3 normal;
};

HitInfo castRay(vec3 rayPos, vec3 rayDir) {
    Tile tile;
    rayPos *= 0.0625;

    // this entire function was stolen from https://www.shadertoy.com/view/4dX3zl
    // not too sure how it works, but it's much faster than what i had before
	vec3 mapPos = floor(rayPos);
	vec3 deltaDist = abs(vec3(length(rayDir)) / rayDir);
	vec3 rayStep = sign(rayDir);
	vec3 sideDist = (sign(rayDir) * (mapPos - rayPos) + (sign(rayDir) * 0.5) + 0.5) * deltaDist; 
	vec3 mask;
	
	for (int i = 0; i < MAX_STEPS; i++) {
        tile = world(mapPos);
		if (tile.ID != 0) break;
		mask = step(sideDist.xyz, sideDist.yzx) * step(sideDist.xyz, sideDist.zxy) + 0.000000001; // for some reason some gpus won't render anything if this isn't here, spooky
		sideDist += mask * deltaDist;
		mapPos += mask * rayStep;
	}
    
    rayPos = rayDir / dot(mask * rayDir, vec3(1.0)) * dot(mask * (mapPos + vec3(lessThan(rayDir, vec3(0.0))) - rayPos), vec3(1.0)) + rayPos;
    rayPos = rayDir / dot(mask * rayDir, vec3(1.0)) * dot(mask * (mapPos + vec3(lessThan(rayDir, vec3(0.0))) - rayPos), vec3(1.0)) + rayPos;
    
    return HitInfo(tile, rayPos, mask);
}


// the only way i know how to import custom textures is to hard code them
// this will definitely change in future versions
vec3 blockTexture(vec2 p) {
    vec3[] palette = vec3[](vec3(0.5, 0.25, 0.25), vec3(0.8, 0.5, 0.25), vec3(1,0.75,0.15));
    int[] pixels = int[](
        1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
        0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
        0,2,1,1,1,1,1,1,1,1,1,1,1,1,0,2,
        0,2,1,1,2,2,2,2,2,2,2,2,2,1,0,2,
        0,2,1,0,1,1,1,1,1,1,1,1,2,1,0,2,
        0,2,1,0,1,1,1,1,1,1,1,1,2,1,0,2,
        0,2,1,0,1,1,1,1,1,1,1,1,2,1,0,2,
        0,2,1,0,1,1,1,1,1,1,1,1,2,1,0,2,
        0,2,1,0,1,1,1,1,1,1,1,1,2,1,0,2,
        0,2,1,0,1,1,1,1,1,1,1,1,2,1,0,2,
        0,2,1,0,1,1,1,1,1,1,1,1,2,1,0,2,
        0,2,1,0,1,1,1,1,1,1,1,1,2,1,0,2,
        0,2,1,0,0,0,0,0,0,0,0,0,1,1,0,2,
        0,2,1,1,1,1,1,1,1,1,1,1,1,1,0,2,
        0,2,2,2,2,2,2,2,2,2,2,2,2,2,1,2,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1
    );
    return palette[pixels[int(mod(floor(p.x), 16.0) + mod(floor(16.0-p.y), 16.0) * 16.0)]];
}

vec4 grassTexture(vec2 p) {
    vec4[] palette = vec4[](vec4(0), vec4(0.8, 1.0, 0.0, 1), vec4(0.4, 0.8, 0.0, 1), vec4(0.3, 0.7, 0.0, 1), vec4(0.5, 0.25, 0.25, 1));
    int[] pixels = int[](
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        2,1,1,1,2,2,2,2,1,1,2,2,2,2,2,2,
        2,2,2,2,2,3,3,2,2,2,2,3,3,3,3,3,
        3,3,3,3,3,4,4,3,3,3,3,4,4,4,4,4,
        4,4,4,4,4,0,0,4,4,4,4,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    );
    return palette[pixels[int(mod(floor(p.x), 16.0) + mod(floor(16.0-p.y), 16.0) * 16.0)]];
}

vec4 playerTexture(vec2 p) {
    vec4[] palette = vec4[](vec4(0), vec4(1), vec4(0.8,0.8,0.7,1), vec4(0.7,0.7,0.5,1), vec4(0, 0, 0, 1));
    int[] pixels = int[](
        0,3,2,1,1,1,1,1,1,1,1,1,0,0,0,0,
        3,2,2,1,1,1,1,1,1,1,1,1,1,0,0,0,
        3,2,1,1,1,1,1,1,1,1,1,1,1,0,0,0,
        3,2,1,4,1,1,1,1,1,1,1,4,1,0,0,0,
        3,2,1,1,1,1,1,1,1,1,1,1,1,0,0,0,
        3,2,2,1,1,1,1,1,1,1,1,1,1,0,0,0,
        0,3,3,2,1,1,1,1,1,1,1,1,0,0,0,0,
        0,0,0,0,3,3,2,1,0,0,0,0,0,0,0,0,
        0,0,0,3,2,1,1,1,1,0,0,0,0,0,0,0,
        0,0,3,1,1,1,1,1,1,0,0,0,0,0,0,0,
        0,3,1,3,1,1,1,1,1,3,0,0,0,0,0,0,
        0,0,0,3,2,1,1,1,1,0,0,0,0,0,0,0,
        0,0,0,3,2,1,1,1,1,0,0,0,0,0,0,0,
        0,0,0,3,2,2,1,1,1,0,0,0,0,0,0,0,
        0,0,0,3,2,0,0,3,2,0,0,0,0,0,0,0,
        0,0,0,3,0,0,0,0,3,0,0,0,0,0,0,0
    );
    return palette[pixels[int(mod(floor(p.x), 16.0) + mod(floor(16.0-p.y), 16.0) * 16.0)]];
}

