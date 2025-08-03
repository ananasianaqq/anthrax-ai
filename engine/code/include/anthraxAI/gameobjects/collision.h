#pragma once

#include "anthraxAI/utils/defines.h"

#include "anthraxAI/gameobjects/objects/npc.h"
#include "anthraxAI/gameobjects/objects/camera.h"

namespace Gfx {
 struct Vertex {
        glm::vec4 position;
        glm::vec3 normal;
        glm::vec3 color;
        glm::vec2 uv;
        float weights[4];
        int boneID[4];
    };
    
    struct ComputeVertex {
        glm::vec2 position;
        glm::vec2 velocity;
        glm::vec4 color;
    };

}

namespace Keeper
{ 
    namespace Collision
    {
        struct AABB {
            glm::vec3 min = {};
            glm::vec3 max = {};
        };
    
        bool Cull(const glm::mat4& vp, const Keeper::Objects* data);
        Keeper::Collision::AABB  Create(const std::vector<Gfx::Vertex>& verts);
    }    
}

