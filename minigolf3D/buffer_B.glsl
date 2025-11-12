// iChannel0 = Buffer A
// iChannel1 = image (random noise)

/* 
Buffer B 
uses bufferA data to render the scene
*/

struct Ray
{
    vec3 origin;
    vec3 direction;
    vec3 color;
    vec3 hitNormal;
    float hitDistance;
};

vec3 spherePos;


#define NUM_DIRECTIONS 128.0 

vec3 snapUniform3D(vec3 v)
{
    v = normalize(v);
    float phi = 2.3999632297;
    int bestIndex = 0;
    float bestDot = -1.0;
    
    for (int i = 0; i < int(NUM_DIRECTIONS); i++) {
        float y = 1.0 - (float(i) / (NUM_DIRECTIONS - 1.0)) * 2.0;
        float radius = sqrt(1.0 - y * y);
        float theta = phi * float(i);
        
        vec3 dir = normalize(vec3(cos(theta) * radius, y, sin(theta) * radius));
        
        float d = dot(v, dir);
        if (d > bestDot) {
            bestDot = d;
            bestIndex = i;
        }
    }
    
    float y = 1.0 - (float(bestIndex) / (NUM_DIRECTIONS - 1.0)) * 2.0;
    float radius = sqrt(1.0 - y * y);
    float theta = phi * float(bestIndex);
    
    return normalize(vec3(cos(theta) * radius, y, sin(theta) * radius));
}

mat3 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float c = cos(angle);
    float s = sin(angle);
    float t = 1.0 - c;

    return mat3(
        t * axis.x * axis.x + c,       t * axis.x * axis.y - s * axis.z, t * axis.x * axis.z + s * axis.y,
        t * axis.x * axis.y + s * axis.z, t * axis.y * axis.y + c,       t * axis.y * axis.z - s * axis.x,
        t * axis.x * axis.z - s * axis.y, t * axis.y * axis.z + s * axis.x, t * axis.z * axis.z + c
    );
}

mat3 RotationMatrixY(float angleY) 
{
    return rotationMatrix(vec3(0.,1.,0.), angleY);
}

mat3x3 RotationMatrixX(float angleX) 
{
    return rotationMatrix(vec3(1.,0.,0.), angleX);
}

vec3 ComputeFaceNormal(vec3 v0, vec3 v1, vec3 v2)
{
    return normalize(cross(v1 - v0, v2 - v0));
}

// Based on Morgan McGuire @morgan3d
// https://www.shadertoy.com/view/4dS3Wd
float random (in vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898,78.233)))* 43758.5453123);
}
float noise (in vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));
    vec2 u = f * f * (3.0 - 2.0 * f);
    return mix(a, b, u.x) + (c - a)* u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}
float fbm (in vec2 st) {
    float value = 0.0;
    float amplitude = .5;
    float frequency = 0.;
    value += amplitude * noise(st+iTime);
    st *= 2.;
    amplitude *= .5;
    value += amplitude * noise(st-iTime);
    return value;
}

vec3 computeWaveNormal(vec2 p) {
    float eps = 0.01; 
    float h = 0.4 * fbm(p*0.3);
    float hx = 0.4 * fbm((p + vec2(eps, 0.0))*0.3);
    float hy = 0.4 * fbm((p + vec2(0.0, eps))*0.3);
    return normalize(vec3(h - hx, 0.01, h - hy));
}

int rayPlane(inout Ray ray) 
{
    float t = 0.;
    {// intersection part
        float denom = dot(ray.direction, WORLD_UP);
        if (abs(denom) < 1e-6) return 0; // no intersection
         t = (SEA_LEVEL - dot(ray.origin, WORLD_UP)) / denom;
    }
    {// color part
        if(t>0. && t < ray.hitDistance)
        {
            ray.hitDistance = t;
            vec3 hitPoint = ray.origin + ray.hitDistance * ray.direction;
            ray.color = SEA_COLOR;
            ray.hitNormal = computeWaveNormal(hitPoint.xz);
            
            return 1;
        }
    }
    return 0;
}

int raySphereIntersect(inout Ray ray, vec3 spherePos, float sphereRadius)
{
    vec3 oc = ray.origin - spherePos;
    float inside = step(length(oc), sphereRadius);
    float a = dot(ray.direction, ray.direction);
    float b = 2.0 * dot(oc, ray.direction);
    float c = dot(oc, oc) - sphereRadius * sphereRadius;
    float discriminant = b * b - 4.0 * a * c;
    float hit = step(0.0, discriminant) * step(b, 0.0);
    return int(max(inside, hit));
}



