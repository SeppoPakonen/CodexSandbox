#version 330 core

// Final Image: Add HUD
// This shader adds the heads-up display elements to the rendered scene

uniform sampler2D iChannel0;  // Position and velocity data
uniform sampler2D iChannel1;  // Rendered scene
uniform vec3 iResolution;
uniform float iTime;
uniform int iFrame;
uniform vec4 iMouse;

// Shared constants
const float ballRad = .025;
const float bounce = .5; // [0,1)
const float collisionThreshold = .001;
const float holeRad = .08;

out vec4 fragColour;

void main()
{
    ivec2 fragCoord = ivec2(gl_FragCoord.xy);
    
	vec4 sceneColor = texelFetch(iChannel1, fragCoord, 0);

	// HUD
    vec4 hud = vec4(0); // premultiplied alpha, so can do additive or blended
    
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

    vec2 jitter = vec2(0);
    
    // this should be moved into shared
	vec3 ray;
    ray.xy = (jitter + vec2(fragCoord) - iResolution.xy*.5)/iResolution.y;
    ray.z = 1.;
    ray = normalize(ray);
    
    // turn camera toward ball
    vec3 camk = normalize(normalize(ballPos-camPos)+vec3(0,.2,0));
    vec3 cami = normalize(cross(vec3(0,1,0),camk));
    vec3 camj = cross(camk,cami);
    
    ray = ray.x*cami + ray.y*camj + ray.z*camk;


    // HUD
    
    // Flag
    float flagH = .3; // to centre of triangle
    float flagW = .1;
    vec3 tohole = holePos+vec3(0,holeRad,0)-camPos;
    tohole = normalize(tohole); // don't do perspective
    float dk = dot(tohole,camk);
    if ( dk > .0 )
    {
        tohole /= dk;
		ray /= dot(ray,camk);
        vec2 flagUV = vec2( dot(ray-tohole,cami), dot(ray-tohole,camj) );
        if ( ( flagUV.x > .0 && flagUV.x < flagW - 1.5*abs(flagUV.y-flagH) )
            || ( abs(flagUV.x) < .005 && flagUV.y > 0. && flagUV.y < flagH+flagW/1.5 ) )
            hud = vec4(.7,0,0,.2); // premultiplied alpha, mostly additive
    }
    

    // golf swing
    vec2 swingPos = iResolution.xy*vec2(.499,.299);
    float swingRad = iResolution.y*.03;

    float strength = 0.;
    if ( length( mouseDragStart - swingPos ) < swingRad )
    {
    	strength = clamp((-mouseDrag.y)/(.25*iResolution.y),0.,1.);
    }

    //if ( mouseDragStart.x != -1. )
    if ( stationary )
    {
        // show strength as a line from a circle, with the circle tracked horizontally to ignore camera move, and greyed out when power is below cut-off
        vec2 hitUV = vec2(fragCoord) - swingPos;//vec2(mouseDragStart.x + mouseDrag.x,mouseDragStart.y);

        float ungreyed = stationary?1.:0.;
        
        vec4 hitCol = mix( vec4(0,0,0,.3), vec4(1,1,0,1), smoothstep(.0,.1,strength)*ungreyed );

        // dotted line from finger to circle to make arrow-is-reflection clearer
        float strengthY = strength*iResolution.y*.25;
		float f = max(max(max( abs(hitUV.x), -hitUV.y-strengthY ), hitUV.y+swingRad ), sin(-iTime*10.+hitUV.y/4.)*4. );
        hud = mix( hud, vec4(0,0,0,.3), smoothstep(1.5,0.,f) );

        // arrow showing force (opposite side to drag)
		f = max(max( abs(hitUV.x), hitUV.y-strengthY ), -hitUV.y+swingRad );
        hud = mix( hud, hitCol, smoothstep(1.5,0.,f) );
        
        // circle showing contact
        hud = mix( hud, hitCol, smoothstep(1.5,0.,abs(length(hitUV)-swingRad)) );
    }

    
    fragColour.rgb = hud.rgb + sceneColor.rgb*(1.-hud.w);
    
    fragColour.rgb = pow(fragColour.rgb,vec3(1./2.2))
        +vec3(1.,0.,2.)/(3.*255.); // break up grey gradients so we get more than 255 steps
}