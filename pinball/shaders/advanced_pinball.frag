#version 330 core

// Shadertoy uniforms
uniform vec3      iResolution;           // viewport resolution (in pixels)
uniform float     iTime;                 // shader playback time (in seconds)
uniform float     iTimeDelta;            // render time (in seconds)
uniform vec4      iMouse;                // mouse pixel coords. xy: current (if MLB down), zw: click
uniform vec4      iDate;                 // (year, month, day, time in seconds)
uniform float     iFrame;                // shader playback frame

// Textures
uniform sampler2D iChannel0;             // for buffer data

#define FOV_SCALE 1.3
#define ITERATIONS 60
#define SHADOW_ITERATIONS 24
#define REFLECTION_ITERATIONS 24

#define EPSILON .0008
#define NORMAL_EPSILON .004

#define VIEW_DISTANCE 4.

#define pi 3.141592

const float ballRadius = .035;

// Distance functions from iq's site
float sdTorus( vec3 p, vec2 t ) {
  vec2 q = vec2(length(p.xz)-t.x,p.y);
  return length(q)-t.y;
}
float udBox( vec3 p, vec3 b )
{
  return length(max(abs(p)-b,0.0));
}
float sdBox( vec3 p, vec3 b )
{
  vec3 d = abs(p) - b;
  return min(max(d.x,max(d.y,d.z)),0.0) +
         length(max(d,0.0));
}
float sdCapsule( vec3 p, vec3 a, vec3 b, float r )
{
    vec3 pa = p - a, ba = b - a;
    float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
    return length( pa - ba*h ) - r;
}
float sdTriPrism( vec3 p, vec2 h )
{
    vec3 q = abs(p);
    return max(q.z-h.y,max(q.x*0.866025+p.y*.5,-p.y)-h.x*0.5);
}
float smin( float a, float b, float k )
{
    float h = clamp( 0.5+0.5*(b-a)/k, 0.0, 1.0 );
    return mix( b, a, h ) - k*h*(1.0-h);
}

vec2 rot(in vec2 v, in float ang) {
    float si = sin(ang);
    float co = cos(ang);
    return v*mat2(si,co,-co,si);
}

float sdBall(in vec3 rp) {
    // Get ball data from texture
    vec4 ballData = texture(iChannel0, vec2(0.5, 0.0)/iResolution.xy);
    vec4 decodedBall = ballData - 1.0; // decodeBall function
    return length(rp-vec3(decodedBall.x,1.065,decodedBall.y))-ballRadius;
}

float flaps(in vec3 rp) {
    // Get flap data from texture
    vec4 flapData = texture(iChannel0, vec2(2.5, 0.5)/iResolution.xy);
    
    vec3 b1 = rp;
    b1.y -= 1.05;
    b1.x += .79;
    
    vec3 b2 = b1;
    
    b1.z -= .24;
    b2.z += .24;
    
    b1.xz = rot(b1.xz,2.-flapData.x);
    b2.xz = rot(b2.xz,flapData.y+1.);
    
    b1.z += .08;
    b2.z -= .08;
    
    return min( udBox(b1, vec3(.013,.05,.12)),
                udBox(b2, vec3(.013,.05,.12)) );
}

float pins(in vec3 rp) {
    vec3 p = rp;
    p.y -= 1.1;
    vec3 p2 = p;
    
    p.x = abs(p.x+.12)-.25;
    
    p2.x -= .7;    
    
    return min(
        sdCapsule(p,vec3(0.),vec3(0.,-.12,0.),.03),
        sdCapsule(p2,vec3(0.),vec3(0.,-.12,0.),.03) );
}

