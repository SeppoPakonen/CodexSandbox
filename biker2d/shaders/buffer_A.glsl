// iChannel0 = Buffer A (this)
// iChannel1 = Keyboard
// iChannel3 = song



// TODO need some leaning controls; gas alone isn't enough - sort of in now, needs work.
// TODO some artificial forces to help balance, especially when jumping.
// TODO stb suggests just "staying wrecked" and pressing a button to right the bike;
// should just store a "wrecked" flag in the state and wait for keypress, then just fix the bike and keep going.
// could even make a button-pressing mini-game out of it!
// TODO if in "attract mode", maybe should automatically gas it periodically?
// FIXME if the reset kick or hard landing manages to invert the particle triangle,
// the front wheel becomes the drive wheel! must detect inverted coordinate frame
// currently just resets the game if it happens!
// TODO run physics in a loop to handle lower frame rates more gracefully.
// TODO switch to Verlet integration, or at least leapfrog or implicit Euler
// FIXME often losing context results in botched state; unsure why resolution pixel test isn't catching it
    
#define BufA iChannel0
#define Kbd  iChannel1

// links table
// GLSL ES does not like arrays of arrays
ivec2[3] links = ivec2[3] (
    ivec2(0, 1)
  , ivec2(0, 2)
  , ivec2(1, 2)
);

const float
  tension = exp2(8.5)
, sdamp = .04
, friction = 6.
, gravity = 1.5
, motorpower = 3.
, leanpower = 1.6
;
// FIXME if all spring tension coeffs are the same, why put them in a table?
vec2[3] linklen = vec2[3] ( // actually length, kspring
   vec2(.13, tension)
 , vec2(.17, tension)
 , vec2(.2 , tension) 
);
 // TODO mass, color, radius

bool key(int k)
{
    return texelFetch(Kbd, ivec2(k,0), 0).x > .5;
}

// the original had a joystick/dpad for leaning
// and changing lanes, and a button for acceleration
// TODO not all features implemented yet
const int
  KEY_SPACE = 32 // accel      - space bar
, KEY_W     = 87 // lane up    - WASD for qwerty
, KEY_A     = 65 // lean left
, KEY_S     = 83 // lane down
, KEY_D     = 68 // lean right
, KEY_LEFT  = 37 // lean left  - arrow keys
, KEY_RIGHT = 39 // lean right
, KEY_UP    = 38 // lane up
, KEY_DOWN  = 40 // lane down
, KEY_ESC   = 27 // reset      - HACK reset bike? TODO
;


void FixupFrame(inout vec4 c, int id, vec2 pos)
{
  //  float t = float(i);
    switch (id) {
        default:
        case pidBody: c.xy = vec2(  0,.232); break;
        case pidWheelBack:  c.xy = vec2(-.1,.1); break;
        case pidWheelFront: c.xy = vec2( .1,.1); break;
  //      default: c.xy = (vec2(rand(t+iDate.w), rand(t*1.6+iDate.z))*.15+.5) * 2. - 1.; break;
    }
    c.xy += pos + vec2(0.,.3); // initial world pos offset to all particles
    c.zw = vec2(.5,.1); // begin travelling rightward
}

void Init(inout vec4 c, int i, vec2 p)
{
    vec2 R = iResolution.xy;
    c = vec4(0);
    if (i >= nparticles || int(p.y) > 0) {
        if (ivec2(p) == ivec2(0,1)) //ivec2(R)-1) //
            c.xy = R+vec2(.5); // resolution pixel
        return;
    }
	FixupFrame(c, i, getParticle(BufA, 0).xy);
}

// the undamped spring was originally iq's from https://shadertoy.com/view/tlyGRR
// but it's been heavily modified since, added damping!
void Constraint(vec4 pa, vec4 pb, out vec2 f,out vec2 v, float lr, float k)
{
    vec2 d = pb.xy-pa.xy; // rel pos
    vec2 w = pb.zw-pa.zw; // rel vel
    float ld = length(d);
    vec2 e = d/max(ld,1e-3); // unit vector aimed at b from a
    vec2 h = tension * e; //k * e; //
    f = (ld-lr) * h; // simple undamped spring force
    f += sdamp * dot(w,e) * h; // damping force (euler integration's unstable without it)
    v = vec2(0); // no impulse
}
    // actual air resistance forces are too detailed for this game
    //vec2 n = vec2(e.y,-e.x); // perp
    //float s = dot(pa.zw, n);
    //f -= n * s * r;

// check for flipped determinant
bool isInverted(vec2 p0, vec2 p1, vec2 p2)
{
    vec2  d = (p2 - p1).xy;
    d = normalize(d);
    return pdot(d, (p0 - p1).xy) < 0.; // determinant()
}

