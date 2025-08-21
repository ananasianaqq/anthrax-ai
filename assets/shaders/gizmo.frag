#version 450

#include "defines/fragdef.h"
#include "defines/defines.h"


void main()
{
    vec4 mousepos = GetResource(Camera, bufferbind).mousepos;

    uint depth_ind = uint(gl_FragCoord.z * DEPTH_ARRAY_SCALE);
    if( length(mousepos.xy - gl_FragCoord.xy) < 5)
    {
        GetResource(Storage, storagebind).data[0] = objectID;
    }

    vec4 color = texture(textures[texturebind], incoord.xy).xyzw;
    if (selected == 1) {
        color.rgb += vec3(0.1);
        color.rgb = clamp(color.rgb, 0.0, 1.0);
    }

    if (gizmo == GIZMO_X)
    {
        color.rgb = selected == 1 ? vec3(0.9,0.1,0.1) : vec3(0.37,0.1,0.1);
    }
    if (gizmo == GIZMO_Y){
        color.rgb = selected == 1 ? vec3(0.0,0.8,0.0) : vec3(0.01,0.27,0.01);

    }
    if (gizmo == GIZMO_Z) {
        color.rgb = selected == 1 ? vec3(0.05,0.05,0.9) : vec3(0.1,0.1,0.27);
    }

    outfragcolor = color;
}
