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
bool hasTexture = true;



//phong shading
void main(void) {

    vec3 L = normalize(varyingLightDir);
    vec3 N = normalize(varyingNormal);
    vec3 V = normalize(-varyingVertPos);
    vec3 H = normalize(varyingHalfVector);

    if (!gl_FrontFacing) N = -N;
    float notInShadow = textureProj(shTex, shadow_coord);

    //R can be omitted using dot(H, N) instead of dot(V, R).
    vec3 R = normalize(reflect(-L, N));
    float cosTheta = dot(L, N);
    float cosPhi = dot(H, N);

    vec3 mAmbient = ((globalAmbient * material.ambient) + (light.ambient * material.ambient)).xyz;
    vec3 mDiffuse = light.diffuse.xyz * material.diffuse.xyz * max(cosTheta, 0.0f);
    vec3 mSpecular = material.specular.xyz * light.specular.xyz * pow(max(cosPhi, 0.0f), material.shininess);

    // color = vec4((ambient + diffuse + specular), 1.0f);
    lightColor = vec4((mDiffuse + mSpecular), 1.0f);

    if (hasTexture) {
        textureColor = texture(samp, tc);
        color = 0.8 * textureColor + 0.2 * vec4(mAmbient, 1.0f);
    }
    else color = vec4(mAmbient, 1.0f);
    
    if (notInShadow == 1.0) {
        color += 0.4 * lightColor;
    }
}



