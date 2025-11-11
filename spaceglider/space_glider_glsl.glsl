/*
 *				SPACE GLIDER 2020 SHADERTOY EDITION
 *					   by Christian Schüler
 *                       (c) 2001 - 2020
 *
 * Part 6 of 6: Image shader (postprocess, HMD and text overlay)
 * This software comes with no warranty. Use it at your own risk.
 *
 * ----------------------------------------------------------------------------
 *
 * You fly the 'Super-XR 7000' spaceplane around a small planet.
 * Select a start location by pressing the corresponding key.
 * After watching the transfer sequence you are free to go.
 *
 *
 * Controls
 * --------
 *
 *		TAB			 		Menu
 *		M					Map view
 *
 *		up/down				Pitch control
 *		up/down + alt/ctrl	Pitch trim (for aero controls only)
 *		left/right			Roll control
 *		A/D or Q/D			Yaw control or wheel steering
 *		W/S or Z/S			Throttle control (with stops at 15%, 35% and 70%)
 *
 *		(pressing shift gives finer controls for all of the above)
 *
 *		F					Move flaps down one notch
 *		F + shift			Move flaps up one notch
 *		V					Toggle spoilers (airbrakes)
 *		G					Toggle landing gear
 *		B					Hold wheel brakes
 *		B + shift			Hold wheel brakes (less effort)
 *		space				Halt (Cut throttle and apply brakes)
 *
 *		< or `				Rotate thrust vector up one position
 *		< or ` + shift		Rotate thrust vector down one position
 *
 *		mouse 				Mouse look
 *		backspace			Forward look
 *
 *		R					Cycle image magnification
 *		H					Cycle overlay brightness
 *		N					Toggle photo multiplier overlay ('night vision')
 *		T					Toggle terrain radar overlay
 *		I					Toggle J-band infrared image
 *
 *		P					'dynamic' pause mode
 *		F1					Time acceleration up to ×10
 *		F2					Time acceleration up to ×100
 *		F3					Time acceleration up to ×1000
 *		F4					Time acceleration up to ×10000 (only in space)
 *	 	F12					CHEESE mode (hide all text and HMD overlays)
 *
 * 	When in map view
 *
 *		TAB			 		Menu
 *		M					Exit map view
 *
 * 		W/S or Z/S			zoom
 *		mouse drag			move/pan
 *		mouse click			set marker position
 *								- double click to remove
 *								- convert marker to waypoint via menu item 6
 *      backspace			reset position
 *
 *
 * Menu: Info pages
 * ----------------
 *  Info pages can be selected via menu item 1.
 *
 *	Location info:
 *		Shows current position.
 *  	Latitude (lat), longitude (long), altitude (alt), heading (hdg).
 *
 *	Waypoint info:
 *		Shows relation to the selected waypoint.
 *		Bearing (brg), slant range (dst), height difference (delta-h),
 *		estimated time to arrival (eta).
 *
 *	Orbit info:
 *		Shows continuously updated orbital elements.
 *		Apoapsis (Ap), periapsis (Pe), eccentricity (e),
 *	  	true anomaly (theta).
 *
 *	Glide info:
 *		Shows continuously updated aerodynamic coefficients.
 *		Lift coeffienct (CL), drag coefficient (CD), glide ratio (L/D),
 *      angle of attack (alpha).
 *
 *	Control info:
 *		Shows current control inputs.
 *		Elevator (elev), aileron (ail), rudder (rudd), trim tab (trim).
 *
 *	Air info:
 *		Shows current outside air data.
 *		Temperature (T), static pressure (P), dynamic pressure (Q),
 *		density (rho).
 *
 *  Time info:
 *		Shows current in-game date (year-day), time (hours:minutes),
 *		local time with timezone. The ingame time scale is defined by the
 *		constant SECONDS_PER_MINUTE in the common tab.
 *
 *
 * Menu: HMD modes
 * ---------------
 *  Modes for the helmet mounted display (HMD) can be selected via menu item 2.
 *
 *	HMD off:
 *		HMD overlay is disabled.
 *
 *	Surface overlay:
 *		Shows speed and flight path relative to the local surface
 *		and a pitch ladder oriented to the local horizon.
 *
 *	Orbit overlay:
 *		Same as surface mode but speed and flight path are shown relative
 *		to the orbit center of mass (local planet) with additional
 *		markers that indicate the outcome of applying thrust in the
 *		given direction:
 *
 *		+a or -a		change Pe and Ap so that eccentricity does not change
 *		+e or -e		change Pe and Ap by equal and opposing amounts
 *		+Pe or -Pe		change only Pe, leave Ap constant
 *		+Ap or -Ap		change only Ap, leave Pe constant
 *		+h or -h		change obital inclination
 *
 *		---   ---		a pair of dashed lines (when they appear) indicate the
 *						pitch angle at which the current engine thrust will
 *						exactly counter gravity
 *
 *	General overlay symbology:
 *
 *		left group		speed in m/s, mach number (M), dyn. pressure in bars (Q)
 *		right group		altitude in meters and vertical speed in m/s
 *		bottom group	heading in degrees and vertical acc. in g units (G)
 *
 *		pitch ladder	major ticks every 10°, minor ticks every 5°
 *						surface only: one tick at -3° for landing
 *		_	 _
 * 		 \/\/			Water line
 *						(direction of the body-fixed x axis)
 *        .
 *      --O--			Flight path marker
 *						(direction of the velocity vector)
 *
 *		\  /
 *       \/				Waypoint (if set)
 *
 *
 * Menu: Aero modes
 * ----------------
 *  Operating modes for the aerodynamic control surfaces (elevator, aileron and
 *  rudder) can be selected from menu item 3.
 *
 *	Aero off:
 *		Aero control is disabled, but manual trim setting is preserved.
 *
 *	Direct manual control:
 *		Pitch, roll and yaw inputs are directly connected to elevator, aileron
 *		and rudder.
 *
 *	Fly by wire control (experimental):
 *		Automatic controller where pitch input commands a desired g-load (-3..9)
 *		and roll input commands roll rate. Yaw input is still manual.
 *
 *
 * Menu: RCS modes
 * ----------------
 *	Operating modes for the reaction control system (RCS) can be selected from
 *  menu item 4.
 *
 *	RCS off:
 *		RCS control is disabled.
 *
 *	Direct manual control:
 *		Pitch, roll and yaw inputs are directly connected to the corresponding
 *		thrusters.
 *
 *	Rotation rate control:
 *		Pitch, roll and yaw inputs command rotation rates in the inertial
 *		reference frame.
 *
 *	Rotation rate control + LVLH:
 *		Pitch, roll and yaw inputs command rotation rates relative to
 *		the LVLH reference frame (local vertical, local horizon).
 *
 *
 * Menu: Engine modes
 * ------------------
 *  Engine modes can be selected from menu item 5.
 *
 *	Engine off:
 *		All engines are disabled.
 *
 *	Drive engine:
 *		An electical motor connected to the wheels.
 *		Provides passive wheel braking.
 *
 *	Impulse engine:
 *		A hypothetical propellant-less propulsion engine.
 *
 *	Nova engine:
 *		Not yet implemented.
 *
 *
 * How to fly
 * ----------
 *
 *	The simulated plane is a blend between an F16 and a Space Shuttle.
 *  Full throttle gives a thrust-to-weight ratio of 130%.
 *
 *  Some reference speeds (valid at sea level):
 *
 *		stall speed			45 m/s
 *		touchdown speed		65 m/s
 *		approach speed		70 m/s
 *		best glide speed	83 m/s
 *
 *  Speed is shown in m/s relative to the selected frame (surface or orbit).
 *  Inside an atmosphere, the dynamic pressure (Q, in bars) relates to 
 *  equivalent airspeed (EAS, in knots):
 *
 *		Q 	airspeed	Q	airspeed
 *		------------	------------
 * 		0.01	  80	0.20	 350
 *		0.02	 110	0.26	 400
 *		0.03	 140	0.33	 450
 *		0.04	 160	0.41	 500
 *		0.05	 175	0.49	 550
 *		0.06	 190	0.58	 600
 *		0.08	 220	0.68	 650
 *		0.10	 250	0.79	 700
 *		0.12	 270	0.91	 750
 *		0.15	 300	1.04	 800
 *
 * 	Take off:
 *
 *		Gentle (like an airliner)					Scramble (like a fighter)
 *
 *		- Trim 2.5%, flaps 2 notches down			- Trim neutral, no flaps
 *		- Throttle to 35%							- Throttle to 70%
 *
 *		For all cases
 *
 *		- Accelerate to 75 m/s
 *		- Bring the nose up *gently* (repeated tap on keyboard is enough)
 *		- Retract gears immediately
 *      - Retract flaps when climb is stable
 *		- Manually adjust trim as you go, or select the fly-by-wire controller
 *
 * 	Cruise:
 *
 *      During cruise it is recommended to select the fly-by-wire controller,
 *		as this will automatically keep the plane on a straight path.
 *  	Some typical settings to fly like a ...
 *
 *		  				speed	alt		throttle    flaps
 *
 *		Cessna	 		60 		500		10		    3
 *		Airliner		250		11 k	15
 *		Concorde 		600		18 k	45
 * 		SR-71			1000	24 k	50
 *
 *	Landing:
 *
 *  	The goal is to come in on a 5% glide slope (or about -3 degrees).
 *  	This means that at 20 km distance the runway is 1 km below, etc.
 *		The safe limit to deploy gears is about Q < 0.1, but this is not 
 *		checked yet.
 *
 *		The final configuration for approaching the runway should be:
 *
 *			speed 				70 m/s
 *			vertical speed		-3.5 m/s
 *			full flaps
 *			gears down
 *
 *		The vertical speed at touchdown then determines the rating you are
 *		given for the landing:
 *
 * 			0 .. 2 m/s		excellent
 *			2 .. 4 m/s		normal
 *			4 .. 6 m/s		hard landing
 *			6 and above		crash landing
 *
 * 	Going into orbit:
 *
 *		The lowest practical altitude for an orbit in this simulation is 120 km,
 *		everything below that would suffer atmospheric drag. The required speed
 *		is around 2900 m/s. It takes about 8 minutes real time to get there.
 *		The following procedure works to reach near (up to 200 km) orbits in
 *		prograde (eastwards) direction:
 *
 *		(1) Accelerate upwards
 *
 *      At the surface:
 *		- Full throttle
 *		- Maintain a steady 60° climb angle
 *
 *		At 30 km altitude:
 *		- Enable RCS (with rate control)
 *
 *		(2) Transition to accelerate forward and increase Ap
 *
 *      When vertical speed has reached 600 m/s:
 *		- Switch HMD to orbit overlay (shows the orbital velocity vector)
 *		- Follow the flight path marker as it moves down ('gravity turn')
 *      - Switch to orbit info page and keep an eye on the 'Ap' value
 *
 *		(4) Increase Pe
 *
 *		When 'Ap' has reached the intended target altitude:
 *		- Pitch down below the horizon and follow the '+Pe' symbol on the HMD
 *      - Keep an eye on the 'theta' value on the orbit info page
 *		
 *		Eventually one of the following will happen:
 *
 *      - 'theta' is no longer increasing:
 *		    - Throttle down until theta is increasing again
 *
 *      - 'theta' has reached 180°, but 'Pe' is still lower than 'Ap':
 *			- Throttle up and pitch up to the dashed 'hover line' in the HMD
 *      	- keep vertical speed at zero while Pe increases
 *
 *      (5) Orbital insertion:
 *
 *      - You are finished when 'theta' has reached 180° and 'Pe' equals 'Ap'
 *      - Switch RCS to LVLH mode to keep aligned with the orbital rotation
 *      - Congratulations, and enjoy the view!
 *
 *
 * 	Return from orbit:
 *
 *		This procedure requires roughly 1/2 orbit to complete so you will
 *		end up on the opposite side of the planet from where the procedure
 *		is started.
 *
 * 		At first your altitude will decrease, but at some point it
 *		would start to rise again when you get lift from the atmosphere.
 *		The idea in the second part is not let that happen and deflect the
 *		undesirable lift sideways by rolling to the side (hard).
 *
 *		Stage 1 (deorbit)
 *
 *		- Align with flight path
 *		- Reverse thrust until Pe is just above the surface (5 km or so)
 *		- Select aero direct manual control
 *		- Trim up to 100%
 *		- Pitch up to 20 degrees
 *		- Deploy spoilers
 *
 *		Stage 2 (reentry)
 *
 *		- Wait until air contact at about 80 km altitude
 * 		- Switch to surface HMD
 *		- Keep an eye on vertical speed
 *		- Bank sideways to maintain negative vertical speed at -100 m/s
 *		- Perform roll reversals from time to time to maintain course
 *
 *		The following roll reversal schedule is an example
 *		that works most of the time:
 *
 *		- When vertical speed gets to -100 m/s, roll to the side on which
 *			your target is, trim down to 70%
 *		- At mach 7.5, roll to the other side, trim 50%
 *		- At mach 5.5, roll to the other side, trim 20%
 *		- At mach 3.5, or whenever back on course, roll upright,
 *		  disable RCS and trim for glide, or enable fly-by-wire.
 *
 *		You should then be in atmospheric flight, flying Mach 3 in 30 km
 *		altitude right over your target.
 */