float map(in vec3 rp) {
    
    vec3 asr = rp;
    asr.z = abs(asr.z);
    asr.x -= -1.;
    asr.xz = rot(asr.xz, 2.3);
    
    return min( min(min(udBox(abs(asr-vec3(.7,1.05,.75))-vec3(.15,0.,.2),vec3(.07,.06,.02)),
                        length(asr-vec3(1.1,1.1,1.35))-.1),
                    sdCapsule(asr,vec3(0.,1.05,.7),vec3(0.,1.05,.4),.05)),
        max(-sdBox(rp-vec3(0.,1.,0.),vec3(1.,.1,.5)), -rp.y+1. ));
}

float df(in vec3 rp) {
    return min(pins(rp), min(sdBall(rp), min(flaps(rp), map(rp))));
}
float df_hq(in vec3 rp) {
	return df(rp);//-texture(iChannel1,(abs(rp.xz)+abs(rp.yy)*.5)).x*.004*max(0.,cos(iTime+length(cos(rp*4.234)*14.234)));
}

const vec3 ne = vec3(NORMAL_EPSILON,0.,0.);
vec3 normal2(in vec3 rp) {
    return normalize(vec3(df(rp+ne)-df(rp-ne),
                          df(rp+ne.yxz)-df(rp-ne.yxz),
                          df(rp+ne.yzx)-df(rp-ne.yzx)));
}

vec3 normal(in vec3 rp) {
    return normalize(vec3(df_hq(rp+ne)-df_hq(rp-ne),
                          df_hq(rp+ne.yxz)-df_hq(rp-ne.yxz),
                          df_hq(rp+ne.yzx)-df_hq(rp-ne.yzx)));
}

struct material {
    vec3 diffuse,specular,emissive;
    float metallic,roughness;
};

material mat(vec3 rp) {
    
    int closestId = -1;
    float closestDst = 999999.;
    
    #define cd(f,i) if ((buf=f(rp))<closestDst){closestId=i;closestDst=buf;}
    
    float buf;
    cd(pins,0);
	cd(flaps,1);
    cd(sdBall,2);
    cd(map,3);
    
    if (closestId == 0) {
        return material(vec3(1.),
                        vec3(1.),
                        vec3(1.,rp.x+.5,rp.x+.5)*(.75+cos(rp.y*100.+iTime*10.)*.25),
                        0.,
                        1.);
    }
    if (closestId == 1) {
    	return material(vec3(.34), //diffuse
                     vec3(.74,.54,.65), //specular
                 	 vec3(0.), //emissive
                     .74,//metallic
                     .1);//roughness
    }
    if (closestId == 2) {
		return material(vec3(.74), //diffuse
                     vec3(.9), //specular
                 	 vec3(0.), //emissive
                     0.84,//metallic
                     0.04);//roughness   
    }
    
    float boxes = length(max(abs(mod(abs(rp.xz),.5)-.25)-.24, 0.));
    if (boxes > 0.) {
        vec3 c = vec3(length(max(abs(mod(abs(rot(rp.xz,.4)),.02)-.01)-.008, 0.))>0.? .2 : 0.);
    	return material(c, vec3(.4)+c*.5, vec3(0.), 1., .03);
    }
    
    float bid = (floor(rp.x/.5)+floor(rp.z/.5)*12.)+150.;
    return material(vec3(cos(bid*.0383+.9824),cos(bid*.0283+1.639),cos(bid*.0729+.9824))*.5+.5, //diffuse
                     vec3(1.-(cos(bid*.234)*.5+.5)*.5), //specular
                 	 vec3(0.), //emissive
                     cos(bid*.983+.9824)*.5+.5,//metallic
                     cos(bid*.234)*.5+.5);//roughness

}

// Lighting structures
struct light {
    vec3 position, color;
    float size;
};

#define nLights 2

#if nLights != 0
light lights[nLights];
#endif    

void initLights() {
    #if nLights != 0
    lights[0] = light(vec3(0.2,0.5,.5),
                      vec3(1.,.7,.85),
                      1.5);
    
    lights[1] = light(vec3(0.2,0.5,-.5),
                      vec3(0.75,.95,.83),
                      1.5);
	#endif
}

const vec3 ambient = vec3(.4);

