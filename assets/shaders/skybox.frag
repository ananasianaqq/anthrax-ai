#version 450

#include "defines/defines.h"
#include "defines/fragdef.h"

void main()
{
    int cube_ind = GetResource(Camera, bufferbind).cubemapbind;
    outfragcolor = texture(cubemaps[cube_ind], vec3(inpos.xyz)).xyzw;

}
