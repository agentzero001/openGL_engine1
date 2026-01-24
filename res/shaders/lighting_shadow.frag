#version 430 core

in vec3 varyingNormal;
in vec3 varyingLightDir;
in vec3 varyingVertPos;
in vec3 varyingHalfVector;

in vec2 tc;
in vec4 shadow_coord;

layout (binding = 0) uniform sampler2D samp;
layout (binding = 1) uniform sampler2DShadow shTex;
layout (binding = 2) uniform samplerCube sampCube;


out vec4 color;


struct PositionalLight{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 position;
};

struct Material {
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;  
};


uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;
uniform Material material;
uniform PositionalLight light;

uniform vec4 globalAmbient;

uniform float tf;

vec4 textureColor;
vec4 lightColor;


float lookUp(float ox, float oy) {
    float t = textureProj(shTex, shadow_coord + vec4(ox * 0.001 * shadow_coord.w, oy * 0.001 * shadow_coord.w, -0.01, 0.0));
    return t;
}

//phong shading
void main(void) {

    float shadowFactor = .0;
    vec3 L = normalize(varyingLightDir);
    vec3 N = normalize(varyingNormal);
    vec3 V = normalize(-varyingVertPos);
    vec3 H = normalize(varyingHalfVector);

    float sWidth = 1.0;

    // vec2 offset = mod(floor(gl_FragCoord.xy), 2.0) * sWidth;
    // shadowFactor += lookUp(-1.5 * sWidth + offset.x, 1.5 * sWidth - offset.y);
    // shadowFactor += lookUp(-1.5 * sWidth + offset.x, -0.5 * sWidth - offset.y);
    // shadowFactor += lookUp(0.5 * sWidth + offset.x, 1.5 * sWidth - offset.y);
    // shadowFactor += lookUp(0.5 * sWidth + offset.x, -0.5 * sWidth - offset.y);
    // shadowFactor = shadowFactor / 4.0;


    // float endp = sWidth * 3.0 + sWidth / 2.0;
    // for (float m=-endp; m<=endp; m=m+sWidth) {
    //     for (float n=-endp; n<=endp; n=n+sWidth) {
    //         shadowFactor += lookUp(m, n);
    //     }
    // }
    // shadowFactor = shadowFactor / 64.0;


    float notInShadow = textureProj(shTex, shadow_coord);

    //R can be omitted using dot(H, N) instead of dot(V, R).
    vec3 R = normalize(reflect(-L, N));
    float cosTheta = dot(L, N);
    float cosPhi = dot(H, N);

    vec3 ambient = ((globalAmbient * material.ambient) + (light.ambient * material.ambient)).xyz;
    vec3 diffuse = light.diffuse.xyz * material.diffuse.xyz * max(cosTheta, 0.0f);
    vec3 specular = material.specular.xyz * light.specular.xyz * pow(max(cosPhi, 0.0f), material.shininess);

    // color = vec4((ambient + diffuse + specular), 1.0f);
    textureColor = texture(samp, tc);
    lightColor = vec4((diffuse + specular), 1.0f);
    color = 0.4 * textureColor + 0.2 * vec4(ambient, 1.0f);
    if (notInShadow == 1.0) {
        color += 0.5 * lightColor;
    }
    // color = vec4((color.xyz + shadowFactor * (lightColor.xyz)), 1.0);

    // vec3 r = -reflect(normalize(-varyingVertPos), normalize(varyingNormal));
    // color = texture(sampCube, r);

}



