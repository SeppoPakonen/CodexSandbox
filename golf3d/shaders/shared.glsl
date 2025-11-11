// ----------- shared -----------

const float ballRad = .025;
const float bounce = .5; // [0,1)
const float collisionThreshold = .001;
const float holeRad = .08;
    
vec3 ballPos, holePos;

// SDF for dynamic objects
float BallSDF( vec3 pos )
{
    return length(pos-ballPos)-ballRad;
}

uvec4 Hash( uint seed ) 
{
    // integer hash from Hugo Elias
	seed = (seed << 13U) ^ seed;
    seed = seed * (seed * seed * 15731U + 789221U) + 1376312589U;
    return seed * uvec4(seed,seed*16807U,seed*48271U,seed*31713U);
}

vec3 vrand( uint seed )
{
    return vec3(Hash(seed).xyz&0x7fffffffU)/float(0x7fffffffU);
}

float HoleSDF( vec3 pos )
{
    vec3 dhole = pos-holePos;
    dhole.y = min(dhole.y,0.); // sphere bottom with cylinder above
    return holeRad-length(dhole);
}


float GetNoise( vec3 pos )
{
    float noise = (sin(pos.x/3.)+sin(pos.z/3.))*3./2.;
    float noise2 = (sin(pos.z/6.283)+sin((-pos.x*sqrt(.75)+pos.z*sqrt(.25))/6.283))/2.;
    float noise3 = (sin(pos.x/28.)+sin((pos.z*sqrt(.75)-pos.x*sqrt(.25))/28.))/2.;
    float noise4 = (sin(pos.x/100.+sin((pos.x+pos.z)/61.8))+sin(pos.z/100.+sin((pos.x-pos.z)/77.7))+2.)/4.;
    
    return mix(noise*(.5-noise2),noise2*6.283 + noise3*28.,.5)
        *(1.-pow(noise4,2.)); // flatter spots
}

// scenery
float SDF( vec3 pos, bool includeDynamic )
{
    const float maxgrad = 1.; // steepest gradient the noise can output (not sure this is still true)

    // flatten a green around the hole
    float noise = GetNoise(pos);
    noise = mix( noise, GetNoise(holePos), pow( smoothstep(8.,2.,length(pos-holePos) ), 2. ) );
    
    float f = (pos.y + noise)/sqrt(1.+maxgrad*maxgrad); // normalize so 3D gradient is <= 1.0 at all points
    
    if ( includeDynamic )
    {
        f = min(f,BallSDF(pos));
    }

    // add hole to the SDF:
    f = max(f,HoleSDF(pos));
    
    return f;
}

vec3 GetNormal( vec3 pos, float eps, bool i )
{
    vec2 d = vec2(-1,1)*eps;
    return normalize(
	    				SDF(pos+d.xxx,i)/d.xxx +
	    				SDF(pos+d.xyy,i)/d.xyy +
	    				SDF(pos+d.yxy,i)/d.yxy +
	    				SDF(pos+d.yyx,i)/d.yyx
            );
}

// ----------- end of shared -----------