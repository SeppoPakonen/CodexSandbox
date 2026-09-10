float toLinear1(float c)
{
    return (c <= 0.04045) ? c / 12.92 : pow((c + 0.055) / 1.055, 2.4);
}

vec3 toLinear(vec3 c)
{
    return vec3(toLinear1(c.r), toLinear1(c.g), toLinear1(c.b));
}

float toSrgb1(float c)
{
    return (c < 0.0031308) ? c * 12.92 : 1.055 * pow(c, 0.41666) - 0.055;
}

vec3 toSrgb(vec3 c)
{
    return vec3(toSrgb1(c.r), toSrgb1(c.g), toSrgb1(c.b));
}

vec3 mask(vec2 pos)
{
    const float maskDark=0.6;
    const float maskLight=2.0;

    pos.x += pos.y * 3.0;
    vec3 mask = vec3(maskDark);
    pos.x = fract(pos.x / 6.0);

    if (pos.x < 0.333) mask.r = maskLight;
    else if (pos.x < 0.666) mask.g = maskLight;
    else mask.b = maskLight;

    return mask;
}

vec2 fragCoord2UV(vec2 fc)
{
    vec2 fitNum = floor(iResolution.xy / TIT_RES_F);
    float scale = max(min(fitNum.x, fitNum.y), 1.0);
    vec2 fc2 = remap(vec2(0.0), TIT_RES_F * vec2(float(scale)), vec2(0.0), TIT_RES_F, fc);
    fc2 -= floor((remap(vec2(0.0), TIT_RES_F * vec2(float(scale)), vec2(0.0), TIT_RES_F, iResolution.xy) - TIT_RES_F) / vec2(2.0));
    return fc2 / iResolution.xy;
}


void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord2UV(fragCoord);

    vec3 col = toLinear(texture(iChannel0, uv).rgb);
    col *= mask(fragCoord.xy);
    col = toSrgb(col);

    fragColor = vec4(col, 1.0);
}