// ----------------------------------------------------------------------------

/*
TODO:

-	add automatic flaps mode
-	add autothrottle modes
-	add tracking targets to RCS modes
-	proper sun position
-	support multiple celestial bodies
	-	current localplanet index and orbitplanet index in gamestate
	-	move hardcoded cloud params to planetdata
	-	move hardcoded terrain colors to planetdata
-	seasonal variation of cloud patterns

*/

// ----------------------------------------------------------------------------
// IMAGE OPTIONS
// ----------------------------------------------------------------------------

#define WITH_IMG_BALANCE			1					// enable white balance
#define WITH_IMG_DCI_P3				0					// for MacBook with P3 display when browser does not color manage
#define WITH_IMG_EXPOSURE			1					// enable auto exposure
#define WITH_IMG_FILMIC 			1					// film like contrast enhancement for high exposures
#define WITH_IMG_GLARE				1					// mip-map based glare effect, simulating aperture diffraction
#define WITH_IMG_LENS				1					// mip-map based lens flare effect, simulating internal reflections
#define WITH_IMG_PRIMARIES 			1					// enable conversion from monochromatic color primaries to sRGB (or P3)
#define WITH_IMG_RODVISION			1					// enable a simulation of scotopic vision at low light levels
#define WITH_IMG_SOFT_SATURATE		1					// choose a soft saturation function: 0 = none, 1 = exp, 2 = tanh, 3 = sin
#define WITH_IMG_SRGB_EOTF			1					// use the exact piecewise sRGB curve, otherwise a simple gamma curve
#define WITH_IMG_SUNGLARE			1					// enable the special-case glare effect centered on the sun

