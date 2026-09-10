/** Boulder Dash - https://www.shadertoy.com/view/MstXzN
	License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License.
	(cc) 2016, Stefan Berke

	Move around, collect gems, don't get hurt or trapped.
*/

// tiles
#define EMPTY 0
#define SAND 1
#define WALL 2
#define STONE 3
#define DIAMOND 4
#define PLAYER 5

// tile states
#define S_REST 0
#define S_FALL 1
#define S_ROLL_LEFT 2
#define S_ROLL_RIGHT 3
#define S_LANDED 4
#define S_UP 5
#define S_DOWN 6
#define S_LEFT 7
#define S_RIGHT 8
#define S_PUSH_LEFT 9
#define S_PUSH_RIGHT 10
#define S_SMASHED 11
#define S_COLLECTED 12

#define TILE(pix) int(pix.x+.5)
#define STATE(pix) int(pix.y+.5)
#define TEXMOD(pix) pix.z

#define SET_TILE(pix, tile) pix.x = float(tile)
#define SET_STATE(pix, tile) pix.y = float(tile)
#define SET_TEXMOD(pix, tile) pix.z = float(tile)

// game state
#define storeVal(idx_, vec4_) if (int(fragCoord.x) == int(idx_)) fragColor = vec4_;
vec4 value(in int idx) { return texture(iChannel0, vec2(float(idx)+.5,.5)/iChannelResolution[0].xy); }
#define V_PLAYER_POS 0
#define V_CAMERA_CENTER 1
#define V_CAMERA_MOD 2
#define V_SMASHED 3
#define V_GEMS 10

bool key(in int k) { return texture(iChannel1, vec2(float(k)+.5,1.5)/vec2(256.,3.)).x > .5; }
#define K_LEFT 37
#define K_UP 38
#define K_RIGHT 39
#define K_DOWN 40
#define K_R 82

// return map data, wraps around, 'invents' wall at x|y==0 
vec4 levelData(in ivec2 pos)
{
    vec2 fpos = mod(vec2(pos), iChannelResolution[0].xy);
    vec4 t = texture(iChannel0, ((fpos + .5) / iChannelResolution[0].xy));
    return fpos.x < 1. || fpos.y < 1. 
        ? vec4(WALL, S_REST, 0, 0)//vec4(floor(hash1(vec2(pos))*1.+.5)*2., 0., 0., 0.)
        : t;
}


// rather creates patterns than strict randomness 
// change input texture for other patterns
float hash1(in vec2 p)
{
    p = fract(p * vec2(6.171, 5.1213));
    vec4 t = texture(iChannel3, p / iChannelResolution[3].xy);
    p.xy += t.xy;
    return fract(p.x * p.y * 35.937 * (1.+t.z));
}


// creates a new map
void createMap(out vec4 fragColor, in vec2 fragCoord)
{
    vec4 self = vec4(0.);//levelData(ivec2(fragCoord));
    
    vec2 seed = fragCoord + sin(iDate.zw);
    
    // sand and walls
    SET_TILE(self, floor(hash1(seed) * 2. + .4));
    
    // stones and diamonds
    if (TILE(self) != WALL && hash1(seed*1.13) < .3)
        SET_TILE(self, 2.+floor(hash1(seed*1.31) * 2. + .7) );
        
    if (ivec2(fragCoord) == ivec2(30,20))
    {
        SET_TILE(self, PLAYER);
    }
    
	SET_TEXMOD(self, hash1(seed*3.11+.1)*10.);
        
    fragColor = self;
}

bool canPlayerEnter(in int tile) { return tile == EMPTY || tile == SAND || tile == DIAMOND; }

