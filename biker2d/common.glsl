const int LEVELS = 4;
const float SCALE = .5 //4. // // of track noise
, difficultyh = .2 // similar to SCALE, affect height of bumps
, difficultye = 1e-2 // exponential scaling over track length
;
const int nparticles = 3;
const float pi = acos(-1.);

const float wheelradius = .05
    , tirethick = .02;

vec2 StoQ(vec2 s, vec2 r)
{
    return (s+s-r)/r.y;
}

bool isAttract(vec4 mouse)
{
    return mouse.z <= 0. && dot(mouse.xy, mouse.xy) <= 4.;
}

const int // particle id's
    pidBody = 0
  , pidWheelBack = 1
  , pidWheelFront = 2
  ;
    
// particles are each a vec4
// where the pseudo-struct is
// xy=position, zw=velocity
// but I should change it to zw=oldposition
// because I swear, verlet is much easier 
// to deal with than euler integration!
// I just know this euler crap will blow up.

// relative offset from segment given two vectors relative to one endpoint of seg
// v is from a to query pt, e is from a to b points of segment
vec2 rseg(vec2 v, vec2 e)
{ // this is taken from iq's sdLine I think
    return v - e * clamp(dot(v,e)/dot(e,e), 0., 1.);
}
// just take length afterward

float dseg(vec2 v, vec2 e)
{
    return length(rseg(v, e));
}

// perp dot product for finding which side of an edge
float pdot(vec2 a, vec2 b)
{
    return a.x * b.y - a.y * b.x;
}

// spinor of radians = rotator
vec2 cossin(float r)
{
    const float halfpi = .5*pi; //1.5707963;
    return cos(vec2(r, r - halfpi));
}

// see https://stackoverflow.com/questions/12964279/whats-the-origin-of-this-glsl-rand-one-liner
float rand(float x)
{
    return fract(sin(x) * 25314.67285);
}


// see Eikonal Voronoise 1D https://shadertoy.com/view/wsXcW7

float tent1(float x)
{
    return max(0., (1.-abs(x)));
}

float noise1(float x)
{
    float o = x;
    x = mod(x - 1., 2.) - 1.;
    float y = sin(3.1415927*(sqrt(5.)+1.)*floor((o-1.)/2.)); // some pseudorandom hash
	float v = tent1(x);
    v *= sqrt(.5); // fix sheared 'distance' measurements - but this is 1D so unsure it's even a good thing to try
    if (y < 0.) v = -v;
    return v;
}

float fbm1(float x)
{
    float d = -1.
    , s = 1.;
    for (int i = LEVELS; --i >= 0; s *= .5, x *= 2.) {
        float n = s * noise1(x);
        d = max(d, n);
    }
    return d;
}

float lodshape(float dshape, float dlod, float fade)
{
    float b = abs(dlod);
    if (b >= fade) return dlod;
    if (b <= 0.) return dshape;
    float a = abs(dshape);
    return mix(dshape, dlod, b / fade);
}

float lnoise1(float x, float y)
{
    float n = fbm1(x);
    // FIXME I get the feeling n is 0..1 here ?!  honestly don't understand what's going on w the range
    n = clamp(n, -.01, .19);
    n *= difficultyh * exp2(clamp(difficultye*x, 0., 3.)); // hills grow in steepness/size as track progresses
    n = min(n, .5); // let's not get too ridiculous
	const float slab = 1.; // for clipping max extent of hill
    float d0 = y - n, d1 = sign(y) * max(0., abs(y) - slab);
    return d0;
}

// track signed height pseudo-distance
float dtrack(vec2 q)
{
    float pd = q.y
    , nd = lnoise1(q.x * SCALE, (q.y * -SCALE + .2)) / -SCALE
    , d = min(nd, pd);
    return d;
}

// gradient for normal of track
vec2 ntrack(vec2 q)
{
    vec2 e = vec2(0,.002);
    return normalize(vec2(
    	    dtrack(q + e.yx)
    	  , dtrack(q + e.xy)
        ) - dtrack(q));
}

vec4 getParticle(sampler2D ch, int i)
{
    return texelFetch(ch, ivec2(i,0), 0);
}