#define WITH_IMG_EXPERIMENTAL_ROD_ACUITY 0				// enable a blurry image for scotopic vision

const float IMG_BALANCE = .5;
const float IMG_BLACKLEVEL = 0. / 255.;
const float IMG_BLACKLEVEL_VR = 0. / 255.;
const float IMG_FILMIC = bool(WITH_IMG_SRGB_EOTF) ? .05 : .08;
const float IMG_FILMIC_VR = .02;
const float IMG_FILMIC_EXPOSURE_REF = 4.5;
const float IMG_GAMMA = 2.2;
const float IMG_GAMMA_VR = 2.4; 						// measured for the HTC vive
const float IMG_QUANTIZE = 1. / 255.;

// Conversion matrices from monochromatic color
// primaries (615, 535, 445) to display color space
// including a white point change from illuminant E to D65
#if WITH_IMG_DCI_P3
const mat3	IMG_PRIMARIES = mat3(  1.3272,  0.0177, -0.0008,
                      	  		  -0.1814,  1.0097, -0.0328,
                       			   0.0136, -0.0706,  0.9504  );
#else
const mat3	IMG_PRIMARIES = mat3(  1.6218, -0.0374, -0.0283,
                      		      -0.4493,  1.0598, -0.1119,
                       			   0.0325, -0.0742,  1.0491  );
#endif
const vec3	IMG_BALANCE_ADAPT = pow( inverse( IMG_PRIMARIES ) * ONE, vec3( IMG_BALANCE ) );

const float VIS_EXPONENT = -.65;
const float VIS_EXPONENT_VR = -.35;
const vec3  VIS_LIMITS = vec3( .0013, .94e-6, .0001 );
const vec3  VIS_SCOTOPIC_Y = vec3( .02, .63, .35 ) * 1600. / 683.;
const float VIS_PRE_EXPOSURE = 1.;
const float VIS_POST_EXPOSURE = 1.;

// ----------------------------------------------------------------------------

GameState g_game;
VehicleState g_vehicle;
PlanetState g_planet;
LocalEnvironment g_env;
PlanetData g_data;
float g_pixelscale = 0.;
vec4 g_exposure = vec4(0);
vec3 g_hudcolor = ZERO;
vec3 g_raydir = ZERO;
mat2x3 g_Kr = mat2x3(0);

float g_textlodbias = 0.;
vec2 g_textscale = vec2(1);
vec4 g_overlayframe = vec4(0);

bool g_vrmode = false;
mat3 g_vrframe = mat3(0);
vec4 g_vrfocus = vec4(0);
vec2 g_vrcoord = vec2(0);
vec3 g_vrdir = ZERO;
uniform vec4 unViewport;
uniform vec3 unCorners[5];

vec2 project3d( vec3 r, float z )
{
	return g_vrmode ?
        unViewport.zw * ( g_vrfocus.xy + .5 * z * g_vrfocus.zw * r.yz / r.x * vec2( 1, -1 ) ) :
		iResolution.xy * ( .5 + .5 * z * barrel_distort_inv( CAM_FOCUS * r.yz / r.x, CAM_DISTORT ) * vec2( 1, -iResolution.x / iResolution.y ) );
}

// ----------------------------------------------------------------------------
// HMD PRIMITIVES
// ----------------------------------------------------------------------------

float hmd_chrout_inner( vec2 coord, float size, float chr )
{
    float result = 0.;
    if( coord.x >= 0. && coord.x < size && coord.y >= 0. && coord.y < size )
    {
        vec2 cell = vec2( mod( chr, 16. ), 15. - floor( chr / 16. ) );
        float lod = 5. - log2( size ) + g_textlodbias;
        result += textureLod( iChannel2, cell / 16. + coord * 64. / ( size * iChannelResolution[2].xy ), lod ).x;
    }
    return result;
}

float hmd_chrout( vec2 coord, float size, float chr )
{
    coord.x += TXT_FONT_BACKSLANT * coord.y;
    return hmd_chrout_inner( coord, size, chr );
}

float hmd_txtout( vec2 coord, vec3 cc, int index )
{
    float result = 0.;
    ivec2 addr = ivec2( ( index / 2 ) << 4, int( iResolution.y - 2. ) + ( index & 1 ) );
    vec4 params = IMG_MIPMAP_HIDE * texelFetch( iChannel1, addr, 0 );
    bool underline = params.z < 0.;
    bool vector = params.w < 0.;
    params.zw = abs( params.zw );
    float n = IMG_MIPMAP_HIDE * texelFetch( iChannel1, ivec2( addr.x + 1, addr.y ), 0 ).x;
    bool hudclip = n < 0.;
    n = abs(n);
    if( vector )
    {
        vec3 v = vec3( floor( params.x ), fract( params.x ) * 4096., params.y ) / 2047.5 - 1.;
        if( g_vrmode )
        	v *= g_vrframe;
        if( v.x > 0. )
            params.xy = ( project3d( v, g_game.camzoom ) - g_overlayframe.xy ) * g_textscale
            		    - params.w * vec2( n * TXT_FONT_SPACING, 1 ) / 2.;
        else
            n = 0.;
    }
    coord -= params.xy;
    coord.x += TXT_FONT_BACKSLANT * coord.y;
  	float i = floor( coord.x / ( params.w * TXT_FONT_SPACING ) );
    float w = params.w * n * TXT_FONT_SPACING;
    if( n != 0. &&
        coord.x >= 0. && coord.x < w && coord.y >= -1. && coord.y < params.w &&
        ( !hudclip || ( abs( cc.y ) < HMD_BORDER.x * cc.x && abs( cc.z ) < HMD_BORDER.y * cc.x ) ) )
    {
       	float chr = IMG_MIPMAP_HIDE * texelFetch( iChannel1, ivec2( addr.x + ( int( i + 5. ) >> 2 ), addr.y ), 0 )[ uint( i + 5. ) & 3u ];
   		result += params.z * hmd_chrout_inner( coord - vec2( params.w * ( i * TXT_FONT_SPACING - TXT_FONT_HOFFSET ), 0. ), params.w, chr );
   		if( underline )
       	{
        	float q = min( max( 0., 1. - abs( coord.y + 0. ) ), min( 1. + coord.x, w + 1. - coord.x ) );
        	result += params.z * q;
        }
    }
    return result;
}

float hmd_center_dot( vec2 coord )
{
    float result = 0.;
    vec2 p = g_vrmode ? unViewport.zw * g_vrfocus.xy : iResolution.xy / 2.;
	if( coord.x >= p.x - 1. && coord.x < p.x &&
		coord.y >= p.y - 1. && coord.y < p.y )
	{
		result += .7;
	}
    return result;
}

