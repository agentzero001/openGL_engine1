#version 430

struct Particle {
	vec3 position;
	vec3 velocity;
    vec3 velocity2;
};

// layout(std430, binding = 0) buffer ParticleBuffer1 {
//     Particle sphereParticles[];
// };

layout(std430, binding = 1) buffer ParticleBuffer2 {
    Particle rainParticles[];
};

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 vertNormal;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 instanceData;

layout (binding = 0) uniform sampler2D samp;
layout (binding = 1) uniform sampler2DShadow shTex;
layout (binding = 2) uniform samplerCube sampCube;

out vec2 tc;

out vec3 varyingNormal; 
out vec3 varyingLightDir;
out vec3 varyingVertPos;
out vec3 varyingHalfVector;


out vec4 shadow_coord;

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

uniform vec4 globalAmbient;
uniform PositionalLight light;
uniform Material material;
uniform mat4 mv_matrix;
uniform mat4 v_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;

uniform mat4 shadow_mvp;

uniform bool isInstanced;
// uniform bool startTime;
uniform float dt;

mat4 buildRotateX(float rad);
mat4 buildRotateY(float rad);
mat4 buildRotateZ(float rad);
mat4 buildTranslate(float x, float y, float z);
mat4 computeInstancedModelView();
mat4 final_mv_matrix;
mat4 final_norm_matrix;

vec3 Xnormal = vec3(1.0, 0.0, 0.0);
float S = 10.0;


//phong shading
void main(void) {


    if (!isInstanced){
        final_mv_matrix = mv_matrix;
        final_norm_matrix = norm_matrix;
    } 
    else {
        final_mv_matrix = computeInstancedModelView();
        final_norm_matrix = transpose(inverse(final_mv_matrix));
    }

    vec4 P = final_mv_matrix * vec4(vertPos, 1.0);
    varyingVertPos = P.xyz;
    varyingLightDir = light.position - varyingVertPos;
    varyingNormal = (final_norm_matrix * vec4(vertNormal, 1.0)).xyz;
    varyingHalfVector = (varyingLightDir + (-varyingVertPos)).xyz;
    shadow_coord = shadow_mvp * vec4(vertPos, 1.0);
    gl_Position = proj_matrix * P; 
    tc = texCoord;

}

// builds and returns a matrix that performs a rotation around the X axis
mat4 buildRotateX(float rad) {
    mat4 xrot = mat4(1.0, 0.0,      0.0,       0.0,
                     0.0, cos(rad), -sin(rad), 0.0,
                     0.0, sin(rad), cos(rad),  0.0,
                     0.0, 0.0,      0.0,       1.0);
    return xrot;
}

// builds and returns a matrix that performs a rotation around the Y axis
mat4 buildRotateY(float rad) {
    mat4 yrot = mat4(cos(rad),  0.0, sin(rad), 0.0,
                     0.0,       1.0, 0.0,      0.0,
                     -sin(rad), 0.0, cos(rad), 0.0,
                     0.0,       0.0, 0.0,      1.0);
    return yrot;
}

// builds and returns a matrix that performs a rotation around the Z axis
mat4 buildRotateZ(float rad) {
    mat4 zrot = mat4(cos(rad), -sin(rad), 0.0, 0.0,
                     sin(rad), cos(rad),  0.0, 0.0,
                     0.0,      0.0,       1.0, 0.0,
                     0.0,      0.0,       0.0, 1.0);
    return zrot;
}

// builds and returns a translation matrix
mat4 buildTranslate(float x, float y, float z) {
    mat4 trans = mat4(1.0, 0.0, 0.0, 0.0,
                      0.0, 1.0, 0.0, 0.0,
                      0.0, 0.0, 1.0, 0.0,
                      x,   y,   z,   1.0);
    return trans;
}

mat4 buildTranslate(vec3 pos) {
    mat4 trans = mat4(1.0, 0.0, 0.0, 0.0,
                      0.0, 1.0, 0.0, 0.0,
                      0.0, 0.0, 1.0, 0.0,
                      pos.x,  pos.y,  pos.z,   1.0);
    return trans;
}

mat4 computeInstancedModelView() {
  
    // float speed = startTime ? dt * 60.0 : 0.0;
    mat4 localRotY = buildRotateY(dt);
    // mat4 localRotZ = buildRotateZ(speed * 0.2);
    // vec3 dir = (localRotY * vec4(instanceData.xyz, 1.0)).xyz;
    // vec3 offset = instanceData * speed * 60.0;
    // vec3 offset = dir * speed * 60.0;
    // if (offset.x >= S && abs(offset.y) <= S && abs(offset.z) <= S) {
    //     offset = reflect(offset, Xnormal);
    // }
    // mat4 localTrans = buildTranslate(a, b, c);
    // return v_matrix * (localTrans * localRotX * localRotY * localRotZ);

    mat4 initialPos = buildTranslate(rainParticles[gl_InstanceID].position);
    return v_matrix * initialPos * localRotY;
}

