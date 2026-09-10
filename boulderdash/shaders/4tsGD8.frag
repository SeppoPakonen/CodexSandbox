// srtuss, 2015

#define pi2 6.283185307179586476925286766559

vec2 rotate(vec2 p, float a)
{
	return vec2(p.x * cos(a) - p.y * sin(a), p.x * sin(a) + p.y * cos(a));
}
#define obs2 0.70710678118654752440084436210485

float titlefnt(vec2 p)
{
    float v;
    vec2 q;
    
    v = -100.0;
    
    // b
    v = max(v, (p.x + abs(p.y + 0.5)) * obs2 - 23.5);
    
    v = max(v, -((5. - p.x + abs(p.y - 3.5)) * obs2));
    
    q = abs(p - vec2(2., 16.));
    v = max(v, -max(q.x - 5., q.y - 4.));
    q.y = abs(p.y + 8.0);
    v = max(v, -max(q.x - 5., q.y - 4.));
    
    q = abs(p);
    v = max(v, max(q.x - 9., q.y - 28.));
    
    float cp = v;
    
    
    p.x -= 22.;
    
    // o
    q = abs(p - vec2(0., -4.));
    v = max(q.x - 7.0, q.y - 24.);
    q = abs(p - vec2(2., -1.));
    v = max(v, -max(q.x - 3.0, q.y - 12.));
    cp = min(cp, v);
    
    p.x -= 20.;
    
    // u
    q = abs(p - vec2(0., -4.));
    v = max(q.x - 7.0, q.y - 24.);
    q = vec2(abs(p.x - 2.), -p.y -1.);
    v = max(v, -max(q.x - 3.0, q.y - 12.));
    cp = min(cp, v);
    
    p.x -= 20.;
    
    // l
    q = abs(p - vec2(0., -4.));
    v = max(q.x - 7.0, q.y - 24.);
    q = vec2(-p.x + 2., -p.y - 1.);
    v = max(v, -max(q.x - 3.0, q.y - 12.));
    cp = min(cp, v);
    
    p.x -= 20.;
    
    // d
    q = abs(p - vec2(0., -4.));
    v = max(q.x - 7.0, q.y - 24.);
    q = abs(p - vec2(2., -1.));
    v = max(v, -max(q.x - 3.0, q.y - 12.));
    v = max(v, (p.x + abs(p.y + 4.5)) * obs2 - 19.0);
    cp = min(cp, v);
    
    p.x -= 20.;
    
    // e
    q = vec2(-p.x + 2., abs(p.y) - 0.5);
    v = -max(q.x - 3.0, q.y - 12.);
    q = vec2(p.x - 3., abs(p.y) - 4.0);
    v = min(v, max(q.x, q.y));
    
    q = abs(p - vec2(0., -4.));
    v = max(v, max(q.x - 7.0, q.y - 24.));
    
    cp = min(cp, v);
    
    p.x -= 20.;
    
    // r
    v = 0.0;
    
    q = abs(p - vec2(2., 8.));
    v = -max(q.x - 3.0, q.y - 4.);
    v = max(v, -max(q.x - 3.0, p.y + 12.));
    
    q = abs(p - vec2(0., -4.));
    v = max(v, max(q.x - 7.0, q.y - 24.));
    
    v = max(v, (p.x + p.y + 4.5) * obs2 - 19.0);
    v = max(v, -((abs(p.y + 4.5) - p.x) * obs2 + 2.0));
    
    cp = min(cp, v);
    
    
    p.x += 102.;
    p.y += 64.;
    
    // line2 b
    v = (p.x + abs(p.y + 0.5)) * obs2 - 23.5;
    
    q = abs(p - vec2(2., 4.));
    v = max(v, -max(q.x - 5., q.y - 16.));
    
    q = abs(p);
    v = max(v, max(q.x - 9., q.y - 28.));
    
    cp = min(cp, v);
    
    
    p.x -= 22.;
    
    // a
    q = abs(p - vec2(2., 8.));
    v = -max(q.x - 3.0, q.y - 4.);
    v = max(v, -max(q.x - 3.0, p.y + 4.));
    
    v = max(v, (p.x + p.y + 4.5) * obs2 - 19.0);
    v = max(v, -max(p.x + 3., 12.-p.y));
    
    q = abs(p - vec2(0., -4.));
    v = max(v, max(q.x - 7.0, q.y - 24.));
    
    cp = min(cp, v);
    
    p.x -= 20.;
    
    // s
    q = vec2(p.x, abs(p.y - 8.));
    v = -max(-1. - q.x, q.y - 4.);
    q = vec2(p.x, abs(p.y + 8.));
    v = max(v, -max(-1. + q.x, q.y - 4.));
    
    q = abs(p - vec2(0., -4.));
    v = max(v, max(q.x - 7.0, q.y - 24.));
    
    cp = min(cp, v);
    
    p.x -= 22.;
    
    // h
    q = vec2(abs(p.x - 2.), abs(p.y));
    v = min(-q.x + 5., q.y - 4.);
    
    q = abs(p - vec2(0., -4.));
    v = max(v, max(q.x - 9.0, q.y - 24.));
    
    cp = min(cp, v);
    
    p.x -= 20.;
    
    // box
    q = abs(p - vec2(0., 16.0));
    cp = min(cp, max(q.x - 5.0, q.y - 4.0));
    
    return cp;
}

