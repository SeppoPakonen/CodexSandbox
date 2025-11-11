#version 330 core

// Buffer A: Physics and collision detection
// This shader handles ball physics, collisions, and updates positions

uniform sampler2D iChannel0;  // Position and velocity data
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

// collision detection
// take position and destination for linear move, and a collision radius
// update position to destination, or intersection if collided
// return distance along ray to collision, or 1.0 if no collision
float Collide( in vec3 pos, in vec3 destination, in float radius )
{
    radius -= collisionThreshold;
    
    float d = length(destination-pos);
    vec3 r = (destination-pos)/d;
    
	// DON'T do SDF+radius - SDF's gradient is <= 1.0 so ball will hover!
    // instead, sample at the front of the ball
    // this means it won't collide properly in tight nooks
//    pos += r*radius;
//AARGH! that ruins rolling!
// => maybe we need a correct, analytical gradient
// or maybe we can correct it because we have more time
    vec3 n = GetNormal(pos,.001,false);
    pos -= n*radius; // aha! displace it toward closest surface
    
    float h = SDF(pos,false);
    
    // early out
    if ( h >= d
       || ( h < collisionThreshold && dot(r,n) > 0. ) ) // hack, don't collide if we're below the ground & moving outward
    {
        return 1.0;
    }
    
    float t = 0.;
    for ( int i=0; i < 20; i++ )
    {
        t += h;
        h = SDF(pos+r*t,false);
        if ( t > d || h < collisionThreshold )
            break;
    }
    
    t /= d;
    return min(t,1.0);
}

// drop a point onto the ground
vec3 Place( vec2 xz )
{
	const float minY = -100., maxY = 100.;
	// if we're using strict heightfields we can solve this, but let's collide so it can handle anything
	float h = Collide( vec3(xz.x,maxY,xz.y), vec3(xz.x,minY,xz.y), ballRad );
	return vec3(xz.x,mix(maxY,minY,h),xz.y); // this syntax is ugly
}

out vec4 outData;

