#version 450

#include "defines/vertdef.h"
#include "defines/defines.h"

#include "defines/instance.h"
void main()
{
    
    DefineInstanceResources();
    mat4 view = mat4(mat3(GetResource(Camera, bufferbind).view ));
    gl_Position =GetResource(Camera, bufferbind).skybox_proj * view * vec4(vposition.xyz, 1.0f);  
    outcolor = vcolor;
    outcoord = vuv;
    outpos = vec4(vposition.xyz, 1.0f);
}