float hmd_symbol_border( inout vec3 v, vec2 limits )
{
    float result = 1.;
 	if( v.x < 0. )
    	v = UNIT_X + normalize(v);
    v.yz /= v.x;
    v.x = 1.;
    if( abs( v.y ) >= limits.x )
    {
        v.z *= limits.x / abs( v.y );
        v.y = limits.x * sign( v.y );
        result = .5;
    }
	if( abs( v.z ) >= limits.y )
	{
        v.y *= .35 / abs( v.z );
        v.z = .35 * sign( v.z );
        result = .5;
	}
    return result;
}

float hmd_waterline( vec2 coord )
{
    float result = 0.;
    vec3 v = g_vehicle.localB[0] * g_game.camframe;
	if( g_vrmode )
    	v *= g_vrframe;
    if( dot( v, v ) > .25e-6 )
    {
        float sz = hmd_symbol_border( v, HMD_BORDER_SYM );
		mat2 I = mat2( g_textscale.x, 0, 0, g_textscale.y );
    	vec2 p = ( coord - project3d( v, g_game.camzoom ) ) * g_textscale;
       	if( Linfinity( p ) < 10. )
        {
            vec2 a = sz * vec2( -3, -6 );
            vec2 b = sz * vec2( +3, -6 );
            vec2 c = sz * vec2( +6,  0 );
            vec2 d = sz * vec2( +9,  0 );
            float shape = 0.;
            shape = max( shape, aaa_line( I, p, a, vec2(0), 1. ) );
            shape = max( shape, aaa_line( I, p, b, vec2(0), 1. ) );
            shape = max( shape, aaa_line( I, p, a, -c, 1. ) );
            shape = max( shape, aaa_line( I, p, b, +c, 1. ) );
            shape = max( shape, aaa_hline( I, p, -d, sz * 3., 1. ) );
            shape = max( shape, aaa_hline( I, p, +c, sz * 3., 1. ) );
        	result += shape * sz;
        }
    }
    return result;
}

float hmd_flight_path_marker( vec2 coord )
{
    float result = 0.;
    vec3 localv = g_vehicle.modes.x == VS_HMD_ORB ?
        g_vehicle.orbitv * g_planet.B * g_game.camframe :
    	g_vehicle.localv * g_game.camframe;
    if( g_vrmode )
        localv *= g_vrframe;
    if( dot( localv, localv ) >= .25e-6 )
    {
        vec3 v = localv;
        float sz = hmd_symbol_border( v, HMD_BORDER_SYM );
        mat2 I = mat2( g_textscale.x, 0, 0, g_textscale.y );
    	vec2 p = ( coord - project3d( v, g_game.camzoom ) ) * g_textscale;
        vec2 a = vec2( +4, 0 );
        vec2 b = vec2( +9, 0 );
        vec2 c = vec2( 0, +4 );
        float shape = 0.;
        if( Linfinity( p ) < 10. )
        {
        	shape = max( shape, aaa_ring( I, p, sz * 8., 1. ) );
            shape = max( shape, aaa_hline( I, p, -sz * b, sz * 5., 1. ) );
            shape = max( shape, aaa_hline( I, p, +sz * a, sz * 5., 1. ) );
            shape = max( shape, aaa_vline( I, p, +sz * c, sz * 4., 1. ) );
        }
		if( localv.x < 0. &&
            abs( localv.y ) < -HMD_BORDER_SYM.x * localv.x &&
            abs( localv.z ) < -HMD_BORDER_SYM.y * localv.x )
		{
        	p = ( coord - project3d( localv, g_game.camzoom ) ) * g_textscale;
            if( Linfinity( p ) < 10. )
            {
            	shape = max( shape, aaa_ring( I, p, 8., 1. ) );
            	shape = max( shape, aaa_hline( I, p, -a, 8., 1. ) );
            	shape = max( shape, aaa_vline( I, p, -c, 8., 1. ) );
            }
        }
        result += shape * sz;
    }
    return result;
}

float hmd_waypoint( vec2 coord )
{
    float result = 0.;
    vec3 v = ( g_game.waypoint - g_game.campos ) * g_game.camframe;
    if( g_vrmode )
        v *= g_vrframe;
	float sz = hmd_symbol_border( v, HMD_BORDER_SYM );
	mat2 I = mat2( g_textscale.x, 0, 0, g_textscale.y );
    vec2 p = ( coord - project3d( v, g_game.camzoom ) ) * g_textscale;
    if( Linfinity( p ) < 13. )
    {
        float shape = 0.;
    	shape = max( shape, aaa_line( I, p, vec2(0), +sz * vec2( -6, 12 ), 1. ) );
    	shape = max( shape, aaa_line( I, p, vec2(0), +sz * vec2( +6, 12 ), 1. ) );
    	result += shape * sz;
    }
    return result;
}

