#version 450

#include "defines/fragdef.h"
#include "defines/defines.h"


void main()
{
    vec4 mousepos = GetResource(Camera, GetUniformInd()).mousepos;

    uint depth_ind = uint(gl_FragCoord.z * DEPTH_ARRAY_SCALE);
    if( length(mousepos.xy - gl_FragCoord.xy) < 5)
    {
        GetResource(Storage, GetStorageInd()).data[0] = pushconstants.objectID;
    }

    vec4 color = texture(textures[GetTextureInd()], incoord.xy).xyzw;
    if (pushconstants.selected == 1) {
        color.rgb += vec3(0.1);
        color.rgb = clamp(color.rgb, 0.0, 1.0);
    }

    int gizmo = pushconstants.gizmo;
    if (gizmo == GIZMO_X)
    {
        color.rgb = pushconstants.selected == 1 ? vec3(0.0,0.8,0.0) : vec3(0.01,0.27,0.01);
    }
    if (gizmo == GIZMO_Y){
        color.rgb = pushconstants.selected == 1 ? vec3(0.05,0.05,0.9) : vec3(0.1,0.1,0.27);

    }
    if (gizmo == GIZMO_Z) {
        color.rgb = pushconstants.selected == 1 ? vec3(0.9,0.1,0.1) : vec3(0.37,0.1,0.1);
    }

    outfragcolor = color;
}
