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
uniform sampler2D iChannel1;             // for texture data

#define RADIUS 0.075
#define PI 3.14159

float Paddle(vec2 pa, vec2 ba){
	float t=clamp(dot(pa,ba)/dot(ba,ba),0.0,1.0);
	return length(pa-ba*t)+t*0.01;
}

float Tube(vec2 pa, vec2 ba){
    return length(pa-ba*clamp(dot(pa,ba)/dot(ba,ba),0.0,1.0));
}

float DE2(in vec2 p){//2d version
	float y=p.y*0.12;
    vec2 pdl = texture(iChannel0, vec2(0.5, 0.0)/iResolution.xy).xy; // get paddle data
    vec2 pdr = texture(iChannel0, vec2(1.5, 0.0)/iResolution.xy).xy;
    
    float dP=min(Paddle(p-vec2(-0.5,-1.0),pdl),Paddle(p-vec2(0.5,-1.0),pdr));
	p.x=abs(p.x);
	float d=min(max(p.x-1.0,abs(p.y)-1.55),length(p-vec2(0.0,1.5))-1.0);
	p.y=abs(p.y);
	d=min(abs(d),Tube(p-vec2(0.47-y,0.95),vec2(0.32,-0.12)));
	p.y=abs(p.y-1.0);
	d=min(d,Tube(p-vec2(0.26,1.0),vec2(0.15,-0.14)));
	float dB=length(p-vec2(0.74-y,0.79));
	d=min(min(d,dB),dP);
	d-=0.05;
	return d;
}

vec3 Color(in vec3 p0){
	vec2 p=p0.xz;
	float y=p.y*0.12;
    vec2 pdl = texture(iChannel0, vec2(0.5, 0.0)/iResolution.xy).xy; // get paddle data
    vec2 pdr = texture(iChannel0, vec2(1.5, 0.0)/iResolution.xy).xy;
    
	float dP=min(Paddle(p-vec2(-0.5,-1.0),pdl),Paddle(p-vec2(0.5,-1.0),pdr));
	p.x=abs(p.x);
    float r=length(p-vec2(0.0,1.5))-1.0;
	float d=min(max(p.x-1.0,abs(p.y)-1.55),r);
	p.y=abs(p.y);
	d=min(abs(d),Tube(p-vec2(0.47-y,0.95),vec2(0.32,-0.12)));
	p.y=abs(p.y-1.0);
	d=min(d,Tube(p-vec2(0.26,1.0),vec2(0.15,-0.14)));
	float dB=length(p-vec2(0.74-y,0.79));
	if(dP<d && dP<dB)return vec3(1.0,0.9,0.8);
	vec3 col=vec3(0.5);
	if(dB<d){
		col+=0.4*sin(p0.zxx*9.0)+sin(p0.y*150.0)*0.1;
	}else {
		if(p0.y>1.0)col=vec3(0.4,0.6,0.7);
		if(p0.y<-0.5)col=vec3(0.7,0.6,0.5);
		if(p0.y>0.4)col.b+=0.3;
        vec3 h=fract(p0*20.0)-0.5;
        col+=vec3((h.x*h.z+h.y)*0.1+p0.y*2.0)+smoothstep(0.05,0.04,abs(p0.y))*0.3;
        col=mix(col,vec3(1.0),smoothstep(0.0,0.005,max(p.x-0.95,min(p0.z-1.5,r+0.05))));
	}
	return col+sin(p0.xyy*3.0)*0.1;
}

float RoundedIntersection(float a, float b, float r) {//modified from Mercury SDF http://mercury.sexy/hg_sdf/
	return max(max(a,b),length(max(vec2(0.0),vec2(r+a,r+b)))-r);
}

float DE(in vec3 p){//another pinball
	return RoundedIntersection(DE2(p.xz),p.y-RADIUS,0.02);
}

float Sphere( in vec3 ro, in vec3 rd, in vec3 p, in float r)
{//intersect a sphere - based on iq's
	float t=100.0;
	p=ro-p;
	float b=dot(p,rd);
	float h=b*b-dot(p,p)+r*r;
	if(h>=0.0){
		t=-b-sqrt(h);
		if(t>0.0)return t;
	}
	return 10.0;
}

vec2 rotate(vec2 v, float angle) {
    return cos(angle)*v+sin(angle)*vec2(v.y,-v.x);
}