int rayBallIntersect(inout Ray ray, vec3 spherePos, float sphereRadius, vec3 iColor)
{
    Ray tmpRay = ray;
    tmpRay.origin -= spherePos;
    spherePos = vec3(0.);
    vec2 currentSphereR = texelFetch(MEMORY_CHANNEL, sphereRotationAdress, 0).xy;
    mat3 rotX = RotationMatrixX(currentSphereR.x);
    mat3 rotY = RotationMatrixY(PI/2. - currentSphereR.y);

    const float scale = 1.0f;
    mat3 transformMat = scale * rotX;
    transformMat = transformMat * rotY ;
    tmpRay.origin = transformMat*tmpRay.origin;
    tmpRay.direction = transformMat*tmpRay.direction;

    vec3 oc = tmpRay.origin - spherePos;
    float a = dot(tmpRay.direction, tmpRay.direction);
    float b = 2.0 * dot(oc, tmpRay.direction);
    if(b >0.) return 0;
    float c = dot(oc, oc) - sphereRadius * sphereRadius;
    float discriminant = b * b - 4.0 * a * c;
    if (discriminant < 0.0)
        return 0 ;
    
    float sqrtDiscriminant = sqrt(discriminant);
    float t1 = (-b - sqrtDiscriminant) / (2.0 * a);
    float t2 = (-b + sqrtDiscriminant) / (2.0 * a);
    
    float mask = step(t1,0.0);
    float t = mask * t2 + t1 - mask * t1;
    if(t > tmpRay.hitDistance)
        return 0;
    ray.hitDistance = t;
    vec3 hitP = tmpRay.origin + tmpRay.direction * t;
    ray.color = BALL_COLOR;        
    ray.hitNormal = inverse(transformMat) *normalize(hitP-spherePos);
    #if BALL_PATTERN
    {
        vec3 refPos = 
        normalize(
            snapUniform3D(hitP-spherePos)
            )
            * BALL_SIZE 
            + spherePos;
        hitP = hitP - spherePos;
        hitP = hitP + spherePos;
        ray.hitNormal =length(refPos - hitP) < 0.05 ?  normalize(-(hitP-refPos)*10.+ray.hitNormal) : ray.hitNormal;
    }
    #endif //BALL_PATTERN
    return 1;
}

void rayTriangle(inout Ray ray, vec3 v0, vec3 v1, vec3 v2, vec3 color, mat3 transform)
{
    vec3 faceNormal = ComputeFaceNormal(v0,v1,v2); 
    float d = dot(faceNormal, v0);

    // Ray-plane intersection
    float t = (d - dot(faceNormal, ray.origin)) / dot(faceNormal, ray.direction);
    if (t < 0.0) return; // Skip if behind the camera
    vec3 p = ray.origin + t * ray.direction;
    
    // triangle test if the point is in the triangle
    float c0 = dot(cross(v1 - v0, p - v0), faceNormal);
    float c1 = dot(cross(v2 - v1, p - v1), faceNormal);
    float c2 = dot(cross(v0 - v2, p - v2), faceNormal);

    float condition = step(0.0, c0) * step(0.0, c1) * step(0.0, c2) * step(t, ray.hitDistance);
        
    {
        #if WIREFRAME
        if (!(c0 > 0.2 && c1 > 0.2 && c2 > 0.2))
        #endif 
        {
            vec3 hitPt = ray.origin + ray.direction * t;

            ray.color = ray.color * (1.-condition) + color * condition;

            faceNormal = -faceNormal * sign(dot(faceNormal, ray.direction));
            faceNormal = faceNormal*transform;
            ray.hitNormal = ray.hitNormal * (1.-condition) + faceNormal * condition;
            ray.hitDistance = ray.hitDistance * (1.-condition) + t * condition;
        }
    }
}

void rayMesh(inout Ray ray)
{
    
    for(int i = 0 ; i < tiles.length() ; i++)
    {
        Ray tmpRay = ray;
        int tmp = raySphereIntersect(tmpRay, tiles[i].position, tiles[i].boundingSphereRadius);
        mat3 transformMat = tiles[i].rotationMatrix ;
        tmpRay.origin -= tiles[i].position;
        tmpRay.origin = transformMat*tmpRay.origin;
        tmpRay.direction =  transformMat*tmpRay.direction;
        for (int j = tiles[i].type.x; j < tiles[i].type.y * tmp;j++) 
        {
            rayTriangle(tmpRay, verticesTiles[trianglesTile[j].x], verticesTiles[trianglesTile[j].y], verticesTiles[trianglesTile[j].z], tiles[i].color,transformMat);
            ray.color = tmpRay.color;
            ray.hitDistance= tmpRay.hitDistance;
            ray.hitNormal =  tmpRay.hitNormal ;
        }
    }
}

void rayIntersectShapes(inout Ray currentRay)
{
    rayBallIntersect(currentRay, spherePos, BALL_SIZE, vec3(1.0));
    rayMesh(currentRay);
}



