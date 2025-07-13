#pragma once

#include "anthraxAI/gamemodules/modules.h"
#include "anthraxAI/gameobjects/gameobjects.h"
#include "anthraxAI/gfx/renderhelpers.h"
#include "anthraxAI/utils/mathdefines.h"
#include "anthraxAI/utils/parser.h"
#include "anthraxAI/utils/thread.h"

#include "anthraxAI/gameobjects/gameobjects.h"
#include "anthraxAI/gameobjects/objects/camera.h"

#include <atomic>
#include <cstdint>
#include <string>
#include <vulkan/vulkan_core.h>
#include "anthraxAI/utils/tracy.h"

namespace Core
{
    class Scene : public Utils::Singleton<Scene>
    {
        public:
            ~Scene() { if (GameObjects) delete GameObjects; if (GameModules) delete GameModules; }
            void Init();
            void InitModules();

            void Loop();
        
            void ExportScene();
            void ExportObjectInfo(const Keeper::Objects* obj);
            void RenderScene(bool playmode);
            bool HasAnimation(uint32_t id) { if (GameModules) { return GameModules->HasAnimation(id); } return false; }
            void ReloadAnimation(uint32_t id, const std::string& s) { if (GameModules) { return GameModules->ReloadAnimation(id, s); }}

            void SetCurrentScene(const std::string& str);
            Modules::ScenesMap& GetScenes() { return GameModules->GetSceneModules(); }

            Keeper::Camera& GetCamera() { return *EditorCamera; }
            const Keeper::Base* GetGameObjects() const { return GameObjects; }
            void SetNewObjectType(const std::string& str) { GameObjects->NewObjectInfo.Type = str; }
            const std::string& GetNewObjectType() { return GameObjects->NewObjectInfo.Type; }
            void SetNewObjectLightType(const std::string& str) { GameObjects->NewObjectInfo.LightType = str; }
            const std::string& GetNewObjectLightType() { return GameObjects->NewObjectInfo.LightType; }
            void SetNewObjectParsedID(const std::string& str) { GameObjects->NewObjectInfo.ParsedID = str; }
            const std::string& GetNewObjectParsedID() { return GameObjects->NewObjectInfo.ParsedID; }
            void SetNewObjectColor(glm::vec3 position) { GameObjects->NewObjectInfo.Color = position; }
            glm::vec3 GetNewObjectColor() { return GameObjects->NewObjectInfo.Color.convert(); }
            void SetNewObjectPosition(glm::vec3 position) { GameObjects->NewObjectInfo.Position = position; }
            glm::vec3 GetNewObjectPosition() { return GameObjects->NewObjectInfo.Position.convert(); }
            void SetNewObjectTexture(const std::string& str) { GameObjects->NewObjectInfo.Texture = str; }
            const std::string& GetNewObjectTexture() { return GameObjects->NewObjectInfo.Texture; }
            void SetNewObjectMaterial(const std::string& str) { GameObjects->NewObjectInfo.Material = str; }
            const std::string& GetNewObjectMaterial() { return GameObjects->NewObjectInfo.Material; }
            void SetNewObjectModel(const std::string& str) { GameObjects->NewObjectInfo.Model = str; }
            const std::string& GetNewObjectModel() { return GameObjects->NewObjectInfo.Model; }
            void ClearNewObjectInfo();
            void DeleteSelectedObject();
            void SaveObject();// { GameObjects->Create<Keeper::Npc>(new Keeper::Npc(GameObjects->NewObjectInfo)); }        

            void ReloadResources();
            void ParseSceneNames();
            const std::vector<std::string>& GetSceneNames() const { return SceneNames; }

            void KeepEditor(bool keep) { HasEditor = keep; }
            const std::string& GetCurrentScene() const { return CurrentScene; }
            void SetSelectedID(uint32_t id) { GameObjects->SetSelectedID(id); }
            uint32_t GetSelectedID() { return GameObjects->GetSelectedID(); }

            const glm::mat4& GetGlobalTransform() const { return GameModules->GetGlobalTransform(); }
            bool RenderPassed = false;
        private:
            void PopulateModules();

            void LoadScene(const std::string& filename);
            void Render(Modules::Module& module);
            void RenderThreaded(Modules::Module& module);

            Keeper::Base* GameObjects = nullptr;
            Modules::Base* GameModules = nullptr;

            std::string CurrentScene = "intro";
            std::vector<Keeper::Info> ParsedSceneInfo;

            std::vector<std::string> SceneNames;

            Keeper::Camera* EditorCamera;

            Utils::Parser Parse;
            VkPipeline ThreadedPipeline;
            std::vector<VkCommandBuffer> sec_cmds;
            bool HasEditor = false;
            bool HasGBuffer = false;
            bool HasFrameGizmo = false;
            bool HasFrameOutline = false;
            bool HasFrameGrid = false;

    };

}