//rp = ray pos
//rd = ray dir
//maxDist = max trace distance
//returns -1 if nothing is hit
float trace(in vec3 rp, inout vec3 rd, float maxDist) {
    
    float d,s = 0.;
    for (int i = 0; i < ITERATIONS; i++) {
        d = df(rp+rd*s);
        if (d < EPSILON || s > maxDist) break;
        s += d;
        
        //rd = normalize(rd+vec3(.01,-.001,0.)*d);
    }
    
    if (d < EPSILON) return s;
    
    return -1.0;
}
float rTrace(in vec3 rp, inout vec3 rd, float maxDist) {
    
    float d,s = 0.;
    for (int i = 0; i < REFLECTION_ITERATIONS; i++) {
        d = df(rp+rd*s);
        if (d < EPSILON || s > maxDist) break;
        s += d;
        
        //rd = normalize(rd+vec3(.01,-.001,0.)*d);
    }
    
    if (d < EPSILON) return s;
    
    return -1.0;
}

vec3 randomHemiRay(in vec3 d, in vec3 p, in float amount) {
    vec3 rand = normalize(cos(cos(p)*512.124+cos(p.yzx*16.234)*64.3249+cos(p.zxy*128.234)*64.4345));
    return mix(d, rand*sign(dot(d,rand)), amount);
}

float rand(vec3 s) {
    //Thanks to Shane for the improved random function
    return fract(cos(dot(s, vec3(7, 157, 113)))*43758.5453);
}

vec3 randDir(vec3 s) {
    return vec3(sin(rand(s.yzx+.89234)*34.24),
                cos(rand(s.zxy-1.445)*34.24),
                -cos(rand(s)*34.35));
}

//ambient occlusion function is XT95's from https://www.shadertoy.com/view/4sdGWN
float ambientOcclusion(in vec3 rp, in vec3 norm) {
    float sum = 0., s = EPSILON;
    vec3 lastp = rp;
    
    for (int i = 0; i < 16; i++) {
        vec3 p = rp+randomHemiRay(norm,lastp*(1.+float(i)*.15),.3)*s;
        sum += max(0., (s*.5-abs(df(p)))/(s*s));//randomHemiRay(norm,rp,.5)*s);
        lastp = p;
        s += .01;
    }
    
    return clamp(1.-sum*0.01, 0., 1.);
}

float softShadowTrace(in vec3 rp, in vec3 rd, in float maxDist, in float penumbraSize, in float penumbraIntensity) {
    vec3 p = rp;
    float sh = 0.;
    float d,s = 0.;
    for (int i = 0; i < SHADOW_ITERATIONS; i++) {
        d = df(rp+rd*s);
        sh += max(0., penumbraSize-d)*float(s>penumbraSize*2.);
        s += d;
        if (d < EPSILON || s > maxDist) break;
    }
    
    if (d < EPSILON) return 0.;
    
    return max(0.,1.-sh/penumbraIntensity);
}

vec3 background(in vec3 rd) {
	vec3 c = vec3(0.);
    #if nLights != 0
    for (int i = 0; i < nLights; i++) {
        c += lights[i].color*max(0., dot(rd, normalize(lights[i].position)))*.6;
    }
    #endif
    return c;
}

vec3 locateSurface(in vec3 rp) {    
    vec3 sp = rp;
    for (int i = 0; i < 3; i++) {
        float sd = abs(df(rp));
        if (sd < EPSILON) return sp;
        sp += normal2(sp)*sd*.5;
    }
    return sp;
}