// Tick just updates all the bike particles 
// individually depending on which pixel this is
void Tick(inout vec4 c, int id, vec2 p)
{
    vec2 R = iResolution.xy;
    c = texelFetch(BufA, ivec2(p), 0);
    if (id >= nparticles || int(p.y) > 0) return;
    float dt = iTimeDelta;
    // HACK detect thumbnail and just fix the framerate at a 60fps that seems fine
    bool attract = isAttract(iMouse);
    if (attract) dt = .0167; // HACK otherwise seems too fast
    if (!(dt > 0.) || isnan(dt)) return; // abort!
    dt = clamp(dt, .005, .033); // prevent severe temporal discontinuities from blowing things up
    //dt = .033; // HACK very high or low framerate to stress test physics - high seems fine
    vec4[3] part = vec4[3] (
        getParticle(BufA, 0)
      , getParticle(BufA, 1)
      , getParticle(BufA, 2)
      );
    vec2 p0 = part[pidBody].xy
      , p1 = part[pidWheelBack].xy
      , p2 = part[pidWheelFront].xy
      ;
    // iTimeDelta seems always locked at low-ish framerate (60fps)
    // was originally causing the thumbnail to freak out.
    bool accel  = key(KEY_SPACE)                   
    , leanback  = key(KEY_A) || key(KEY_LEFT )
    , leanfront = key(KEY_D) || key(KEY_RIGHT)
    , laneup    = key(KEY_W) || key(KEY_UP   )
    , lanedown  = key(KEY_S) || key(KEY_DOWN )
    , reset     = key(KEY_ESC)
    , inverted = pdot((p2 - p1).xy, (p0 - p1).xy) < 0. // determinant()
    ;
    // check other particles to see if linked and apply connecting forces
    vec2 op = c.xy;
    vec2 forces = vec2(0), impulses = vec2(0);
    for (int i = links.length(); i-- > 0; ) {
        for (int x = 2; x-- > 0; ) 
            if (id == links[i][x]) {
                int j = links[i][x^1];
	        	vec4 pj = part[j]; //getParticle(BufA, j);
    	    	vec2 f, v;
        	    Constraint(c, pj, f, v, linklen[i].x, linklen[i].y);
	            forces += f;
                impulses += v;
                break;
    	    }
    }
    forces.y -= gravity;
    c.zw += forces * dt + impulses;
    vec2 oldvel = c.zw; // after forces but prior collision
    // FIXME Euler integration isn't stable enough for low framerates, need loop or better implicit integrator
    c.xy += c.zw * dt; // integrate
    float htrack = dtrack(vec2(c.x,0));
    // collide with track
    float dtr = dtrack(vec2(c.x,0));
    bool onground = c.y <= wheelradius - dtr;
    if (attract && onground && !inverted 
        && dot(part[0].zw,part[0].zw) < 4.) // speed limit
         accel = sin(.8*iTime) + sin(6.1*iTime) > .2;
    // AI balancing, spin prevention
    vec2 fw = normalize(p2 - p1);
    if (id == pidBody && attract && abs(fw.y) > .51 && 
        ((p0.y > .5 && !onground) || dot(c.zw,c.zw) > 5. || inverted)) {
        float sf = 0.; // something about the relative velocities vs. positions of p0..2
        sf += fw.y;
        float d = dot(part[0].zw - part[1].zw, fw.yx * vec2(-1,1));
        sf += d;
        // if inverted, we should keep spinning the same direction, otherwise opposite
        if (inverted)
     	    sf = -sf;
        if (sf < -3e-2)
	        leanback = true;
	    else if (sf > 3e-2)
	        leanfront = true;
    }
                
    vec2 g = vec2(0,1);
    if (onground) {
    	g = ntrack(c.xy);
	    c.y = max(c.y, wheelradius - dtr);
    }    
    c.zw = (c.xy - op) / max(dt, 1e-6);
    float vd = dot(g, oldvel);
    bool stopped = dot(oldvel,oldvel) < 3e-3;
    if (onground) {
	    if (vd <= 0.)
	        c.zw -= vd * g;
	    if (id == pidWheelBack) // back tire acceleration
	        if (accel) 
	            c.z += motorpower * dt;
	    if (id == pidBody) // && inverted) // ?
	        if (stopped) // main body touching ground == upside down, going slow?
	        	c.zw += vec2(.1,1.35); // BOING rescue
	        else
	            c.z *= exp2(-friction*dt); // drag/friction
    }
    // only body gets air control forces but it
    // often doesn't know if we're truly airborne
    // so I'll just let player control body leaning at all times
    // also serves as slow forward/reverse
    // probably other ways to manage the forces but this should work ok
    // meh, wind up needing to add counter-torque forces to the other particles.
    // I may need to handle the bike as a whole as a rigid body, in the end.
    if (id == pidBody) {
        float s = 0.;
        if (leanback) ++s;
        if (leanfront) --s;
        if (inverted) s = -s;
        vec2 d = normalize(p1 - p2);        
        c.zw += leanpower*dt*s*d;
    }
    bool broken = isInverted(p0, p1, p2)
    , flipbike = texelFetch(Kbd, ivec2(KEY_SPACE, 1), 0).x > .5; // strobe, must release and press again
	if ((broken || (stopped && inverted)) && (attract || flipbike)) // bike's messed up, just fix it if accelerate is pressed
        FixupFrame(c, id, p0);
}

void mainImage(out vec4 c, vec2 p)
{
    vec2 R = iResolution.xy;
    int i = int(p.x); // + R.x*floor(p.y));
    // check bufferA resolution pixel matches
    vec2 oldr = texelFetch(BufA, ivec2(0,1)/*ivec2(R)-1*/, 0).xy;
    // TODO I should just load the entire state of the bike
    // because I'll surely wind up needing it, and
    // no sense loading the stuff more than once.  Make a struct!
	// FIXME must detect exploded physics 
    // (nan's in particle positions, huge particle
    // heights, gigantic velocities, etc.)
	// and just reset the toy when it happens
    vec4 body = getParticle(BufA, 0);
    //vec4 p1 = getParticle(BufA, 1);
    //vec4 p2 = getParticle(BufA, 2);
    if (iFrame == 0 
     || ivec2(oldr) != ivec2(R+.5)
     || body.y > 4.
     || dot(body.zw,body.zw) > 1e4
     || key(KEY_ESC) // TODO rather just fix the bike, leave the game running
     //|| isInverted(body.xy, p1.xy, p2.xy) // bike's messed up, just reset the game!
        // || (isAttract(iMouse) && score > 5.)
       )
        Init(c, i, p);
    else
        Tick(c, i, p);
}

