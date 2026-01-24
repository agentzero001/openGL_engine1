#version 430
layout (location = 0) in vec3 vertPos;
uniform mat4 shadow_mvp;


void main(void) {
    gl_Position = shadow_mvp * vec4(vertPos, 1.0);
}