vec3 scene(vec3 ro, vec3 rd){
    // Get ball position from buffer
    vec4 ballData = texture(iChannel0, vec2(0.5, 0.0)/iResolution.xy);
    vec2 bp = ballData.xy; // ball position
    
    // Get paddle rotation from buffer
    vec4 paddleData = texture(iChannel0, vec2(0.0, 0.0)/iResolution.xy);
    vec2 pdl_rot = vec2(0.33, 0.0);
    vec2 pdr_rot = vec2(-0.33, 0.0);
    if(iFrame > 0) { // Not on first frame
        pdl_rot = rotate(vec2(0.33, 0.0), paddleData.x);
        pdr_rot = rotate(vec2(-0.33, 0.0), paddleData.y);
    }
    
	float maxt=(-RADIUS+0.005-ro.y)/rd.y;
	float tball=Sphere(ro,rd,vec3(bp.x,0.0,bp.y),RADIUS);
	float t=(RADIUS+0.005-ro.y)/rd.y,d,od=1.0;
	for(int i=0;i<24;i++){
		t+=d=DE(ro+rd*t);
		if(d<0.001 || t>maxt)break;
		od=d;
	}
	vec3 col=vec3(0.0);
	vec2 p=ro.xz+rd.xz*maxt;
	float d2=min(max(abs(p.x)-1.0,abs(p.y)-1.55),length(p-vec2(0.0,1.5))-1.0);
	if(d2<0.0){
		vec3 P=ro+rd*maxt+vec3(-0.03,0.0,0.03);
		d2=DE(P);
		d2=min(d2,length(P-vec3(bp.x,-RADIUS+0.005,bp.y)));
		d2=smoothstep(0.0,0.1,d2);
        // Use a default color instead of texture lookup
        col=vec3(0.7, 0.6, 0.5)*(0.7+0.3*d2)+abs(sin(floor(p.x*20.0)))*0.2*d2;
	}
	if(d<0.01){
		float dif=clamp(1.0-d/od,0.6,1.0);
		col=mix(Color(ro+rd*t)*dif,col,smoothstep(0.0,0.01,d));
	}
	if(tball<t){
		t=tball;
		vec3 N=normalize(ro+rd*tball-vec3(bp.x,0.0,bp.y));
		d=dot(N,-rd);
		float dif=max(0.0,d);
		col=mix(col,vec3(1.0,1.0,0.7)*dif,smoothstep(0.0,0.56,d));
	}
    // Using a simpler light effect instead of st2.x
    float lightEffect = 0.0;
    if(iMouse.x > 0.0) lightEffect = 1.0;
    if(lightEffect > 0.0){
        float mld = length(ro.xz - vec2(0.74, 0.79)); // Simulated closest pin
        col+=lightEffect*vec3(1.0)/(0.5+1000.0*mld*mld);
    }
	return col;
}

float segment(vec2 uv){//from Andre https://www.shadertoy.com/view/Xsy3zG
	uv = abs(uv);return (1.0-smoothstep(0.0,0.28,uv.x)) * (1.0-smoothstep(0.4-0.14,0.4+0.14,uv.y+uv.x));
}

float sevenSegment(vec2 uv,int num){
	uv=(uv-0.5)*vec2(1.5,2.2);
    float seg=0.0;
    if(num>=2 && num!=7 || num==-2)seg+=segment(uv.yx);
    if (num==0 || (uv.y<0.?((num==2)==(uv.x<0.) || num==6 || num==8):(uv.x>0.?(num!=5 && num!=6):(num>=4 && num!=7) )))seg += segment(abs(uv)-0.5);
    if (num>=0 && num!=1 && num!=4 && (num!=7 || uv.y>0.))seg += segment(vec2(abs(uv.y)-1.0,uv.x));
    return seg;
}

//prints a "num" filling the "rect" with "spaces" # of digits including minus sign
float formatInt(vec2 uv, vec2 rect, float num, int spaces){//only good up to 6 spaces!
	uv/=rect;if(uv.x<0.0 || uv.y<0.0 || uv.x>1.0 || uv.y>1.0)return 0.0;
	uv.x*=float(spaces);
	float place=floor(uv.x);
	float decpnt=floor(max(log(num)/log(10.0),0.0));//how many digits before the decimal place
	uv.x=fract(uv.x);
	num+=0.000001*pow(10.,decpnt);
	num /= pow(10.,float(spaces)-place-1.0);
	num = mod(floor(num),10.0);
	return sevenSegment(uv,int(num));
}

mat3 lookat(vec3 fw){
	fw=normalize(fw);vec3 rt=normalize(cross(fw,vec3(0.0,1.0,0.0)));return mat3(rt,cross(rt,fw),fw);
}

out vec4 FragColor;

void main()
{
    // Get the current texture coordinates
    vec2 uv = gl_FragCoord.xy;
    
    // Set up camera
    vec4 st0 = texture(iChannel0, vec2(0.0, 0.0)/iResolution.xy); // paddle and camera data
    vec4 st1 = texture(iChannel0, vec2(0.5, 0.0)/iResolution.xy); // ball data
    // st2 would contain score info but simplified for this conversion
    
    // Get ball position
    vec2 bp = st1.xy;
    
    // Get paddle rotations
    vec2 pdl = rotate(vec2(0.33, 0.0), st0.x);
    vec2 pdr = rotate(vec2(-0.33, 0.0), st0.y);
    
    vec3 ro=vec3(0.0,2.5,-2.5);
    vec3 rd=vec3((uv-0.5*iResolution.xy)/iResolution.x,1.0);
    rd=lookat(vec3(st0.z*0.05,0.0,st0.w*0.2)-ro)*normalize(rd);
    
    FragColor=vec4(scene(ro,rd),1.0);
    
    // Add UI elements (score display)
    vec2 coord=uv/iResolution.xy;
    float d=formatInt(coord-vec2(0.1,0.9), vec2(0.1,0.05), floor(1234.0), 5); // Using fixed score for this conversion
    d+=formatInt(coord-vec2(0.85,0.9),vec2(0.02,0.05), floor(3.0)+1.0, 1); // Using fixed lives for this conversion
    FragColor=mix(FragColor,vec4(1.0,0.0,0.0,1.0),d);
}