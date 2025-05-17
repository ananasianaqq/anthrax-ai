#pragma once

#include <atomic>
#include <cstdint>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <vector>
#include <map>

#include "anthraxAI/utils/defines.h"
#include "anthraxAI/utils/mathdefines.h"

namespace Keeper {

    struct Info {
        Vector3<float> Position;
        Vector3<float> Color;
        Vector3<float> Offset;
        float Radius = 0.0f;
        std::string Material;
        std::string Vertex;
        std::string Fragment;
        std::vector<std::string> Textures;
        std::string Texture;
        std::string Model;
        std::string Mesh;
        std::vector<std::string> Animations;
        std::string ParsedID = "";
        bool Spawn = false;
        bool IsModel = false;
        bool IsLight = false;
        bool VertexBase = false;
    };

    enum Type {
        CAMERA = 0,
        SPRITE,
        NPC,
        GIZMO,
        TEST,
        LIGHT,
        SIZE
    };

    enum Infos {
        INFO_INTRO = 0,
        INFO_GRID,
        INFO_MASK,
        INFO_OUTLINE,
        INFO_GBUFFER,
        INFO_LIGHTING,
        INFO_SIZE
    };

    class Objects
    {
        public:
            Objects() { UniqueID = IDCounter; IDCounter++; }
            Objects(const Info& info) { UniqueID = IDCounter; IDCounter++; }
            Objects(const Info& info, int axis) { UniqueID = IDCounter; IDCounter++; }
            virtual ~Objects() {}

            virtual Type GetType() const { return SIZE; }
            virtual int GetAxis() const { return -1; }
            virtual uint32_t CameraType() const { return -1; }

            virtual std::string GetModelName() const { return ""; }
            virtual std::string GetTextureName() const { return ""; }
            virtual std::string GetFragmentName() const { return ""; }
            virtual std::string GetVertexName() const { return ""; }
            virtual std::string GetMaterialName() const { return ""; }
            virtual Vector3<float> GetPosition() const { return {0.0f, 0.0f, 0.0f}; }
            virtual Vector3<float> GetColor() const { return {0.0f, 0.0f, 0.0f}; }
            virtual float GetRadius() const { return 0.0f; }
            virtual const std::vector<std::string>& GetAnimations() const { return EmptyAnimations; }

            virtual Keeper::Objects* GetHandle() const { return nullptr; }
            virtual Keeper::Objects* GetGizmo() const { return  nullptr;}

            virtual void SetTextureName(const std::string& str) {}
            virtual void SetSelected(bool id) { }
            virtual void SetGizmo(Keeper::Objects* gizmo) {}
            virtual void SetPosition(const Vector3<float> pos) { }
            virtual void SetHandle(Keeper::Objects* id) { }
            virtual void SetVisible(bool vis) { }
            virtual bool IsVisible() const { return true; }
            virtual bool HasAnimations() const { return false; }

            virtual void Update() {}

            virtual void PrintInfo() {}

            virtual int GetID() const { return UniqueID; }
            virtual const std::string& GetParsedID() const { return ParsedID; }
            virtual void ResetCounterID() { IDCounter = 1; }

        private:
            std::string ParsedID = "";
            std::vector<std::string> EmptyAnimations;
            int UniqueID = 1;
            inline static std::atomic_int IDCounter = 1;
    };

    typedef std::map<int, std::vector<Objects*>> GameObjectsMap;

    class Base
    {
        public:
            Base();
            ~Base();
            void CleanIfNot(Keeper::Type type, bool resetID = false);
            template<typename T>
            void Create(T* type) { ASSERT(type->GetType() == SIZE, "Keeper::Base::Create(): Error: child has no GetType() defined"); ObjectsList[type->GetType()].push_back(type); }

            bool IsValid(Type type) { if (!ObjectsList[type].empty()) { return true; } return false; }
            std::vector<Objects*>& Get(Type type) {  return ObjectsList[type]; }

            void Create(const std::vector<Info>& info);
            void Update();

            std::vector<Objects*> GetObjects(Type type) { return (ObjectsList[type]); }
            const GameObjectsMap& GetObjects() const { return ObjectsList; }

            Info& GetGizmoInfo(int type) { return GizmoInfo[type]; } ;

            void SetSelectedID(uint32_t id) { SelectedID = id; }
            uint32_t GetSelectedID() { return SelectedID; }

            size_t GetObjectsSize() const;
            void UpdateObjectNames();

            Keeper::Objects* GetNotConstObject(Keeper::Type type, int id);
            Keeper::Objects* GetNotConstObject(Keeper::Type type, const std::string& str);
            const Keeper::Objects* GetObject(Keeper::Type type, int id) const;
            const std::vector<std::string>& GetObjectNames() const { return ObjectNames; }

            Info GetInfo(Infos info) const { return DefaultObjects[info]; }

            bool Find(Keeper::Type type) const;

        private:
            void SpawnObjects(const Info& info);

            GameObjectsMap ObjectsList;
            Info GizmoInfo[3];
            Info DefaultObjects[INFO_SIZE];
            uint32_t SelectedID = 0;

            std::vector<std::string> ObjectNames;
    };
}
