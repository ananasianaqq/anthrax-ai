#pragma once

#include "anthraxAI/gameobjects/gameobjects.h"
#include "anthraxAI/gameobjects/objects/gizmo.h"

namespace Keeper
{
  class Npc : public Objects
  {
        public:
            Npc() {}
            Npc(const Info& info);

            ~Npc() {}

            Type GetType() const override { return ObjectType; }
            void SetSelected(bool id) override { Selected = id; }
            void SetVisible(bool vis) override { Visible = vis; }
            bool IsVisible() const override{ return Visible; }
            void Update() override;
            void PrintInfo() override;

            bool HasAnimations() const override { return IsAnimated; }
            void SetGizmo(Keeper::Objects* gizmo) override { GizmoHandle = reinterpret_cast<Keeper::Gizmo*>(gizmo); }

            void SetTextureName(const std::string& str) override { TextureName = str; }

            Keeper::Objects* GetGizmo() const override { return  reinterpret_cast<Keeper::Objects*>(GizmoHandle);}
            Vector3<float> GetPosition() const override { return Position; }
            const std::string& GetModelName() const override { return ModelName; }
            const std::string& GetTextureName() const override { return TextureName; }
            const std::string& GetMaterialName() const override { return MaterialName; }
            const std::string& GetFragmentName() const override { return Fragment; }
            const std::string& GetVertexName() const override { return Vertex; }

            const std::string& GetParsedID() const override { return ParsedID; }
            const std::vector<std::string>& GetAnimations() const override { return Animations; }
        
            float GetAnimOffset() const override { return AnimOffset; }
        private:
            Keeper::Type ObjectType = Type::NPC;

            Vector3<float> Position;
            
            float AnimOffset = 1.0;
    bool ResetMouse = false;

            std::string ParsedID = "";
            std::string Vertex;
            std::string Fragment;
            std::string TextureName;
            std::string MaterialName;
            std::string ModelName;
            bool Visible = true;
            bool Selected = false;
            bool IsAnimated = false;

   bool reset = true;
            std::vector<std::string> Animations;

            Keeper::Gizmo* GizmoHandle = nullptr;
  };
}
