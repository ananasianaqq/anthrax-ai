#version 450

#include "defines/defines.h"

layout(location = 0) in vec2 inpostion;
layout(location = 1) in vec2 invelocity;
layout(location = 2) in vec4 incolor;

layout(location = 0) out mediump vec4 outcolor;

void main()
{
    gl_Position = vec4(inpostion, 0, 1);
    gl_PointSize = 10 * exp2(dot(inpostion, inpostion) * -3);
    outcolor = incolor;
}
