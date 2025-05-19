#pragma once

#include "anthraxAI/gameobjects/gameobjects.h"
#include "anthraxAI/gameobjects/objects/gizmo.h"

namespace Keeper
{
#define LIGHT_TYPE \
    X(GLOBAL, "Global") \
    X(POINT, "Point") \
    X(LIGHT_SIZE, "light size") \

#define X(element, name) element,
    typedef enum {
       LIGHT_TYPE 
    } LightTypes;
#undef X

    static std::string GetValue(const LightTypes id)
    {
        std::string retval;
#define X(element, name) if (id == element) { retval = name; } else
    LIGHT_TYPE
#undef X
        {
            retval = "undef";
        }
        return retval;
    }
    static LightTypes GetKey(const std::string& id)
    {
        LightTypes retval;
#define X(element, name) if (id == name) { retval = element; } else
    LIGHT_TYPE
#undef X
        {
            retval = POINT;
        }
        return retval;
    }

  class Light : public Objects
  {
        public:
            Light() {}
            Light(const Info& info, const std::string& tag);

            ~Light() {}

            Keeper::Type GetType() const override { return ObjectType; }
            void SetSelected(bool id) override { Selected = id; }
            void SetVisible(bool vis) override { Visible = vis; }
            bool IsVisible() const override{ return Visible; }
            void Update() override;
            void PrintInfo() override;

            void SetGizmo(Keeper::Objects* gizmo) override { GizmoHandle = reinterpret_cast<Keeper::Gizmo*>(gizmo); }

            Keeper::Objects* GetGizmo() const override { return  reinterpret_cast<Keeper::Objects*>(GizmoHandle);}
            Vector3<float> GetPosition() const override { return Position; }
            Vector3<float> GetColor() const override { return Color; }

            const std::string& GetModelName() const override { return ModelName; }
            const std::string& GetTextureName() const override { return TextureName; }
            const std::string& GetMaterialName() const override { return MaterialName; }
            const std::string& GetFragmentName() const override { return Fragment; }
            const std::string& GetVertexName() const override { return Vertex; }

            const std::string& GetParsedID() const override { return ParsedID; }
            uint32_t LightType() const override { return static_cast<uint32_t>(Type); }
        private:
            Keeper::Type ObjectType = Keeper::Type::LIGHT;
            LightTypes Type = LightTypes::POINT;

            Vector3<float> Position;
            Vector3<float> Color;
            bool ResetMouse = false;

            std::string ParsedID = "";
            std::string Vertex;
            std::string Fragment;
            std::string TextureName;
            std::string MaterialName;
            std::string ModelName;
            bool Visible = true;
            bool Selected = false;

            bool reset = true;

            Keeper::Gizmo* GizmoHandle = nullptr;
  };
}
