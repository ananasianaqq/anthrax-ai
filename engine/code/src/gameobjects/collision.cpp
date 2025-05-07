#include "anthraxAI/gameobjects/collision.h"
#include "anthraxAI/core/scene.h"
#include "anthraxAI/gfx/model.h"
#include "anthraxAI/gfx/vkdevice.h"
#include "anthraxAI/gfx/vkmesh.h"
#include "anthraxAI/gfx/vkrenderer.h"
#include <complex>
#include <cstdio>

Keeper::Collision::AABB Keeper::Collision::Create(const std::vector<Gfx::Vertex>& verts)
{
    glm::vec3 vmin = glm::vec3(verts[0].position.x, verts[0].position.y, verts[0].position.z);
    glm::vec3 vmax = vmin;

    for (int i = 1; i < verts.size(); ++i) {
        const glm::vec3& cur = glm::vec3(verts[i].position.x, verts[i].position.y, verts[i].position.z);
        vmin = glm::min(vmin, cur);
        vmax = glm::max(vmax, cur);
    }
    return {vmin, vmax};
}

bool Keeper::Collision::Cull(const glm::mat4& vp, const Objects* data)
{
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(data->GetPosition().x, data->GetPosition().y, data->GetPosition().z));

    const std::vector<Gfx::MeshInfo*>&  meshes = Gfx::Model::GetInstance()->GetModel(data->GetModelName())->Meshes;
    
        bool culled = false;
    for (Gfx::MeshInfo* mesh : meshes) {
        const AABB& aabb = mesh->aabb;
    glm::vec4 corners[8] = {
        {aabb.min.x, aabb.min.y, aabb.min.z, 1.0}, 
        {aabb.max.x, aabb.min.y, aabb.min.z, 1.0}, 
        {aabb.min.x, aabb.max.y, aabb.min.z, 1.0}, 
        {aabb.max.x, aabb.max.y, aabb.min.z, 1.0}, 

        {aabb.min.x, aabb.min.y, aabb.max.z, 1.0}, 
        {aabb.max.x, aabb.min.y, aabb.max.z, 1.0}, 
        {aabb.min.x, aabb.max.y, aabb.max.z, 1.0}, 
        {aabb.max.x, aabb.max.y, aabb.max.z, 1.0}, 
    };
    
    if (data->HasAnimations()) {
            // did something stupid in order to check whether i can cull animations before calculation bone transforms
        model *= Core::Scene::GetInstance()->GetGlobalTransform() * glm::scale(glm::mat4(1.0f), glm::vec3(0.05));
    }

    glm::mat4 mvp = vp * model;


    for (size_t corner_idx = 0; corner_idx < 8; corner_idx++) {
        // Transform vertex
        glm::vec4 corner = mvp * corners[corner_idx];
        // Check vertex against clip space bounds
        culled = culled ||
            (corner.x >=  -corner.w && corner.x <= corner.w) &&
            (corner.y >=  -corner.w && corner.y <= corner.w) &&
            (corner.z >=  0 && corner.z <= corner.w);

       // if (!culled) break;
    }
    }
    return culled; 
}
