#define VERTEX_MODE 65
#define EDGE_MODE 83
#define PIN_MODE 68
#define RESET 82
#define WAIT 30
#define STIFF 100. //1=rigid 1000=soft
#define Mouse (iMouse.xy/R.y-R.xy/R.y/2.)
#define R iResolution
#define A(x) texelFetch(iChannel0,ivec2(x,0),0)
#define B(y) texelFetch(iChannel0,ivec2(y,1),0)  
#define V(y) texelFetch(iChannel0,ivec2(y,2),0)  
#define keyDown(k) (texelFetch(iChannel1, ivec2(k,0),0).x>.5)
#define editMode (texelFetch(iChannel1,ivec2(32,2),0).x<=0.)

float line( in vec2 p, in vec2 a, in vec2 b )
{
    vec2 pa = p-a, ba = b-a;   
    float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
    return length( pa - ba*h );
}

void drawChar(sampler2D ch, inout vec3 color, in vec3 charColor, in vec2 p, in vec2 pos, in vec2 size, in int char) {
    p = (p - pos) / size + 0.5;
    if (all(lessThan(abs(p - 0.5), vec2(0.5)))) {
        float val = textureGrad(ch, p / 16.0 + fract(vec2(char, 15 - char / 16) / 16.0), dFdx(p / 16.0), dFdy(p / 16.0)).r;
        color = mix(color, charColor, val);
    }
}

#define drawChars( ch, color,  charColor,p,  pos,  size, chars){for(int i_=0;i_<chars.length();i_++) drawChar( ch, color,  charColor,p,  pos +vec2(i_,0)*size.x*.5,  size , chars[i_]);}

