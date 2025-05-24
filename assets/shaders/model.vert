#version 460

#include "defines/vertdef.h"
#include "defines/defines.h"
//#extension GL_EXT_debug_printf : enable

void main()
{
    uint hasanim = GetResource(Instance, GetInstanceInd()).instances[gl_BaseInstance].hasanimation;
    mat4 bonetransforms = mat4(1.0f);

    if (hasanim == 1) {
        bonetransforms = GetResource(Instance, GetInstanceInd()).instances[gl_BaseInstance].bonesmatrices[vboneid[0]] * vweight[0];
        bonetransforms += GetResource(Instance, GetInstanceInd()).instances[gl_BaseInstance].bonesmatrices[vboneid[1]] * vweight[1];
        bonetransforms += GetResource(Instance, GetInstanceInd()).instances[gl_BaseInstance].bonesmatrices[vboneid[2]] * vweight[2];
        bonetransforms += GetResource(Instance, GetInstanceInd()).instances[gl_BaseInstance].bonesmatrices[vboneid[3]] * vweight[3];
    }

    mat4 rendermatrix = GetResource(Camera, GetUniformInd()).proj * GetResource(Camera, GetUniformInd()).view * GetResource(Instance, GetInstanceInd()).instances[gl_BaseInstance].rendermatrix;

    vec4 position = bonetransforms * vec4(vposition.xyz, 1.0f);
    gl_Position = rendermatrix * position;

   // debugPrintfEXT("instanceIndex=%d | %d\n", gl_BaseInstance, GetUniformInd());
    outcoord = vuv;
    outweight = vweight;
    outboneid = vboneid;
    mat4 m = bonetransforms * GetResource(Instance, GetInstanceInd()).instances[gl_BaseInstance].rendermatrix;
    vec4 p = m * vec4(vposition.xyz, 1.0);
    outpos = p;
    outnormal = vec4(GetResource(Instance, GetInstanceInd()).instances[gl_BaseInstance].rendermatrix * vec4(vnormal, 0.0)).xyz ;// transpose(inverse(mat3(m))) * vnormal;
}