void computeRay(inout Ray currentRay)
{
   rayIntersectShapes(currentRay);
   int hitPlane = rayPlane(currentRay);
   
   vec3 hitPoint = currentRay.origin + currentRay.direction * currentRay.hitDistance;
   
    
   float specularStrength = 1.0;
   float diffuseStrength = 1.0;
   #if GRASS_PATTERN
   {
       float grass = texture(iChannel1,hitPoint.xz*0.2).r;
       vec3 color = mix(GRASS_COLOR*0.7, GRASS_COLOR, grass);
       { // if(currentRay.color == GRASS_COLOR)
           float match = step(0.001, 1.0 - dot(step(0.001, abs(currentRay.color - GRASS_COLOR)), vec3(1.0)));
           currentRay.color = mix(currentRay.color, color, match);
           specularStrength = mix(specularStrength, 0.1, match);
       }
       
       { // if(currentRay.color == BALL_COLOR)
           float match = step(0.001, 1.0 - dot(step(0.001, abs(currentRay.color - BALL_COLOR)), vec3(1.0)));
           specularStrength = mix(specularStrength, 0.1, match);
           diffuseStrength = mix(diffuseStrength, 0.8, match);
       }
   }
   #endif //GRASS_PATTERN
   
   #if PHONG_LIGNTING
   {
       // ambiant
       vec3 ambiant = 0.3 * LIGHT_COLOR;
       // diffuse 
       vec3 norm = normalize(currentRay.hitNormal);
       vec3 lightDir = normalize(LIGHT_DIR);
       float diff = max(dot(norm, lightDir), 0.0);
       vec3 diffuse = diffuseStrength * diff * LIGHT_COLOR;
       // specular
       vec3 viewDir = normalize(currentRay.direction );
       vec3 reflectDir = reflect(-lightDir, norm);  
       float spec = pow32(max(dot(viewDir, -reflectDir), 0.0)); 
       vec3 specular = specularStrength * spec * LIGHT_COLOR;  
       float tmpStep = step(currentRay.hitDistance, renderDistance*10.);
       currentRay.color = ((ambiant + diffuse + specular)* tmpStep + 1.-tmpStep) * currentRay.color;
   }
   #endif //PHONG_LIGNTING

   #if RENDER_SHADOWS
   if( currentRay.hitDistance > 0. && currentRay.hitDistance <= renderDistance)
   {
       Ray shadowRay;
       shadowRay.origin      = currentRay.origin + currentRay.direction * (currentRay.hitDistance-epsilon*5.);

       float condition = step(SEA_LEVEL+epsilon, (currentRay.origin+currentRay.hitDistance*currentRay.direction).y);
       shadowRay.direction   = normalize(LIGHT_DIR * condition + (LIGHT_DIR+1.0 * dot(LIGHT_DIR, currentRay.hitNormal) * currentRay.hitNormal)*(1.-condition));

       shadowRay.hitDistance = renderDistance*20.0;
       rayIntersectShapes(shadowRay);
       float condition2 = step(shadowRay.hitDistance, renderDistance * 2.0);
       currentRay.color = currentRay.color * (1. - condition2) + currentRay.color * 0.5 * condition2;
   }
   #endif //RENDER_SHADOWS
   
   #if REFLEXION
   for(int i = 0 ; i < hitPlane ; i++)
   {
       Ray reflectRay;
       reflectRay.origin = currentRay.origin + currentRay.direction * (currentRay.hitDistance-epsilon*5.);
       reflectRay.direction = normalize(currentRay.direction - 2.0 * dot(currentRay.direction, currentRay.hitNormal) * currentRay.hitNormal);
       reflectRay.hitDistance = renderDistance*20.0;
       reflectRay.color = SKY_COLOR;
       rayIntersectShapes(reflectRay);
       currentRay.color = currentRay.color * 0.7 + reflectRay.color*0.3;
   }
   #endif //REFLEXION*/
}



void mainImage(out vec4 fragColor, in vec2 fragCoord) {

    /* MANAGE MAX RESOLUTION */
    float dividing =  texelFetch(MEMORY_CHANNEL, dividingAdress, 0).r;
    if(fragCoord.x > iResolution.x /dividing || fragCoord.y > iResolution.y /dividing)
        return;
    fragCoord.x = fragCoord.x * dividing;
    fragCoord.y = fragCoord.y * dividing;
    /* --------------------- */
    
    vec2 uv = fragCoord/iResolution.xy;
    uv   -= 0.5;
    uv.x *= iResolution.x/iResolution.y;
    
    
    spherePos = texelFetch(MEMORY_CHANNEL, charPosAdress, 0).xyz;
    

    vec3 cameraPos = texelFetch(MEMORY_CHANNEL, camPosAdress, 0).xyz;
    vec3 forward = normalize(spherePos - cameraPos);
    vec3 up = normalize(WORLD_UP - dot(WORLD_UP, forward) * forward);
    vec3 right = cross(up, forward);
    vec2 currentSpeed = texelFetch(MEMORY_CHANNEL, charSpeedAdress, 0).xz;
    float zoom = 1. - mix(0.0, 0.3, length(currentSpeed)/MAX_SPEED); // changes fov relative to speed
    
    vec3 center = cameraPos + (forward * zoom);
    vec3 i = center + (uv.x * right) + (uv.y * up);
    
    Ray currentRay;
    currentRay.origin      = cameraPos;
    currentRay.direction   = i - cameraPos;
    currentRay.hitDistance = renderDistance*20.0;
    currentRay.color       = SKY_COLOR;
    
    computeRay(currentRay);
    fragColor = vec4(currentRay.color, 1.0);
}
