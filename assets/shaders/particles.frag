#version 450

#include "defines/defines.h"
precision mediump float;
layout(location = 0) out vec4 outcolor;
layout(location = 0) in mediump vec4 incolor;

float falloff(vec2 point_coord)
{
    vec2 center_dist = point_coord - 0.5;
    float dist_sqr = dot(center_dist, center_dist);
    return exp2(-dist_sqr * 15.0);
}

void main()
{
    vec2 coord = gl_PointCoord.xy;
    outcolor = vec4(falloff(coord) * incolor.rgb, incolor.a );
}