float hmd_pitch_ladder( vec2 coord, vec3 cc )
{
    float result = 0.;
    vec3 localv = g_vehicle.modes.x == VS_HMD_ORB ?
        g_vehicle.orbitv * g_planet.B :
    	g_vehicle.localv;
	if( dot( localv, localv ) >= .25e-6 )
    {
        vec3 down = normalize( -g_vehicle.localr );
        vec3 horz = normalize( reject( localv, down ) );
       	vec3 left = cross( down, horz );
        mat3 M = mat3( horz, left, down );
        vec3 dir = g_raydir * M;
       	float Kp = degrees( Linfinity( ( down * dir.x - horz * dir.z ) * g_Kr ) ) / dot( dir.xz, dir.xz );
       	float Ks = degrees( Linfinity( left * g_Kr ) ) * inversesqrt( 1. - dir.y * dir.y );

		if( abs( cc.y ) < HMD_BORDER_LAD.x * cc.x && abs( cc.z ) < HMD_BORDER_LAD.y * cc.x )
        {
            // pitch ladder lines
        	float pitch = degrees( atan( -dir.z, dir.x ) );
        	float side = degrees( atan( dir.y, length( dir.zx ) ) );
        	float twist = max( 0., .5 * dir.z );        	
            float p = pitch + twist * ( abs( side ) - 5. );
            bool tick = mod( abs(p) + 2.5, 10. ) < 5.;            
            float shape = 0.;
            if( g_vehicle.modes.x == VS_HMD_SFCE )
                shape = aaa_interval( Kp, pitch + 2.8624, Kp / g_textscale.y );
            shape = ( tick ? 1. : .5 ) * max( shape, 
				aaa_stipple( Kp, p + 2.5, 5., Kp / ( 5. * g_textscale.y ) ) ) * 
                aaa_interval( Ks, abs( side ) - ( tick ? 7.5 : 5.5 ), tick ? 5. : 1. );
            shape = max( shape, 
                aaa_interval( Kp, pitch, Kp ) * aaa_interval( Ks, abs( side ) - 45., 80. ) );
        	shape = max( shape,
        	    aaa_stipple( Kp, pitch + 5. + sign( pitch ) * 0.625, 10., .125 ) *
        	    aaa_interval( Ks, abs( side ) - 7.5 - 2.5 * sign( pitch ), Ks / g_textscale.y ) );
			float bright = .5;
			result += shape * bright;

            // pitch ladder numbers
            const float s15 = sin( radians( 12. ) );
            const float c15 = cos( radians( 12. ) );
            for( float i = -3.; i < 4.; ++i )
            if( i != 0. )
            {
                float a = 30. * i;
                float b = a + 6. * max( 0., .5 * sin( radians(a) ) );
                for( float side = -1.; side < 3.; side += 2. )
                {
                    vec2 sc = sincospi( b / 180. );
                	vec3 v = M * vec3( sc.y * c15, side * s15, sc.x * c15 ) * g_game.camframe;
                	if( g_vrmode )
                    	v *= g_vrframe;
                	if( v.x > 0. )
                	{
                    	vec2 p = ( coord - project3d( v, g_game.camzoom ) ) * g_textscale;
                    	result += .5 * hmd_chrout( p + 12. * vec2( TXT_FONT_SPACING, .5 ), 12., 48. + round( abs(a) / 10. ) );
                	}
                }
            }

            // neutral-g indicator for orbit HMD
			if( g_vehicle.modes.z == VS_ENG_IMP &&
                g_vehicle.modes.x == VS_HMD_ORB )
            {
                vec3 r = g_vehicle.orbitr;
			    float r2 = dot( r, r );
    			vec3 gr = r2 < square( g_data.radius ) ?
        			- g_data.GM / cube( g_data.radius ) * r :
        			- g_data.GM / ( r2 * sqrt( r2 ) ) * r;
				vec3 movement_omega = cross( g_vehicle.orbitr, g_vehicle.orbitv ) / dot( g_vehicle.orbitr, g_vehicle.orbitr );
        		gr -= cross( movement_omega, cross( movement_omega, g_vehicle.orbitr ) );

                float a_max = FDM_MASS_SCALE * 136000. / 10630.;
                float a = 0.001 * g_vehicle.throttle * a_max;
                vec3 rn = normalize( g_vehicle.orbitr );
                float f = sin( radians( g_vehicle.tvec ) ) * dot( rn, g_vehicle.B[1] );
                float sinpitch = -dot( gr, rn ) / ( a * sqrt( max( 0., 1. - f * f ) ) );
                if( sinpitch < 1. )
                {
                    float line1 = degrees( asin( sinpitch ) ) - g_vehicle.tvec;
                    float line2 = 180. - degrees( asin( sinpitch ) ) - g_vehicle.tvec;
	                float shape =
                        max( aaa_stipple( Kp, pitch + 180. - line1, 360., Kp / ( 360. * g_textscale.y ) ),
                             aaa_stipple( Kp, pitch + 180. - line2, 360., Kp / ( 360. * g_textscale.y ) ) ) *
    	    	    	aaa_interval( Ks, abs( side ) - 5., 5. ) *
                        aaa_stipple( Ks, side, 2., .5 );
                    result += .25 * shape;
                }
            }
        }
    }
    return result;
}

void hmd_night_vision( inout vec3 col, vec2 coord, vec3 cc )
{
    vec2 uv = coord / iResolution.xy;
    if( abs( cc.y ) < HMD_BORDER.x * cc.x && abs( cc.z ) < HMD_BORDER.y * cc.x )
    {
        float relbright = g_game.hudbright * g_exposure.z;
        if( g_vrmode )
            relbright /= g_game.camzoom;
        float sat = COL_NVISNSAT / relbright;
        float gain = min( COL_NVISNGAIN, 1. / ( g_exposure.x * g_exposure.z ) );
        float y = dot( col, irselect( COL_NVISNSENS, ( g_game.switches & GS_IRCAM ) != 0u ) );
        col += COL_P43PHOSPHOR * relbright * sat * y / ( y + sat / gain );
    }
}

void hmd_terrain_radar( inout vec3 col, vec2 coord, vec3 cc )
{
    vec2 uv = ( g_vrmode ? coord + unViewport.xy : coord ) / iResolution.xy;
	if( abs( cc.y ) < HMD_BORDER.x * cc.x && abs( cc.z ) < HMD_BORDER.y * cc.x )
    {
		float center = textureLod( iChannel1, uv, 0. ).w;
    	vec4 sides = vec4(
        	textureLodOffset( iChannel1, uv, 0., ivec2( -1,  0 ) ).w,
			textureLodOffset( iChannel1, uv, 0., ivec2( +1,  0 ) ).w,
			textureLodOffset( iChannel1, uv, 0., ivec2(  0, -1 ) ).w,
			textureLodOffset( iChannel1, uv, 0., ivec2(  0, +1 ) ).w );
    	float mu = dot( sides, vec4(.25) );
    	float laplace = ( center - mu ) * g_game.camzoom;
        float sigma = dot( ( sides - mu ) * ( sides - mu ), vec4(.25) );
        float range = 2. * sqrt( sigma );
		float shape = .1 / ( .4 + mu * exp2pp( 32. * ( laplace / ( 0.0003 + range ) ) ) );
		col += g_hudcolor * shape;
    }
}

void map_position( inout vec3 col, vec2 coord )
{
    float shape = 0.;
    mat2 I = mat2(1);
    vec2 s0 = mc2sc( gs_map_project( g_game, g_vehicle.localr ) );
    float sr = dot( g_vehicle.localB[0], normalize( g_vehicle.localr ) );

    if( abs( sr ) < .9995 )
    {
        // arrow if heading is defined
        vec2 s1 = mc2sc( gs_map_project( g_game, g_vehicle.localr + g_vehicle.localB[0] ) );
        vec2 ds = normalize( s1 - s0 );
        mat2 M = mat2( ds, perp( ds ) );
        vec2 a = s0 + M * vec2( +6, 0 );
        vec2 b = s0 + M * vec2( -6, -4 );
        vec2 c = s0 + M * vec2( -6, +4 );
        shape = max( shape, aaa_line( I, coord, a, b, 1. ) );
        shape = max( shape, aaa_line( I, coord, b, c, 1. ) );
        shape = max( shape, aaa_line( I, coord, c, a, 1. ) );
    }
    else
    if( sr < 0. )
        // cross if downwards
        shape = max( aaa_line( I, coord, s0 - 5., s0 + 5., 1. ),
                     aaa_line( I, coord, s0 + vec2( +5, -5 ), s0 + vec2( -5, +5 ), 1. ) );
    else
        // ring if upwards
        shape = aaa_ring( I, coord - s0, 10., 1. );

	float phase = .25 + .75 * step( .5, fract( iTime ) );
    col += vec3( 1, .5, .0 ) * shape * phase;
}

void map_marker( inout vec3 col, vec2 coord )
{
	mat2 I = mat2(1);
    float shape = 0.;
    vec2 s = mc2sc( gs_map_project( g_game, g_game.mapmarker ) );
    shape = max( shape, aaa_hline( I, coord, s - vec2( 6, 0 ), 12., 1. ) );
    shape = max( shape, aaa_vline( I, coord, s - vec2( 0, 6 ), 12., 1. ) );
    col += vec3( 1, .5, .0 ) * shape;
}