void lighting(in vec3 td, in vec3 sd, in vec3 norm, in vec3 reflDir, in material m, inout vec3 dif, inout vec3 spec) {
    float ao = ambientOcclusion(td,norm);
    dif = ambient*ao;
    spec = vec3(0.);
        
    #if nLights != 0
    for (int i = 0; i < nLights; i++) {
        vec3 lightVec = lights[i].position-td;
        float lightAtten = length(lightVec);
        lightVec = normalize(lightVec);
        float shadow = softShadowTrace(sd, lightVec, lightAtten, .01, .01);
        lightAtten = max(0., 1.-lightAtten/lights[i].size)*shadow;
        
    	dif += max(0., dot(lightVec,norm))*lights[i].color*lightAtten;
        spec += pow(max(0., dot(reflDir, lightVec)), 4.+(1.-m.roughness)*78.)*shadow*lights[i].color;
    }
	#endif
}

//copy of shade without reflection trace
vec3 shadeNoReflection(in vec3 rp, in vec3 rd, in vec3 norm, in material m) {
    vec3 sd = rp+norm*EPSILON*10.;//locateSurface(rp)-rd*EPSILON*2.;
    
    //lighting
    vec3 reflDir = reflect(rd,norm);

    vec3 lightDif,lightSpec;
    lighting(rp,sd,norm,reflDir,m,lightDif,lightSpec);

    return (1.-m.metallic)*lightDif*m.diffuse +
       	    (.5+m.metallic*.5)*lightSpec*m.specular +
       	    m.emissive ;
}

vec3 shade(in vec3 rp, in vec3 rd, in vec3 norm, material m) {
    vec3 sd = rp+norm*EPSILON*10.;//locateSurface(rp)-rd*EPSILON*2.;
    
    //lighting
    vec3 dlc = vec3(0.);
    
    vec3 slc = vec3(0.);
    vec3 reflDir = reflect(rd,norm);
    vec3 tReflDir = normalize(reflDir+cos(rp*245.245-rd*cos(rp*9954.345)*3532.423)*m.roughness);
    tReflDir *= sign(dot(tReflDir,reflDir));
       
    float rtd = rTrace(sd,tReflDir,VIEW_DISTANCE);
    if (rtd < 0.) {
        slc = background(tReflDir);
    } else {
        vec3 rhp = sd+tReflDir*rtd;
        slc = shadeNoReflection(rhp,reflDir,normal(rhp),mat(rhp));
    }
    
    vec3 lightDif,lightSpec;
    lighting(rp,sd,norm,reflDir,m,lightDif,lightSpec);
    dlc += lightDif;
    slc += lightSpec;
    
    float fres = 1.-max(0., dot(-rd,norm));
    
    return (1.-m.metallic)*dlc*m.diffuse +
       	    slc*m.specular*((.5-m.metallic*.5)*fres+m.metallic*(.5+m.metallic*.5)) +
       	    m.emissive ;
}

out vec4 FragColor;

void main()
{
    vec2 R = iResolution.xy;
	vec2 uv = (gl_FragCoord.xy - R*.5)/R.x;

    initLights();
    
    // Get ball and flap data from buffer
    vec4 ballData = texture(iChannel0,vec2(.5)/iResolution.xy);
    vec4 flapData = texture(iChannel0,vec2(2.5,0.5)/iResolution.xy);
    
    // Decode ball data
    vec4 ball = ballData - 1.0; // decodeBall function
    
    // Use flap data directly
    vec4 flap = flapData;
  
	vec3 cameraLocation = vec3(-1.3, .5, 0.);
	vec2 cameraRotation = vec2(ball.y*.4, 1.03);
    
    vec3 rp = cameraLocation;
    vec3 rd = normalize(vec3(uv*vec2(1.,-1.)*FOV_SCALE,1.));

    rd.yz = rot(rd.yz,cameraRotation.y);
    rd.xz = rot(rd.xz,cameraRotation.x);
        
	float itd = trace(rp,rd,VIEW_DISTANCE);
    if (itd < 0.) {
        FragColor = vec4(background(rd),1.);
        return;
    }

    vec3 hp = rp+itd*rd;
    FragColor = vec4(mix(shade(hp,
                      rd,
                      normal(hp),
                      mat(hp)), 
                      background(rd), 
                      max(0.,itd/VIEW_DISTANCE)),1.);
}