// --- process all map physics ---
/* It's a bit tricky, 
   basically a move action is split into three parts:
	1) set request via SET_STATE
	2) fullfill request of neighbour cells
    3) clear self when fullfilled
   This takes 2 frames and the order of things is important and there 
   are still a few ladybugs in this code. 
   Especially: player and non-player objects are updated at different intervals which
   seems problematic, for example, missed player SMASH or missing clears leading to 
   creation of new objects out of nothing.
   Btw. The player is part of the map and is also tracked by V_PLAYER_POS. 
   The map would process multiple players, but the S_SMASHED and S_COLLECTED states are
   only tracked for the one player in V_PLAYER_POS
*/
void processMap(inout vec4 fragColor, in vec2 fragCoord, 
                bool procPlayer, bool procFall)
{    
    ivec2 levelPos = ivec2(fragCoord);
    if (levelPos.x < 1 || levelPos.y < 1)
    	return;
   
    vec4 self =  levelData(levelPos);
	vec4 topl =  levelData(levelPos+ivec2(-1, 1));
	vec4 top =   levelData(levelPos+ivec2( 0, 1));
	vec4 topr =  levelData(levelPos+ivec2( 1, 1));
	vec4 botl =  levelData(levelPos+ivec2(-1,-1));
	vec4 bot =   levelData(levelPos+ivec2( 0,-1));
	vec4 botr =  levelData(levelPos+ivec2( 1,-1));
	vec4 left =  levelData(levelPos+ivec2(-1, 0));
	vec4 right = levelData(levelPos+ivec2( 1, 0));

    // clear previous gem-collection flag
    if (STATE(self) == S_COLLECTED)
        SET_STATE(self, S_REST);
    
    // process player move requests
    bool isDia = TILE(self) == DIAMOND;
    if (canPlayerEnter(TILE(self)) || isDia)
    {
        int newState = isDia ? S_COLLECTED : S_REST;
	    	 if (TILE(bot) == PLAYER && STATE(bot) == S_UP) { self = bot; SET_STATE(self, newState); }
	    else if (TILE(top) == PLAYER && STATE(top) == S_DOWN) { self = top; SET_STATE(self, newState); }
	    else if (TILE(left) == PLAYER && STATE(left) == S_RIGHT) { self = left; SET_STATE(self, newState); }
	    else if (TILE(right) == PLAYER && STATE(right) == S_LEFT) { self = right; SET_STATE(self, newState); }
    }
	else 
    // clear self when player move requests succeed
    if (   (STATE(self) == S_UP && canPlayerEnter(TILE(top)))
        || (STATE(self) == S_DOWN && canPlayerEnter(TILE(bot)))
        || (STATE(self) == S_LEFT && canPlayerEnter(TILE(left)))
        || (STATE(self) == S_RIGHT && canPlayerEnter(TILE(right))))
    {
        self = vec4(0.);
    }

    if (TILE(self) == PLAYER && procPlayer)
    {
        // fall on head
		if (STATE(top) == S_LANDED)
        { 
            SET_STATE(self, S_SMASHED); 
            SET_TEXMOD(self, S_SMASHED);// helper for anim
        }

		// store player move request
        if (key(K_UP)) { SET_TEXMOD(self, S_UP); if (canPlayerEnter(TILE(top))) { SET_STATE(self, S_UP); } }
        if (key(K_DOWN)) { SET_TEXMOD(self, S_DOWN); if (canPlayerEnter(TILE(bot))) { SET_STATE(self, S_DOWN); } }
        if (key(K_LEFT)) { SET_TEXMOD(self, S_LEFT); if (canPlayerEnter(TILE(left))) { SET_STATE(self, S_LEFT); } }
        if (key(K_RIGHT)) { SET_TEXMOD(self, S_RIGHT); if (canPlayerEnter(TILE(right))) { SET_STATE(self, S_RIGHT); } }
    }
    
    
    // process stone push
    if (TILE(self) == EMPTY)
    {
        if (TILE(right) == STONE && STATE(right) == S_PUSH_LEFT) { self = right; SET_STATE(self, S_REST); }
        else 
        if (TILE(left) == STONE && STATE(left) == S_PUSH_RIGHT) { self = left; SET_STATE(self, S_REST); }
    }
    
    // clear stone after push request succeeded
    if (TILE(self) == STONE)
    {
        if (   (STATE(self) == S_PUSH_LEFT && TILE(left) == EMPTY)
            || (STATE(self) == S_PUSH_RIGHT && TILE(right) == EMPTY) )
            self = vec4(0.);
    }
    
    // store stone push requests
    if (TILE(self) == STONE)
    {
		if (TILE(right) == PLAYER && key(K_LEFT)) { SET_STATE(self, S_PUSH_LEFT); }
		if (TILE(left) == PLAYER && key(K_RIGHT)) { SET_STATE(self, S_PUSH_RIGHT); }
    }
    
    if (procFall)
    {
        if (STATE(self) == S_LANDED)
            SET_STATE(self, S_REST);
        
        // clear self when fall requests succeed
        if (   (STATE(self) == S_FALL && TILE(bot) == EMPTY)
            || (STATE(self) == S_ROLL_LEFT && TILE(botl) == EMPTY)
            || (STATE(self) == S_ROLL_RIGHT && TILE(botr) == EMPTY))
        {
            self = vec4(0.);
        }

        // process stuff that wants to enter this tile
        if (TILE(self) == EMPTY)
        {
            // falling states of maptiles above
            // (remember if it has fallen)
                 if (STATE(top) == S_FALL) { self = top; SET_STATE(self, S_LANDED); }
            else if (STATE(topl) == S_ROLL_RIGHT) { self = topl; SET_STATE(self, S_LANDED); }
            else if (STATE(topr) == S_ROLL_LEFT) { self = topr; SET_STATE(self, S_LANDED); }
        }
        

        // create fall requests for stones and diamonds
        if (TILE(self) == STONE || TILE(self) == DIAMOND)
        {
            // fall down
            if (TILE(bot) == EMPTY)
            {
                SET_STATE(self, S_FALL);
            } 
            // roll to side
            else if (TILE(botl) == EMPTY && TILE(left) == EMPTY)
            {
                SET_STATE(self, S_ROLL_LEFT);
            }
            else if (TILE(botr) == EMPTY && TILE(right) == EMPTY)
            {
                SET_STATE(self, S_ROLL_RIGHT);
            }
            else
            // clear fall/roll flag
            // (but keep for one frame, to hit player)
                if (STATE(self) != S_LANDED)
                	SET_STATE(self, S_REST);
        }
    }
    
    
    fragColor = self;
}