void map_waypoint( inout vec3 col, vec2 coord )
{
	mat2 I = mat2(1);
    float shape = 0.;
    vec2 s = mc2sc( gs_map_project( g_game, g_game.waypoint ) );
    shape = max( shape, aaa_line( I, coord, s, s + vec2( -6, 12 ), 1. ) );
    shape = max( shape, aaa_line( I, coord, s, s + vec2( +6, 12 ), 1. ) );
    col += vec3( 1, .5, .0 ) * shape;
}

void map_orbit_track( inout vec3 col, vec2 sc )
{
    vec3 dpdx = ZERO, dpdy = ZERO;    
    vec4 p = gs_map_unproject_d( g_game, sc, iResolution.xy, dpdx, dpdy );    
    vec4 px = vec4( dpdx.x, dpdy.x, 0, p.x );
    vec4 py = vec4( dpdx.y, dpdy.y, 0, p.y );
    vec4 pz = vec4( dpdx.z, dpdy.z, 0, p.z );    
    vec4 plng = atan2_d( py, px );

    vec3 r = g_vehicle.localr;
    vec3 v = g_vehicle.orbitv * g_planet.B; // not: localv!     
    Kepler K = Kepler( 0., 0., 0., 0., 0. );
    float nu = kepler_init( K, r, v, g_data.GM );  
    float M = cvt_E2M( cvt_nu2E( nu, K.e ), K.e );    
    vec3 h = cross( r, v );
    float invsin_i = length(h) / length( h.xy );
    float invtan_i = h.z / length( h.xy );
    if( K.e < .00005 )
    {
		K.w = asin( clamp( normalize(r).z * invsin_i, -1., 1. ) );
        if( v.z < 0. )
            K.w = PI - K.w;
    }
    
    vec4 dlng = asin_d( clamp_d( div_d( pz, hypot_d( px, py ) ) * invtan_i, -ONE_D, ONE_D ) );
    vec4 dnu = asin_d( clamp_d( pz * invsin_i, -ONE_D, ONE_D ) );        
    float dMdt = sqrt( g_data.GM * cube( abs( 1. - K.e * K.e ) / K.p ) );
    float dphidM = g_planet.omega / dMdt;    
    mat2x4 ll = mat2x4( const_d( K.O ) + dlng, const_d( K.O ) - dlng - const_d(PI) );
    mat2x4 nn = mat2x4( const_d( -K.w ) + dnu, const_d( -K.w ) - dnu - const_d(PI) );
    mat2x4 MM = mat2x4( cvt_E2M_d( cvt_nu2E_d( nn[0], K.e ), K.e ), cvt_E2M_d( cvt_nu2E_d( nn[1], K.e ), K.e ) );
    mat2x4 aa = MM - mat2x4( const_d(M), const_d(M) );   
    vec2 KK = vec2( length( ll[0].xy - plng.xy - aa[0].xy * dphidM ), length( ll[1].xy - plng.xy - aa[1].xy * dphidM ) );
    vec2 JJ = vec2( length( aa[0].xy ), length( aa[1].xy ) );
    
    vec3[3] colors = vec3[3]( 
        vec3( .9, .7, .1 ),
        vec3( .4, .4, .4 ),
        vec3( .1, .1, .1 ) );
    
    float stipple = 15. * dMdt;
    float mask = aaa_interval( dFdy(p.w), p.w, 2. );   
    float revlimit = 3. * TAU;
    
#define w2vec2( _a ) vec2( _a[0].w, _a[1].w )
    
    if( mask >= FRACT_1_64 )
    for( int i = 0; i < ( K.e < .99995 ? 3 : 1 ); ++i )
    {    
		float shape = 0.;
        float k = float(i) * TAU;
        vec2 d = ( K.e < .99995 ? ( mod( w2vec2( aa ), TAU ) + k ) : w2vec2( aa ) ) * dphidM;
    	vec2 u = mod( w2vec2( ll ) - plng.w - d + PI, TAU ) - PI;        
        vec2 s = aaa_interval2( KK, u, KK ) * aaa_stipple2( JJ, w2vec2( aa ), vec2( stipple ), vec2(.5) ) * 
            aaa_step2( JJ, revlimit - d ) * ( K.e < 1. ? vec2(1.) : aaa_step2( KK, w2vec2( aa ) ) );
        col += mask * hmax(s) * colors[ min( i, 3 ) ];
    }
    
#undef w2vec2
}

// ----------------------------------------------------------------------------
// POST PROCESSING
// ----------------------------------------------------------------------------

vec3 lens_lookup( vec2 uv, float k, float lod, float subsample )
{
    uv = .5 * subsample + k * ( .5 * subsample - uv );
    return textureLod( iChannel1, uv, lod ).xyz *
        16. * saturate( uv.x ) * saturate( uv.y ) * saturate( 1. - uv.x ) * saturate( 1. - uv.y );
}

vec3 post_get_image( vec2 uv, float subsample )
{
    float k = min( 2.3, 60. * sqrt( sqrt( g_pixelscale ) ) );
    float sharpen = .25;

    uv *= subsample;
    if( g_vrmode && uv.x >= .5 * subsample )
    	uv.x += .5 - .5 * subsample;

	vec3 col = ZERO;
#if WITH_IMG_GLARE
    if( ( g_game.switches & GS_TRMAP ) == 0u )
    {
    #if WITH_IMG_EXPERIMENTAL_ROD_ACUITY
        float y = max( 0., dot( textureLod( iChannel1, uv, 3. ).xyz - textureLod( iChannel1, uv, 4. ).xyz, VIS_SCOTOPIC_Y ) );
    #endif
	    vec3 wsum = ZERO;
    	for( float i = 0.; i < 10.; ++i )
    	{
            float w = 1. / ( 1. + exp2( k * i ) );
            if( i == 1. )
                w -= sharpen;
   		#if WITH_IMG_EXPERIMENTAL_ROD_ACUITY
			if( i < 3. )
            {
    			float suppress = VIS_LIMITS.z / ( 16384. * y + VIS_LIMITS.z );
                w *= g_exposure.y / ( g_exposure.y + suppress * VIS_LIMITS.x );
            }
        #endif
            col += w * textureLod( iChannel1, uv, i ).xyz;
            wsum += w;
    	}
        col = clamp( col / wsum, 0., 16. );
    }
    else
#endif
    	col = textureLod( iChannel1, uv, 0. ).xyz;

#if WITH_IMG_LENS
    if( ( g_game.switches & GS_TRMAP ) == 0u )
    {
	    float bias = .5 * log2( g_pixelscale );
	    col += .25 * FRACT_1_64 * lens_lookup( uv,-.5, -5. - bias, subsample );
	   	col += .25 * FRACT_1_64 * lens_lookup( uv, .5, -4. - bias, subsample );
	   	col += .25 * FRACT_1_64 * lens_lookup( uv, 1., -5.5 - bias, subsample );
	    col += .25 * FRACT_1_64 * lens_lookup( uv, 2., -7. - bias, subsample );
    }
#endif

    // add absolute threshold of vision
    col = col * 1. + vec3( COL_THRESHOLD );
    return col;
}

