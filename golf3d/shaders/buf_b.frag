#version 330 core

// Buffer B: Ray Marching Renderer
// This shader renders the 3D scene using ray marching

uniform sampler2D iChannel0;  // Position and velocity data
uniform sampler2D iChannel1;  // Previous frame data for TAA
uniform vec3 iResolution;
uniform float iTime;
uniform int iFrame;
uniform vec4 iMouse;

// Shared constants and functions
const float ballRad = .025;
const float bounce = .5; // [0,1)
const float collisionThreshold = .001;
const float holeRad = .08;

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

out vec4 o_col;

void main()
{
    ivec2 fragCoord = ivec2(gl_FragCoord.xy);
    
    vec3 ballPos = texelFetch(iChannel0,ivec2(0),0).xyz;
    vec3 ballDPos = texelFetch(iChannel0,ivec2(1,0),0).xyz;
	vec4 dat = texelFetch(iChannel0,ivec2(2,0),0);
	vec2 mouseDragStart = dat.xy;
    vec2 mouseDrag = dat.zw;
    dat = texelFetch(iChannel0,ivec2(3,0),0);
    vec3 camPos = dat.xyz;
    float taa = dat.w;
    vec3 holePos = texelFetch(iChannel0,ivec2(4,0),0).xyz;

    bool stationary = (length(ballDPos) == 0.);
    
    vec2 jitter = vec2(0); // For simplicity in C conversion, we'll implement jittering in C code
    //vec2 jitter = vec2((uint(iFrame)*uvec2(0x61c992U,0x9e3853U))&0xffffffU)/float(0xffffffU+1U); // approximately 1/phi, 1/(phi*phi)
    
    // break up the jitter so it doesn't feel like camera jiggle
    //jitter = fract(jitter+fragCoord.yx/32.);// /32. no visible dither, /1.618 random dither, /2. checker dither
    
	vec3 ray;
    ray.xy = (jitter + vec2(fragCoord) - iResolution.xy*.5)/iResolution.y;
    ray.z = 1.;
    //ray.z -= dot(ray.xy,ray.xy)*2.5; // fisheye lens for lols
    ray = normalize(ray);
    
    // turn camera toward ball
    vec3 camk = normalize(normalize(ballPos-camPos)+vec3(0,.2,0));
    vec3 cami = normalize(cross(vec3(0,1,0),camk));
    vec3 camj = cross(camk,cami);
    
    ray = ray.x*cami + ray.y*camj + ray.z*camk;
    
    vec3 pos = camPos;
    
    float epsilon = .001;
    float h = 1.;
    for ( int i=0; i < 200; i++ )
    {
        h = SDF(pos,true);
        if ( h < epsilon )
            break;
        pos += h*ray;
    }
    
    o_col = vec4(0,0,0,0);
    if ( h < 7. ) // this can be (much) larger, to hide artefacts with best guess
    {
        vec3 n = GetNormal(pos,.001,true);
        
        vec3 sunCol = vec3(.8);
        vec3 sunDir = normalize(vec3(3,2,1));
        
        float l = max(.0,dot(n,sunDir));
        
        // shadow for the ball
        vec3 dball = ballPos-pos;
        float ds = dot(dball,sunDir);
        if ( ds > .0 )
        {
            float p = length(dball-sunDir*ds);
            float s = .01*length(dball);
            l *= 1.-pow(1.-clamp(.5+.5*(p-ballRad)/s,0.,1.),2.);
        }
        
        // measure how much occlusion there is here
        float occD = max(h,.02); // ball-scale
        vec3 occPos = pos + n * occD;
        float vis = (SDF(occPos,true)-h)/occD;
        
        float occD2 = max(h,1.); // scenery-scale
        vec3 occPos2 = pos + n * occD2;
        float vis2 = (SDF(occPos2,true)-h)/occD2;
        if ( vis2 < vis )
        {
            vis = vis2;
            occD = occD2;
            occPos = occPos2;
        }

        // approximate some radiance transfer
        // compute approximate light on the occluder
        vec3 occNorm = GetNormal(occPos,occD,true);
        vec3 occLight = sunCol*mix(.2,1.,smoothstep(-.1,1.,dot(occNorm,sunDir)));

        // find occluder's colour
        vec3 occAlbedo = vec3(0,1,0);//GetColour(occPos,softness);

        vec3 ao = mix( occAlbedo*occLight*.3, vec3(.3,.5,1)*.3, vis );


        vec3 light = ao+sunCol*l;
        
        float v = (sin(pos.x*80.)+sin(pos.z*80.))/2.;
//        vec3 albedo = vec3(max(0.,v)*v*.1,1,max(0.,-v)*(-v)*.1);
        vec3 albedo = vec3(0,1.+.02*v,0);
        if ( BallSDF(pos) < epsilon ) albedo = vec3(1);
        if ( HoleSDF(pos) > -epsilon ) albedo = vec3(.05);
        
        o_col.rgb = albedo*light;

//o_col.rgb = fract(vec3(1./16.,1./256.,1.)*length(camPos2-pos));
        // fog
        o_col.a = exp2(-length(pos-camPos)/500.);
    }
    
    // sky
    vec3 skyCol = exp2(-max(.0,ray.y)/vec3(.1,.3,.6));
    
    // composite sky
    o_col.rgb = mix( skyCol, o_col.rgb, o_col.a );

    // temporal anti-aliasing - in C implementation we'll handle this differently
    //o_col = mix ( o_col, texelFetch(iChannel1,ivec2(fragCoord),0), taa*.8 );
            
/*    vec3 pos = ballPos+vec3(4.*((fragCoord-.5*iResolution.xy)/iResolution.y),0);
	o_col = vec4(fract(pos),1);
    
    float s = SDFB(pos,ballPos);
    o_col.xyz *= .5+.5*s/(.01+abs(s));*/
}