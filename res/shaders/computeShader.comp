#version 430

layout(local_size_x = 1) in;

struct Particle {
    vec3 position;
    vec3 velocity;
    vec3 velocity2;
};

layout(std430, binding = 1) buffer ParticleBuffer2 {
    Particle rainParticles[];
};

uniform float dt;
uniform bool startTime;
uniform bool switchVelocity;


void main() {
    uint id = gl_GlobalInvocationID.x;


    vec3 pos = rainParticles[id].position;

    if (startTime) {
        vec3 dir = normalize(-rainParticles[id].position);
        pos += dir * .05;
    } 
    // pos += rainParticles[id].velocity;
    else {
       if (switchVelocity) {
        pos -= rainParticles[id].velocity * .1;;
        }
        else {
            pos += rainParticles[id].velocity * .1;
        }
    }

    if (pos.y <= -50.0) pos.y = 50.0;
    

    // if (pos.y <= -40.0)
    //     pos.y = 20.0;

    rainParticles[id].position = pos;
}


