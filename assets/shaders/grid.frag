#version 450

#include "defines/defines.h"
#include "defines/helpers.h"

layout (location = 0) in vec3 innear;
layout(location = 1) in vec3 infar;

layout (location = 0) out vec4 outfragcolor;

vec4 grid(vec3 pos, float scale, float div, vec3 col) {
    vec2 coord = pos.xz * scale;
    vec2 derivative = fwidth(coord);
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
    float line = min(grid.x, grid.y);
    float minimumz = min(derivative.y, 1);
    float minimumx = min(derivative.x, 1);

    vec4 color = vec4(col, 1.0 - min(line, 1.0));
    // z axis
    if(pos.x > -0.1 * minimumx && pos.x < 0.1 * minimumx ) {
        color.b = 1.0;
    }
    // x axis
    if(pos.z > -0.1 * minimumz && pos.z < 0.1 * minimumz) {
        color.r = 1.0;
    }
    color.ga *= div;
    return color;
}

void main()
{
    mat4 proj = GetResource(Camera, GetUniformInd()).proj;
    mat4 view = GetResource(Camera, GetUniformInd()).view;

    float y = -innear.y / (infar.y - innear.y); // parametric equation of a line (y axis)
    vec3 clippos = innear + y * (infar - innear);

    if (y < 0) {
        outfragcolor = vec4(0);
        return ;
    }
    gl_FragDepth = clamp(ClipSpaceDepth(clippos, proj, view), 0, 1);

    float lineardepth = clamp(LinearDepth(clippos, proj, view), 0, 1);
    float fading = smoothstep(1, 0, lineardepth);

    outfragcolor.rgba = (grid(clippos, 1, 1, vec3(0.4, 0.4, 0.4))).rgba;
    outfragcolor.rgba += (grid(clippos, 10, 0.2, vec3(0.005, 0.005, 0.005))).rgba;
    outfragcolor.a *= fading;
}
