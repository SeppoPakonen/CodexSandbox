// iChannel0 = Buffer A
// iChannel1 = rock texture
// iChannel2 = some black and white weird bubble/rock texture
// iChannel3 = font texture


// Biker 2D by spalmer

// fork of Glitter Confetti 2D, (private) https://shadertoy.com/view/wttSWN
// one of these days I'll get that to actually work like I wanted, hopefully!

// jorge2017a1 gave me an idea,
// I mean I have been wanting to
// port Excitebike for a long time,
// just never got around to it.
// I will try!
// This is first playable prototype, enjoy lol
// Currently more like a simplified Trials than Excitebike  :(
// TODO maybe an isometric-rendered version
// TODO with opponents! competitor bikes :) auto-piloted by AI! lol
// The AI (blue bike) currently only runs when mouse is at origin for "attract" mode in thumbnails
// TODO time limit (just shows current iDate.w currently)
// TODO choose tracks
// TODO parallax fore/background objects?
// original had hay bales and cameramen,
// final flag-waver at finish line,
// and a crowd stand in the background.
// TODO boost button, overheating
// TODO boost pads
// TODO mud puddles

#define BufA iChannel0
#define Font iChannel3

const float cscale = .5;


#if 1 // numeric display based on https://shadertoy.com/view/llySRh
// using font in iChannel1; otherwise I'd put it in Common
float char(vec2 p, int c)
{
    if (p.x<.0|| p.x>1. || p.y<0.|| p.y>1.)
        return 0.;
    vec2 q = p/16. + fract(vec2(c, 15-c/16) / 16.);
    vec4 t = textureLod(Font, q, 0.);
    // probably not perfect but seems ok
    float scale = 64./(1.+dot(fwidth(q), vec2(1))); //(abs(dFdx(q.x))+abs(dFdy(q.y))); //length(fwidth(p)); //
    scale = clamp(scale, 1./1024., 1024.);
    float g = t.a; // a unorm-encoded signed distance
    g = .5 - (g - .5) * scale; // antialiasing
    g = clamp(g, 0., 1.);
    return g;
}
    //float w = scale/16.;// /iResolution.y; //.05; //
    //g = clamp(.5 - 1./w * (t.w-.5), 0., 1.); //
    //g = smoothstep(.5+w,.5-w, t.w); // aa sdf - could probably do pixel perfect with a bit more effort
    //g = clamp(.5 - (t.w - .5) * 8./scale; // yeah but we need scale info, maybe do the scaling here?

// display unsigned int up to 4 digits base 10
float pUint(vec2 p, float n)
{
    float v = 0.;
    for (int i = int(n); i > 0; i /= 10, p.x += .5)
        v += char(p, 48 + i%10);
    if (abs(n) < 1e-7)
        v += char(p, 48);
    return v;
}

// display unsigned int 4 digits base 10
float pUint4(vec2 p, float n)
{
    float v = 0.;
    for (int i = int(n), d = 4; d-- > 0; i /= 10, p.x += .5)
        v += char(p, 48 + i%10);
    return v;
}
#endif

mat3x3 bikeFrame() 
{
    vec4  p0 = getParticle(BufA, 0)
        , p1 = getParticle(BufA, 1)
        , p2 = getParticle(BufA, 2)
        ;
    vec2  d = (p2 - p1).xy;
    d = normalize(d);
    // HACK for now I'm just gonna ignore
    // the swapping of the power wheels
    // and just fix the bike frame visuals
    // if bike wheels get inverted somehow
    // actually now it just resets the game,
    // so no longer matters, won't be drawn.
    vec2 a = d;
    if (pdot(d, (p0 - p1).xy) < 0.) // determinant()
        d = -d;
    mat3x3 frame;
    frame = mat3x3(
          vec3(d, 0)
        , vec3(-d.y,d.x, 0) // perp d
        , vec3(p0.xy + a * .02, 1)
        );
    return frame;
}

float dframe(vec2 q)
{
    float d0 = length(q - vec2(.007,-.04)) - .034
       ,  d1 = dseg(q - vec2(-.09,-.1), vec2(.08,.08)) - .01
       ,  d2 = dseg(q - vec2(-.09,-.1 ), vec2(.1,.04)) - .01
       ,  d3 = dseg(q - vec2(.04,-.04), vec2(.06,-.06)) - .01
       ,  d = min(min(d0, d1), min(d2, d3));
    return d;
}
const float cplx = .98;
// "clouds" parallax scrolling background
// this is just the mask for the cloud texture in iChannel2
float dcloud(vec2 q, vec2 parallax)
{
    q -= cplx*parallax; // parallax adjust
    q.x /= 2.;  // stretch
    q *= cscale;
    if (q.y < .0) return 99.;
    return (length(.5-fract(q)) - .0) / cscale;
}

// terrain "surface" rendering - sky/ground color
vec3 drawRamps(float d)
{
    vec2 R = iResolution.xy;
    float g = .5-.5*cos(d * R.y * .5);
    g = 1.; // no lines
	vec3 c = d < 0. ? vec3(.25,.25,.1) : vec3(.2,.5,.8);
    c *= g;
    //c *= 1.-exp2(-.5*iResolution.y * abs(d));
    c = mix(c, vec3(.6,.4,.2), exp2(-.5*iResolution.y * abs(d)));
    return c;
}
    //g = mix(1., g, exp2(-128.*abs(d))); // isolines