void main()
{
    ivec2 uv = ivec2(gl_VertexID % int(iResolution.x), gl_VertexID / int(iResolution.x));
    
    // update position & velocity
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
    
    vec3 lastCamPos = camPos;
    vec3 lastBallPos = ballPos;
    
    if ( iFrame == 0 )
    {
        ballPos = Place(vec2(0))+vec3(0,ballRad,0);
//        holePos = Place(vec2(0,10))+vec3(0,-holeRad,0);
        ballDPos = vec3(0);
        mouseDragStart = vec2(-1,0);
        mouseDrag = vec2(0);
        camPos = ballPos + vec3(0,1,-3);
    }
    
    
    // hole placement
    // place one hole at a time, each time ball lands in hole spawn new hole (& perhaps randomise terrain)
    if ( ( stationary && length(ballPos-holePos) < holeRad ) || iFrame == 0 )
    {
        const vec2 courseDirection = vec2(0,1);
        // place new hole
        holePos = Place( ballPos.xz + (courseDirection+vrand(0U).xz*.5)*70. );
        holePos.y -= holeRad; // hole position is centre of sphere, with cylindrical hole cut above it

        // move ball up onto ground (I should probably add a tee to flatten this - or just an "on tee" state)
        ballPos = Place( ballPos.xz );
    }

    // detect mouse drag
    vec2 lastDragStart = mouseDragStart;
    vec2 dragged = vec2(0,0);
    if ( iMouse.x > 0. && iMouse.z > .0 ) // mouse is dragging
    {
        if ( mouseDragStart.x == -1. ) mouseDragStart = iMouse.xy;
        mouseDrag = iMouse.xy - mouseDragStart;
    }
    else if ( mouseDragStart.x != -1. ) // mouse isn't dragging, but it was on previous frame
    {
        mouseDrag = iMouse.xy - mouseDragStart; // remember the last drag, in case we need it (e.g. for a later buffer step)
        dragged = mouseDrag;
        // reset drag data
        mouseDragStart = vec2(-1,0);
    }

    // track camera with ball
    vec3 camTarget = ballPos+vec3(0,.3,0) + normalize( (camPos-ballPos)*vec3(1,0,1) )*2.;
    camTarget -= camPos;
    camPos += normalize(camTarget)* max((length(camTarget))*.1,0.);
    
	// keep the camera out of the ground
    camPos.y += max( -1.5*SDF(camPos-vec3(0,.3,0),false), 0. );
    
    
// debug: hit the ball in random directions
/*if ( stationary && (iFrame&0xff) == 128 )
{
    float a = iTime*1.618;
    ballDPos = vec3(.707*cos(a),.707,.707*sin(a)) * (15.*exp2(sin(iTime*77.7)))/60.; // metres/second
}*/

    // golf swing
    vec2 swingPos = iResolution.xy*vec2(.499,.299);
    float swingRad = iResolution.y*.03;

    if ( length( lastDragStart - swingPos ) < swingRad ) // we're dragging on the ball
    {
        // controls
        if ( stationary )
        {
            float strength = 0.;
            strength = clamp((-dragged.y)/(.25*iResolution.y),0.,1.);

            // hit ball with force proportional to mouse y
    // this is getting a thwack even when dragged up!?
    //        float thwack = clamp(-dragged.y/(.25*iResolution.y),0.,1.);
            if ( strength > 0. )
            {
    //aaargh why isn't this working?
    //This is hitting with almost constant force, not a range from 1 to 255 (where 7/8 should be half)
                float force = exp2(strength*3.)*3./60.;
                ballDPos += force*normalize((ballPos-camPos)*vec3(1,0,1))*.7071 + vec3(0,max(0.,force-6./60.),0); // low-speed hits don't get any vertical hit, powerful hits approach 45 degrees
    //ballPos += ballDPos; // stop it getting stuck in the ground
            }
        }
	}
    else
    {
        if ( mouseDragStart.x != -1. )
        {
            // rotate camera by mouseDrag
            float a = mouseDrag.x*.0001; // rotation speed

            camPos -= ballPos;
            camPos.xz = camPos.xz*cos(a)+sin(a)*vec2(1,-1)*camPos.zx;
            camPos += ballPos;
            camPos.y += mouseDrag.y*.001;
        }
    }    

	float t = 1.;

// this is allowing the ball to drift downhill whilst "stationary"
// but turning off gravity would be bad. Hmm...
    {
        // gravity
        ballDPos.y -= 9.81/3600.;

        // air resistance
        ballDPos -= .01*ballDPos*length(ballDPos);
    }
        
    t = Collide( ballPos, ballPos+ballDPos, ballRad );
    
    ballPos += t*ballDPos;
    
    if ( t < 1.0 )
    {
        // collision, bounce ball off surface
    	vec3 n = GetNormal(ballPos,ballRad,false);

        float perp = dot(ballDPos,n);
        
        ballDPos -= perp*n;
        
        // friction
        ballDPos -= min(length(ballDPos),.0007)*normalize(ballDPos); // more realistic
       	//.0007 = fairway, .0003 = green, roughly.
        
        float rebound = max( -bounce*perp-.01, .0 );
        
        ballDPos += rebound*n;
        
        // complete the move after the collision, otherwise the ball won't roll
        ballPos += (1.-t)*ballDPos;

        // we only compute one collision per frame, so force it out of the ground in case it's fallen through
        ballPos += n*max(-(SDF(ballPos-n*ballRad,false)),0.); // can't rely on SDF-ballRad, because gradient is <= 1
    }
    
    // stop almost-stationary balls
    if ( length(ballDPos) < .0001 )
        ballDPos = vec3(0);

    // fade in TAA when camera is slow
    // this should factor in perspective & camera rotation (or do depth reprojection)
    taa = pow( smoothstep( 1., .0,
// these aren't working because the camera & ball are moved in buf a => store taa value in campos.w
							length(camPos-lastCamPos)/1. +
                        length(normalize(ballPos-camPos)-normalize(lastBallPos-lastCamPos))/.01 // rotation
		),2.);
    
    // store results
    
    outData = vec4(0,0,0,1);
    
    ivec2 iuv = uv;
    if ( iuv.y == 0 )
    {
        if ( iuv.x == 0 ) outData.xyz = ballPos;
        else if ( iuv.x == 1 ) outData.xyz = ballDPos;
        else if ( iuv.x == 2 ) outData = vec4(mouseDragStart,mouseDrag);
        else if ( iuv.x == 3 ) outData = vec4(camPos,taa);
        else if ( iuv.x == 4 ) outData.xyz = holePos;
    }
}