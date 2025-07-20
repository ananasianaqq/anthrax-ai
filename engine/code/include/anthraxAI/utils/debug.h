#pragma once
#include "anthraxAI/utils/defines.h"
#include "anthraxAI/utils/thread.h"
#include <fstream>
#include <string>

namespace Utils
{
    struct Debug : public Utils::Singleton<Debug>
    {
        float FPS;
        float DeltaMs;
        bool HalfSpeed = false;
        long long AnimStartMs;
        bool Grid = true;
        bool Bones = false;
        int BoneID = 0;
        bool Tracy = false;
        int DrawCalls = 0;
        std::string GetDrawCalls() { return std::to_string(DrawCalls); }
        void DebugDrawCall() { DrawCalls++; }
        void NullDrawCall() { DrawCalls = 0; }
    };

    static  bool IsBitSet(int state, int bit) { return (state & bit) != 0; }
    static  void ClearBit(int* state, int bit) { (*state &= ~bit); }
    static  void ToggleBit(int* state, int bit) { (*state ^= bit); }

    static inline void ReadFile(const std::string& filename, std::vector<char>& buffer)
    {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            ASSERT(true, "Can't open a file: " + filename);
        }

        size_t filesize = (size_t) file.tellg();
        buffer.resize(filesize);
        file.seekg(0);
        file.read(buffer.data(), filesize);
        file.close();
    }
}