// draws particles
vec4 drawParticle(vec2 p, int i, vec2 q, vec2 v)
{
    float r = wheelradius
    , d = distance(p, q) - r
    ;
    if (i != 0) d = abs(d + .5*tirethick) - .5*tirethick;
    vec4 c = i == 0 ? vec4(1,1,0,1) : vec4(.1,.1,.1,1.);
    c *= clamp(.5 - .125*iResolution.y * d, 0., 1.);
    return c;
}
    //(cos(vec3(0,2,4) + float(i>>1) * 66.) * .5 + .5);

vec4 drawBike(vec2 q)
{
    vec4 c = vec4(0);
    for (int i = 0; i < nparticles; ++i) {
    	vec4 d = getParticle(BufA, i);
        c = max(c, drawParticle(q, i, d.xy, d.zw));
    }
    mat3x3 mframe = bikeFrame()
    , iframe = inverse(mframe); // probably cheaper ways to accomplish this btw
    float dfr = dframe((iframe * vec3(q,1)).xy);
    float framecov = clamp(.5-iResolution.y/2.*dfr, 0., 1.);
    vec4 cbike = isAttract(iMouse) ? vec4(.1,.1,.5,1) : vec4(.5,.1,.1,1);
    vec4 cframe = cbike * framecov;
    c = c * (1. - cframe.a) + cframe; // proper premultiplied alpha blend
    return c;
}

float dflag(vec2 q, float parallax)
{
    const float pm = 2.; // parallax mul
	q.x += pm*parallax; // seems closer though
    q.x += -.5*pm;
    q.y -= .55;
    q.x = mod(q.x, 10.*pm); // same distance as used for score increments TODO merge constants
    q.x -= 5.*pm;
//    q *= parallax; 
    return dseg(q - vec2(0, -.7), vec2(0, .4)) - 1./iResolution.y; // just a pole for now
}

vec4 drawTrack(vec2 q, vec2 parallax)
{
    float dtr = dtrack(q);
    float BGS = .6;
    vec2 qbg = q - .96 * parallax;
    float dmt = .25 + qbg.y * 3. - fbm1(qbg.x / BGS) / BGS; //lnoise1(q.x * BGS, (q.y * -BGS + .2)) / -BGS; //dtrack(q * .1); // + parallax?
    vec3 ctrack = drawRamps(dtr);
    //vec3 ctrackbg = drawRamps(dmt + 5.5);
    if (dtr < 0.) 
        ctrack *= mix(vec3(1),
            texture(iChannel1, q * 2.).rgb
            , exp2(4.*dtr));
    else if (dmt < -0.1)
        ctrack = mix(ctrack, vec3(.3,.5,.2), .2); // ugly mountains :)
    float dfl = dflag(q, mix(parallax.x, 1., .5));
    float flagcov = clamp(.5 - dfl * 2., 0., 1.);
    vec3 cflag = isAttract(iMouse) ? vec3(.4,.4,1.) : vec3(.4,1.,.4);
    ctrack = mix(ctrack, cflag, flagcov); // green flagpoles
    // TODO parallax flags, crowd?, 2.5D view
    float dcl = dcloud(q, parallax) -.3;
    float cloudcov = clamp(.5 - dcl * 2., 0., 1.);
    // meh, better than nothing I guess
    cloudcov *= texture(iChannel2, (q - cplx*parallax)/3.).x;
    ctrack = mix(ctrack, vec3(1), cloudcov);
	return vec4(ctrack, 1);
}
//    ctrack = vec3(ntrack(q) * .5 + .5, 0); // * vec3(1,0,0); // debug track normals

void mainImage(out vec4 c, vec2 p)
{
    vec2 R = iResolution.xy, q = StoQ(p, R);
    vec4 body = getParticle(BufA, 0);  // particle 0 is the main body
	vec2 parallax = body.xy; // attach camera to bike
    parallax.y *= .5; // only partially vertical
    parallax.y += .5;
    parallax.x += .5;
    q += parallax;
    vec4 cbike = drawBike(q)
      , ctrack = drawTrack(q, parallax);
    c = ctrack;
    // FIXME yeah but all the other bike frame blends are wrong too
    c = c * (1.-cbike.a) + cbike; //c.rgb = mix(c.rgb, cbike.rgb, cbike.a); // FIXME blend wrong for premultiplied alpha DOH just extinct&add
    vec4 ui = vec4(0);
    //float timer = mod(iDate.w, 1000.); // HACK
    float score = round(max(0., body.x / 10.));
    ui += vec4(.2,1,.2,1) * pUint((p - R/vec2(6,8)) * 6./R.y, float(score)); //timer));
    c = max(c, ui); //c = mix(c, vec4(ui.rgb, 1), ui.a); //
    c = vec4(pow(c.rgb, vec3(1./2.2)),1);
}