float titlecmp(vec2 p, vec2 po)
{
    float fnt = step(titlefnt(p), 0.0);
    fnt = max(fnt, step(titlefnt(p + vec2(1.0, 0.0)), 0.0) * 0.4);
    fnt = max(fnt, step(titlefnt(p + vec2(2.0, 0.0)), 0.0) * 0.2);
    
    p.y = floor(po.y - iTime * 34.0) - 0.5;
    
    float idy = floor(p.y / 4.);
    p.x += idy * 2. + 1.;
    vec2 q = mod(p, vec2(4.));
    q -= .5;
    float qd = (q.x * q.x + q.y * q.y) * .2;
    
    float v = mix(0.3, qd, step(max(q.x - 1.0, q.y - 2.), 0.0)) + 0.1;
    v = mix(fnt, v, step(fnt, 0.01));
    
    return v;
}

float hsh(float x)
{
    return fract(sin(x * 12.315623) * 219862.251235);
}

float hsh(vec2 p)
{
    return fract(sin(p.x + p.y * 2.39996) * 219862.251235);
}

float nse(float x)
{
    float fl = floor(x);
    return mix(hsh(fl), hsh(fl + 1.0), smoothstep(0.0, 1.0, fract(x)));
}

float nse(vec2 p)
{
    vec2 fl = floor(p);
    vec2 fr = fract(p);
    fr = fr * fr * (3.0 - 2.0 * fr);
    vec2 h = vec2(1.0, 0.0);
    return mix(mix(hsh(fl), hsh(fl + h.xy), fr.x), mix(hsh(fl + h.yx), hsh(fl + h.xx), fr.x), fr.y);
}

vec3 px(vec2 p)
{
   	
    
    p *= vec2(0.5, 1.0);
    vec2 po = p;
    p = floor(p);
    p -= 0.5;
    
    p += vec2(124.0 * 0.5, -30.0);
    
    float v = titlecmp(p, po);
    
    vec2 q;
    
    q = abs(p - vec2(61.0, -32.0));
    float w = max((q.x - 78.) * 2.0, q.y - 70.);
    
    v = mix(v, 1.0, step(abs(w) - 4.0, 0.0));
    v *= step(w, 0.0);
    
    vec3 col = pow(vec3(v), vec3(0.6, 0.4, 0.1) * 3.0) * 1.2;
    
    float lerr = step(iTime - 0.1, 0.0);
    col *= 1.0 - lerr;
    col = mix(col, vec3(1.0, 0.1, 0.0), step(abs(p.y - 10.0) - 5.0, 0.0) * lerr);
    col = mix(col, vec3(1.0, 0.1, 0.0), step(abs(p.y - 30.0) - 2.0, 0.0) * lerr);
    col = mix(col, vec3(0.0, 1.0, 0.0), step(abs(p.y - 8.0) - 1.0, 0.0) * lerr);
    
    return col;//ceil(v * 4.0 - 0.5) / 3.0;
}