void post_sun_glare( inout vec3 col, vec3 raydir )
{
#if WITH_IMG_SUNGLARE
    vec3 sunshadow = IMG_MIPMAP_HIDE * texelFetch( iChannel1, ivec2( 0, 0 ), 0 ).xyz;
    mat3 frame = g_game.camframe;
    if( g_vrmode )
        frame *= g_vrframe;
    float d = dot( normalize( reject( cross( g_env.L, raydir ), frame[0] ) ),
                   normalize( reject( cross( g_env.L, frame[2] ), frame[0] ) ) );
    float s = 1. / ( 1. + 0.985 * chebychev6(d) );
    float a = square( g_planet_data[0].radius ) / dot( g_vehicle.r, g_vehicle.r );
    float b = 1. - square( .81 / g_game.camzoom );
    float c = 1. - square( .71 / g_game.camzoom );
    float offimage = parabolstep( b, c, dot( frame[0], g_env.L ) );
    vec3 tmp = offimage * g_env.sunlight *
        exp2( -24. * sqrt( max( 0., 1. - square( dot( raydir, g_env.L ) ) ) ) ) *
        a * s * sunshadow / max( vec3( a * s / IMG_EXPOSURE_MAX ), 1. - a * sunshadow - dot( raydir, g_env.L ) );
    col += tmp;
#endif
}

void console_throttle_graphics( inout vec3 col, vec2 coord )
{
    mat2 I = mat2(1);
    float t = g_vehicle.throttle;
    vec2 size = vec2( 15, 60. * abs(t) );
    if( Linfinity( coord ) < 61. )
    {
	    float shape = aaa_rect( I, coord - size / 2., size, vec2(1) );
	    if( t >= 0. )
	        shape = max( shape, aaa_box( I, coord - size / 2., size, vec2(1) ) * .25 );
	    col += g_hudcolor * shape;
    }
}

void post_console_overlay( inout vec3 col, vec2 coord )
{
    vec2 uv = mix( vec2( iResolution.x, 0 ), coord, g_textscale ) / iResolution.xy;
    coord = ( coord - g_overlayframe.xy ) * g_textscale;
    if( !g_vrmode )
    {
        float consolemask = uv.y * iResolution.y - 24.;
        if( ( g_game.switches & GS_IPAGE_MASK ) != 0u )
        {
            vec2 b = ( uv.xy * iResolution.xy - vec2( iResolution.x - 136., 80 ) ) * vec2( -1, 1 );
            float c = max( b.x + b.y, hmax( b ) );
            consolemask = clamp( consolemask, 0., 16. ) + clamp( c, 0., 16. ) - 16.;
        }
        col *= mix( .5, 1., saturate( consolemask / 16. ) );
    }
 	console_throttle_graphics( col, coord - vec2( 16, 12 ) );
}

void post_hmd_overlay( inout vec3 col, vec2 coord, vec3 cc )
{
    col += g_hudcolor * hmd_center_dot( coord );
    col += g_hudcolor * hmd_waterline( coord );
    col += g_hudcolor * hmd_flight_path_marker( coord );
   	col += g_hudcolor * hmd_pitch_ladder( coord, cc );
    if( dot( g_game.waypoint, g_game.waypoint ) > 0. )
        col += g_hudcolor * hmd_waypoint( coord );
}

void post_map_overlay( inout vec3 col, vec2 coord )
{
    coord = ( coord - g_overlayframe.xy ) * iResolution.xy / g_overlayframe.zw;
	map_position( col, coord );
    if( dot( g_game.mapmarker, g_game.mapmarker ) > 0. )
    	map_marker( col, coord );
    if( dot( g_game.waypoint, g_game.waypoint ) > 0. )
    	map_waypoint( col, coord );
    if( g_env.H == 0. ) 
    	map_orbit_track( col, coord );
}

void post_text_overlay( inout vec3 col, vec2 coord, vec3 cc )
{
    coord = ( coord - g_overlayframe.xy ) * g_textscale;
	for( int i = 0; i < TXT_FMT_MAX_COUNT; ++i )
		col += g_hudcolor * hmd_txtout( coord, cc, i );
}

void post_overlay( inout vec3 col, vec2 coord, vec3 cc )
{
    bool mapmode = ( g_game.switches & GS_TRMAP ) == GS_TRMAP;
	if( g_game.stage == GS_RUNNING )
    {
    	if( !mapmode )
        {
	    	if( ( g_game.switches & GS_NVISN ) == GS_NVISN )
    	    	hmd_night_vision( col, coord, cc );
    		if( ( g_game.switches & GS_TRDAR ) == GS_TRDAR )
        		hmd_terrain_radar( col, coord, cc );
            if( g_vehicle.modes.x > 0 )
				post_hmd_overlay( col, coord, cc );
        	post_console_overlay( col, coord );
    	}
        else
			post_map_overlay( col, coord );
    }
    post_text_overlay( col, coord, cc );
}

void post_exposure( inout vec3 col )
{
#if WITH_IMG_EXPOSURE
  #if WITH_IMG_RODVISION
    float y = dot( col, VIS_SCOTOPIC_Y );
    float suppress = VIS_LIMITS.z / ( y + VIS_LIMITS.z );
    col = g_exposure.z * col + g_exposure.w * suppress * y * .25 * COL_RODVISION;
  #else
	col = g_exposure.z * col;
  #endif
#endif
}

void post_balance( inout vec3 col )
{
#if WITH_IMG_BALANCE
    col *= IMG_BALANCE_ADAPT / hmax( IMG_PRIMARIES * IMG_BALANCE_ADAPT );
#endif
}

void post_saturate( inout vec3 col )
{
#if WITH_IMG_SOFT_SATURATE == 1
    col = 1. - exp( -col * 1.1025 );
#elif WITH_IMG_SOFT_SATURATE == 2
    col = 2. / ( 1. + exp( -col * 2.0220 ) ) - 1.;
#elif WITH_IMG_SOFT_SATURATE == 3
    col = sin( min( col * 1.0055, PI / 2. ) );
#else
    col = saturate( col );
#endif
}

void post_primaries( inout vec3 col )
{
#if WITH_IMG_PRIMARIES
    col = max( ZERO, IMG_PRIMARIES * col );
#endif
}

void post_filmic( inout vec3 col )
{
#if WITH_IMG_FILMIC
    float b = ( g_vrmode ? IMG_FILMIC_VR : IMG_FILMIC ) * min( 1.0, IMG_FILMIC_EXPOSURE_REF * g_exposure.y );
    float c = 0.166666667 * ( sqrt( 200. * ( b * b + b ) + 9. ) + 3. ) / ( b + 1. );
    vec3 d = col * c;
    col = ( 1. + b ) * d * d / ( b + d );
#endif
}

// ----------------------------------------------------------------------------
// MAIN
// ----------------------------------------------------------------------------

vec3 eotf( vec3 arg )
{
    return g_vrmode ?
        pow( arg, vec3( IMG_GAMMA_VR ) ) :
#if WITH_IMG_SRGB_EOTF
        mix( arg / 12.92, pow( ( arg + .055 ) / 1.055, vec3( 2.4 ) ), lessThan( vec3( .04045 ), arg ) );
#else
    	pow( arg, vec3( IMG_GAMMA ) );
#endif
}

