#pragma once

#include "anthraxAI/gfx/vkmesh.h"
#include "anthraxAI/utils/singleton.h"
#include "anthraxAI/gamemodules/modules.h"

#include <condition_variable>
#include <cstdint>
#include <cstdio>
#include <mutex>
#include <queue>
#include <functional>
#include <string>
#include <thread>
#include <map>
#include <iostream>
#include <vector>
#include <vulkan/vulkan_core.h>

namespace Thread
{
    constexpr uint32_t MAX_RENDER_THREAD_NUM = 18;
    constexpr uint32_t MAX_WORK_THREAD_NUM =12;

    struct Task {
        enum class Name {
            UPDATE,
            RENDER,
            ANIM,
            NONE
        };
        enum class Type {
            EXECUTE,
            STOP,
        };
        Name name;
        Type type;
        std::function<void(int i, Keeper::Objects* info)> func;
        std::function<void()> func1;
        std::function<void(Gfx::ModelInfo* model,Core::aiSceneInfo* scene)> func2;

        int i;
        Keeper::Objects* info;
        Gfx::RenderObject* obj;
        Gfx::MeshPushConstants* constants;
        Gfx::ModelInfo* model;
        Core::aiSceneInfo* scene;
    };
    
    struct Timing {
        
        std::map<Task::Name, std::pair<double, double>> Timings;
        void BeginTime(Task::Name name, double i) { Timings[name] = std::make_pair(i, 0); }
        void EndTime(Task::Name name, double i) { Timings[name] = std::make_pair(Timings[name].first, i); }
        void PrintTime(Task::Name name) {
            std::string str = name == Task::Name::RENDER ? "RENDER" : "UPDATE";
            printf("Task:[%s] worked %lf\n----------\n", str.c_str(), Timings[name].second - Timings[name].first); 
        }
        
        std::string GetTime(Task::Name name) {
            std::string str = name == Task::Name::RENDER ? "RENDER" : "UPDATE";
            return "Task:[" + str + "] worked " +  std::to_string(Timings[name].second - Timings[name].first); 
        }

    }; 
    static Timing Time = {};

    
    class Pool : public Utils::Singleton<Pool>
    {
        public:
            Pool() {}
            ~Pool() { Stop(); }
            void Init(int num);
            bool IsInit() const { return !Threads.empty(); }
            bool IsPaused() const { return OnPause; }
            void Stop();
            void Reload();
            bool Push(const Task& func);
            bool PushByID(int id, const Task& func);
            void Pause(bool p) { OnPause = p; }
            void WaitRender();
            void WaitWork();
            Timing Time;
        private:
            std::mutex Mutex[MAX_WORK_THREAD_NUM];

            std::mutex RenderMutex[MAX_RENDER_THREAD_NUM];
            std::condition_variable WorkCondition[MAX_WORK_THREAD_NUM];
            std::condition_variable RenderCondition[MAX_RENDER_THREAD_NUM];
            bool Done = false;
            bool OnPause = false;

            void Work(int id);
            void WorkRender(int id);
            void Process(const Task& task);
            std::vector<std::thread> Threads;
            std::vector<std::queue<Task>>  Queue;
            std::vector<std::queue<Task>> RenderQueue;
            std::queue<Task> RenderQueue2;
            std::vector<std::thread> RenderThreads;

            int ThreadCounter = 0;
    };

}
