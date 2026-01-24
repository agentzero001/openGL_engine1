#version 430 core

in vec2 tc;
in vec4 varyingColor;

out vec4 color;

uniform mat4 v_matrix;
uniform mat4 proj_matrix;

layout (binding = 0) uniform sampler2D samp;



void main(void) {
    // color = varyingColor;
    color = texture(samp, tc);
}
//    float t = (gl_FragCoord.x / 600.0) + tf * 2;  // 0 -> 1 across screen
    //color = mix(vec4(1,0,0,1), vec4(0,0,1,1), t);

   // vec3 c;

//    if (t < 0.5) {
//        float k = t / 0.5;
//        c = mix(vec3(1, 0, 0), vec3(0, 1, 0), k);
//    } else {
//        float k = (t - 0.5) / 0.5;
//        c = mix(vec3(0, 1, 0), vec3(0, 0, 1), k);
//    }
//    color = vec4(c, 1);

    // vec2 uv = gl_FragCoord.xy / 1080;   // 0..1
    // vec2 c  = uv - 0.5;                       // center at (0.5, 0.5)

    // float angle = atan(c.y, c.x);            
    // float t = angle / 6.2831 + tf;          // normalize + rotate

    // color = vec4(
    //     0.5 + 0.5 * sin(6.2831 * t + 0.0),
    //     0.5 + 0.5 * sin(6.2831 * t + 2.094),
    //     0.5 + 0.5 * sin(6.2831 * t + 4.188),
    //     1.0
    // );