vec3 oetf( vec3 arg )
{
    return g_vrmode ?
    	pow( arg, vec3( 1. / IMG_GAMMA_VR ) ) :
#if WITH_IMG_SRGB_EOTF
        mix( 12.92 * arg, 1.055 * pow( arg, vec3( .416667 ) ) - .055, lessThan( vec3( .0031308 ), arg ) );
#else
    	pow( arg, vec3( 1. / IMG_GAMMA ) );
#endif
}

vec3 dither( vec3 arg, vec3 noise, float quant )
{
    vec3 c0 = floor( oetf( arg ) / quant ) * quant;
    vec3 c1 = c0 + IMG_QUANTIZE;
    vec3 discr = mix( eotf( c0 ), eotf( c1 ), noise );
    return mix( c0, c1, lessThan( discr, arg ) );
}

void main_image_worker( out vec4 fcolor, in vec2 fcoord )
{
    fcolor = vec4( ZERO, 1 );

#if BUFFER_RUNLEVEL >= 5

    g_game = gs_load( iChannel0, ADDR_GAMESTATE );
    g_data = g_planet_data[1];
	bool mapmode = ( g_game.switches & GS_TRMAP ) == GS_TRMAP;
    float subsample = mapmode ? 1. : SCN_SUBSAMPLE_INV;

    if( fcoord.y >= 2. * subsample && fcoord.y < iResolution.y - 2. * subsample )
    {
        g_vehicle = vs_load( iChannel0, ADDR_VEHICLE );
        g_planet = ps_load( iChannel0, ADDR_PLANETS + ivec2( 1, 0 ) );
        g_env = env_load( iChannel0, ADDR_LOCALENV );
        g_exposure.xy = g_game.exposure;
        g_exposure.zw = pow( g_game.exposure + VIS_LIMITS.xy, vec2( g_vrmode ? VIS_EXPONENT_VR : VIS_EXPONENT ) );
        g_hudcolor = ( mapmode ? 1. : g_game.hudbright ) * COL_P43PHOSPHOR;
        if( !mapmode && g_vrmode )
        	g_hudcolor /= g_game.camzoom;

        vec2 uv = fcoord / iResolution.xy;
		vec2 sc = 2. * uv - 1.;
   	 	vec2 ec = sc * vec2( 1, iResolution.y / iResolution.x );
        vec3 cc = ZERO;
        if( g_vrmode )
        {
            cc = g_vrdir * g_vrframe;
         	if( !mapmode && dot( cc.yz, cc.yz ) >= 1.5 / g_game.camzoom * cc.x * cc.x )
            	return;
			cc.yz /= g_game.camzoom;
            cc = normalize( cc );
		    g_raydir = g_game.camframe * g_vrframe * cc;
        }
        else
        {
            cc = normalize( vec3( CAM_FOCUS, barrel_distort( vec2( ec.x, -ec.y ) / g_game.camzoom, CAM_DISTORT ) ) );
        	g_raydir = g_game.camframe * cc;
        }
       	g_pixelscale = .25 * abs( cc.x * dFdx( cc.y / cc.x ) * dFdy( cc.z / cc.x ) );
	    g_Kr = mat2x3( dFdx( g_raydir ), dFdy( g_raydir ) );

        vec3 col = post_get_image( uv, subsample );

        //*
        if( ( g_game.switches & GS_CHEES ) == 0u )
        {
            g_overlayframe = vec4( 0, 0, iResolution.xy );
            if( g_vrmode )
            {
                float z = mapmode ? 1. : g_game.camzoom;
            	g_overlayframe.xy = project3d( vec3( 1.35, -1, +iResolution.y / iResolution.x ), z );
                g_overlayframe.zw = project3d( vec3( 1.35, +1, -iResolution.y / iResolution.x ), z ) - g_overlayframe.xy;
            }
            g_textscale = texelFetch( iChannel1, ivec2( 2, 0 ), 0 ).xy * IMG_MIPMAP_HIDE / g_overlayframe.zw;
    		g_textlodbias = log2( max( g_textscale.x, g_textscale.y ) );
        	post_overlay( col, g_vrmode ? fcoord - unViewport.xy : fcoord, cc );
        }

        if( !mapmode )
        {
        	post_sun_glare( col, g_raydir );
            col *= VIS_PRE_EXPOSURE;
        	post_exposure( col );
            col *= VIS_POST_EXPOSURE;
			post_filmic( col );
        }

        post_saturate( col );
        post_balance( col );
        post_primaries( col );
		//*/

        // black level, noise shaping and display gamma
		vec3 noise = .5/65536. +
            		 texelFetch( iChannel3, ivec2( fcoord / 8. ) & ( int( iChannelResolution[3] ) - 1 ), 0 ).xyz * 255./65536. +
            		 texelFetch( iChannel3, ivec2( fcoord )      & ( int( iChannelResolution[3] ) - 1 ), 0 ).xyz * 255./256.;

        col = mix( col, ONE, eotf( vec3( g_vrmode ? IMG_BLACKLEVEL_VR : IMG_BLACKLEVEL ) ) );
        fcolor.xyz = dither( col, noise, IMG_QUANTIZE );
    }
#endif // RUNLEVEL
}

void mainImage( out vec4 fcolor, in vec2 fcoord )
	{ main_image_worker( fcolor, fcoord ); }

void mainVR( out vec4 fcolor, in vec2 fcoord, in vec3 _ro_dummy_, in vec3 _rd_dummy_ )
{
    g_vrmode = true;
    vec3 horz = ( unCorners[1] + unCorners[2] - unCorners[0] - unCorners[3] ).zxy * vec3( -1, 1, -1 );
    vec3 down = ( unCorners[0] + unCorners[1] - unCorners[2] - unCorners[3] ).zxy * vec3( -1, 1, -1 );
    vec3 forw = ( unCorners[0] + unCorners[1] + unCorners[2] + unCorners[3] - 4. * unCorners[4] ).zxy * vec3( -1, 1, -1 );
    g_vrframe[1] = normalize( horz );
    g_vrframe[2] = normalize( down );
    g_vrframe[0] = cross( g_vrframe[1], g_vrframe[2] );
    vec3 cent = g_vrframe[0] * dot( forw, g_vrframe[0] ) - 2. * ( unCorners[0] - unCorners[4] ).zxy * vec3( -1, 1, -1 );
    g_vrfocus.xy = vec2( dot( cent, g_vrframe[1] ) / dot( horz, g_vrframe[1] ), dot( cent, g_vrframe[2] ) / dot( -down, g_vrframe[2] ) );
    g_vrfocus.zw = dot( forw, g_vrframe[0] ) / vec2( dot( horz, g_vrframe[1] ), dot( down, g_vrframe[2] ) );
    g_vrcoord = ( gl_FragCoord.xy - unViewport.xy ) / unViewport.zw;
    g_vrdir = normalize( mix( mix( unCorners[0], unCorners[1], g_vrcoord.x ),
                              mix( unCorners[3], unCorners[2], g_vrcoord.x ), g_vrcoord.y ) - unCorners[4] ).zxy * vec3( -1, 1, -1 );
    main_image_worker( fcolor, gl_FragCoord.xy );
}

#define unViewport _unViewport_dummy_
#define unCorners _unCorners_dummy_
