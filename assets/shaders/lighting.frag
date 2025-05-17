#version 450

#include "defines/defines.h"
#include "defines/fragdef.h"

const float constant = 1.0f;
const float linear = 0.09f;
const float quadratic = 0.032f;
void main()
{
    vec3 cam_pos = GetResource(Camera, GetUniformInd()).viewpos.xyz;

    vec3 diffuse = GetResource(Camera, GetUniformInd()).diffuse.xyz;
    vec3 specular = GetResource(Camera, GetUniformInd()).specular.xyz;
    vec3 ambient = GetResource(Camera, GetUniformInd()).ambient.xyz;

    vec3 glob_light_dir = GetResource(Camera, GetUniformInd()).global_light_dir.xyz;

    vec2 uv = incoord.xy;
    uv.y *= -1.0;

    vec4 normal = texture(textures[GetTextureInd()], uv.xy);
    normal = normalize(normal);
    vec3 position = texture(textures[GetTextureInd() + 1], uv.xy).xyz;
    vec3 albedo = texture(textures[GetTextureInd() + 2], uv.xy).xyz;
    
   /* glob_light_dir = normalize(-glob_light_dir);
    float ndotl = max(dot(normal.xyz, glob_light_dir.xyz), 0.0f);

    vec3 view_dir = normalize(cam_pos - position.xyz);
    vec3 reflect_dir = reflect(-glob_light_dir, normal.xyz);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0f), 32.0f);
    vec3 specular_fin = spec * specular;
    vec3 albedo_fin = albedo.xyz * ambient;
    vec3 diffuse_fin = diffuse * albedo.xyz * ndotl; 
    */
    float attenuation = 1.0;
    vec3 lighting = albedo * 0.1;
    vec3 view_dir = normalize(cam_pos - position.xyz);

    int point_size = GetResource(Camera, GetUniformInd()).point_light_size;
    for (int i = 0; i < point_size; i++) {
        vec3 point_lights = GetResource(Camera, GetUniformInd()).point_light_pos[i].xyz;
        vec3 point_color  = GetResource(Camera, GetUniformInd()).point_light_color[i].xyz;


        vec3 point_light_dir = normalize(point_lights.xyz - position.xyz);
        vec3 diffuse = max(dot(normal.xyz, point_light_dir), 0.0) * albedo * point_color;
        lighting += diffuse;
       
    }
   /* specular_fin *= attenuation;
    diffuse_fin *= attenuation;
    albedo_fin *= attenuation * vec3(1,0,0);
    vec3 color = albedo_fin + diffuse_fin + specular_fin;    */
    outfragcolor = vec4(lighting.xyz, 1);
}