vec3 pxss(vec2 p, float scl, float flow)
{
    vec3 h = vec3(6.0 / iResolution.y, 1.0 / iResolution.y, 0.0);
    h *= scl * 0.5;
    p *= scl;
    h *= flow;
    return (px(p) + px(p + h.xz) + px(p + h.zy) + px(p + h.xy)) / 4.0;
}

vec3 tv(vec2 p)
{
    float ll = dot(p, p);
    p *= (ll * ll * 0.01 + ll * ll * ll * 0.001 + ll * 0.04) * 0.3 + 0.99;
    //p = rotate(p, -0.03);
    
    float dstY = fract(iTime * 0.2) * 100.0 - 50.0;
    float dst = dstY - p.y;
    dst = dst * dst;
    float dN = (nse(iTime * 5.0) + nse(iTime * 15.0) * 0.5) * 0.66666666666;
    float dstI = pow(dN, 1.0);
    float gryI = pow(dN, 2.0);
    float synI = pow(dN, 7.0);
    p.x -= exp(dst * -2.0) * 10.0 * dstI;
    //float synI = pow(nse(iTime * 10.0), 10.0) * 10.0 * fract(iTime);
    //p.y = (fract(p.y * 0.5 + 0.5 - synI) - 0.5) / 0.5;
    
    
    
    
    
    float tst = 0.25;
    vec3 col = pow(vec3(pxss(p, tst * iResolution.y, synI * 10.0 + 1.0)), vec3(0.6, 0.5, 0.1) * 2.0);
    
    
    
    //uv * 90.0 + vec2(124.0, -30.0)
    
    col = mix(col, pow(vec3(col.x + col.y + col.z) / 4.0, vec3(0.5)), gryI);
    col -= exp(dst * -0.1) * dstI * 1.0;
    col += nse(rotate(p * 22. + 100.0, iTime * 40.0)) * (0.02 + synI);
    
    vec2 pp = p - vec2(1.4, 0.7);
    vec2 q = abs(pp);
    float v = q.y;
    v = min(v, max(pp.x + 0.08, -pp.y));
    v = min(v, max(q.x, pp.y - 0.04));
    v = max(v, max(q.x - 0.08, abs(pp.y - 0.03) - 0.13));
    col = mix(col, vec3(0.2, 0.8, 0.1), smoothstep(0.01, 0.0, v - 0.02) * step(iTime, 4.0));
    
    
    
    col *= cos(p.y * tst * iResolution.y * pi2) * 0.1 + 0.9;
    
    col += exp(length(p * vec2(0.3, 1.0)) * -3.0) * 0.2;

    
    
    
    
    return col;
}


void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
	vec2 uv = fragCoord.xy / iResolution.xy;
    
    uv = 2.0 * uv - 1.0;
    uv.x *= iResolution.x / iResolution.y;
    
    
    /*float nt = uv.x;
    float v = (smoothstep(0.0, 0.2, nt) * smoothstep(1.0, 0.3, nt));
    
    v = abs(v - uv.y);
    
    v = min(v, abs(uv.x - 1.0));
    
    v = smoothstep(0.0, 0.01, v);*/
    
    //vec3 col = px(uv * 90.0 + vec2(124.0, -30.0)) * vec3(1.0);
    vec3 col = tv(uv) * vec3(1.0);
    
    
    col = pow(col, vec3(1.0 / 2.0));
    
	fragColor = vec4(col, 1.0);
}