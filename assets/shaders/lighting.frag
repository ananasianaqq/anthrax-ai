#version 450

#include "defines/defines.h"
#include "defines/fragdef.h"

#extension GL_EXT_debug_printf : enable
const float constant = 1.0f;
const float linear = 0.7f;
const float quadratic = 1.8f;
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
    vec3 lighting = albedo * 0.1;
    vec3 view_dir = normalize(cam_pos - position.xyz);

    int point_size = GetResource(Camera, GetUniformInd()).point_light_size;
    int j = 0;
    for (int i = 0; i < point_size; i++) {
        if (j >= 4) {
            j = 0;
        }
        vec3 point_lights = GetResource(Camera, GetUniformInd()).point_light_pos[i].xyz;
        vec3 point_color  = GetResource(Camera, GetUniformInd()).point_light_color[i].xyz;
        float point_radius = GetResource(Camera, GetUniformInd()).point_light_radius[i][j];
        j++;

        float distance = length(point_lights - position.xyz);
    debugPrintfEXT("%d, dist=%f | radius=%f\n", i, distance, point_radius);
       if(distance < point_radius) {
            vec3 point_light_dir = normalize(point_lights.xyz - position.xyz);
            vec3 diffuse_fin = max(dot(normal.xyz, point_light_dir), 0.0) * albedo * point_color;
            vec3 halfway_dir = normalize(point_light_dir + view_dir); 
            float spec = pow(max(dot(normal.xyz, halfway_dir), 0.0), 2.0);
            vec3 specular_fin = point_color * spec * specular;
            float attenuation = 1.0 / (1.0 + linear * distance + quadratic * distance * distance);
            diffuse_fin *= attenuation;
            specular_fin *= attenuation;
            lighting += diffuse_fin + specular_fin;
        }
        //vec3 diffuse = max(dot(normal.xyz, point_light_dir), 0.0) * albedo * point_color;
        //lighting += diffuse;
       
    }
   /* specular_fin *= attenuation;
    diffuse_fin *= attenuation;
    albedo_fin *= attenuation * vec3(1,0,0);
    vec3 color = albedo_fin + diffuse_fin + specular_fin;    */
    outfragcolor = vec4(lighting.xyz, 1);
}