void initStates(inout vec4 fragColor, in vec2 fragCoord)
{
    if (int(fragCoord.y) != 0)
        return;
    fragColor = vec4(0.);
    
    storeVal(V_PLAYER_POS, vec4(30,20,0,0));
    storeVal(V_CAMERA_CENTER, vec4(30,20,0,0));
    storeVal(V_CAMERA_MOD, vec4(1,0,0,0));
}

// step the game states
void processStates(inout vec4 fragColor, in vec2 fragCoord)
{
    if (fragCoord.y >= 1.)
        return;
    
    float tdelta = 1./60.;
    
    // track player movement
    ivec2 ppos = ivec2(value(V_PLAYER_POS).xy);
    vec4 player = levelData(ppos);
    if (TILE(player) != PLAYER)
    {
        ivec2 ppos2,
        ppos1 = ppos+ivec2(1,0); if (TILE(levelData(ppos1)) == PLAYER)
        	{ storeVal(V_PLAYER_POS, vec4(ppos1,0,0)); ppos2 = ppos1; }
        ppos1 = ppos+ivec2(-1,0); if (TILE(levelData(ppos1)) == PLAYER)
        	{ storeVal(V_PLAYER_POS, vec4(ppos1,0,0)); ppos2 = ppos1; }
        ppos1 = ppos+ivec2(0,1); if (TILE(levelData(ppos1)) == PLAYER)
        	{ storeVal(V_PLAYER_POS, vec4(ppos1,0,0)); ppos2 = ppos1; }
        ppos1 = ppos+ivec2(0,-1); if (TILE(levelData(ppos1)) == PLAYER)
        	{ storeVal(V_PLAYER_POS, vec4(ppos1,0,0)); ppos2 = ppos1; }
    
		player = levelData(ppos2);    
        
    } 
    // track actions to player
    {
        if (STATE(player) == S_COLLECTED)
	        { storeVal(V_GEMS, value(V_GEMS) + vec4(1)); }

        if (STATE(player) == S_SMASHED)
       	{ 
            float v = value(V_SMASHED).x;
            storeVal(V_SMASHED, vec4(v + tdelta*(1.-v))); 
        }
    }
    
    // camera follows player position
    vec2 campos = value(V_CAMERA_CENTER).xy;
    vec2 cdelta = value(V_PLAYER_POS).xy - campos;
    storeVal(V_CAMERA_CENTER, vec4(campos + tdelta * cdelta, 0., 0.));
    
    // camera gimmicks
    vec4 camMod = value(V_CAMERA_MOD);
    camMod += vec4(.01*sin(iTime*0.2), .03*sin(iTime/7.7), 0,0);
    camMod.x += 0.02 * dot(cdelta, cdelta) * (.3 - camMod.x);
    camMod.x += 0.1 * (1. - camMod.x);
    storeVal(V_CAMERA_MOD, mix(value(V_CAMERA_MOD), camMod, tdelta));
}

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    if (iFrame < 1 || key(K_R))
    {
    	createMap(fragColor, fragCoord);
        initStates(fragColor, fragCoord);
    }
    else
    {
        fragColor = texture(iChannel0, (fragCoord)/iChannelResolution[0].xy);

        processStates(fragColor, fragCoord);
        processMap(fragColor, fragCoord, 
                   value(V_SMASHED).x < .5,
                   int(mod(float(iFrame), 10.)) == 0);
    }           
}