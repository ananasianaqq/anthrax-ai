#pragma once

#include "anthraxAI/gfx/renderhelpers.h"
#include "anthraxAI/utils/defines.h"
#include "anthraxAI/gfx/vkdefines.h"

#include "anthraxAI/gfx/bufferhelper.h"

#include <cstdint>
#include <unordered_map>
#include <algorithm>

#define MAX_BINDING 4

static constexpr uint32_t UniformBinding = 0;
static constexpr uint32_t StorageBinding = 1;
static constexpr uint32_t TextureBinding = 2;
static constexpr uint32_t ComputeBinding = 3;

namespace Gfx
{
    enum DescriptorSetLayoutEnum {
        DESC_SET_LAYOUT_GLOBAL = 0,
        DESC_SET_LAYOUT_SAMPLER,
        DESC_SET_LAYOUT_STORAGE,
        DESC_SET_LAYOUT_TRANSFORMS
    };

    class DescriptorsBase : public Utils::Singleton<DescriptorsBase>
    {
        public:
            void Init();
            void CleanAll();
            void CleanBindless();

            void AllocateBuffers();

            VkDeviceMemory GetCameraBufferMemory(uint32_t frame) const { return CameraBuffer[frame].DeviceMemory; }
            VkBuffer GetCameraBuffer(uint32_t frame) const { return CameraBuffer[frame].Buffer; }
            BufferHelper::Buffer& GetCameraUBO(uint32_t frame) { return CameraBuffer[frame]; }

            VkBuffer GetStorageBuffer(uint32_t frame) const { return StorageBuffer[frame].Buffer; }
            BufferHelper::Buffer& GetStorageUBO(uint32_t frame) { return StorageBuffer[frame]; }
            VkDeviceMemory GetStorageBufferMemory(uint32_t frame) const { return StorageBuffer[frame].DeviceMemory; }
            
            VkBuffer GetComputeBuffer(uint32_t frame) const { return ComputeBuffer[frame].Buffer; }
            BufferHelper::Buffer& GetComputeUBO(uint32_t frame) { return ComputeBuffer[frame]; }
            VkDeviceMemory GetComputeBufferMemory(uint32_t frame) const { return ComputeBuffer[frame].DeviceMemory; }

            VkBuffer GetInstanceBuffer(uint32_t frame) const { return InstanceBuffer[frame].Buffer; }
            BufferHelper::Buffer& GetInstanceUBO(uint32_t frame) { return InstanceBuffer[frame]; }
            VkDeviceMemory GetInstanceBufferMemory(uint32_t frame) const { return InstanceBuffer[frame].DeviceMemory; }

#ifdef COMPUTE_MTX
            VkBuffer GetAnimationBuffer(uint32_t frame) const { return AnimationBuffer[frame].Buffer; }
            BufferHelper::Buffer& GetAnimationUBO(uint32_t frame) { return AnimationBuffer[frame]; }
            VkDeviceMemory GetAnimationBufferMemory(uint32_t frame) const { return AnimationBuffer[frame].DeviceMemory; }
#endif
            size_t PadUniformBufferSize(size_t originalsize);

            uint32_t UpdateTexture(VkImageView imageview, VkSampler sampler, const std::string& name, uint32_t frame);
            uint32_t UpdateBuffer(VkBuffer buffer, VkBufferUsageFlagBits usage, const std::string& name, uint32_t frame);
            uint32_t UpdateCompute(VkBuffer buffer, VkBufferUsageFlagBits usage, const std::string& name, uint32_t frame);

            VkDescriptorSet* GetBindlessSet(uint32_t frame) { return &BindlessDescriptor[frame]; }
            VkDescriptorSetLayout GetBindlessLayout() { return BindlessLayout; }
            
            void ClearTextures();
        private:
            void AllocateDataBuffers();
            void AllocateStorageBuffers();
            void AllocateComputeBuffers();

            BufferHelper::Buffer CameraBuffer[MAX_FRAMES];
            BufferHelper::Buffer StorageBuffer[MAX_FRAMES];
            BufferHelper::Buffer ComputeBuffer[MAX_FRAMES];
            BufferHelper::Buffer InstanceBuffer[MAX_FRAMES];
            BufferHelper::Buffer AnimationBuffer[MAX_FRAMES];
           
            VkDescriptorPool Pool[MAX_FRAMES];
	        VkDescriptorSetLayout BindlessLayout = VK_NULL_HANDLE;
            VkDescriptorSet BindlessDescriptor[MAX_FRAMES];

            std::map<std::string, uint32_t> TextureBindings[MAX_FRAMES];
            std::map<std::string, uint32_t> BufferBindings[MAX_FRAMES];
            std::map<std::string, uint32_t> ComputeBindings[MAX_FRAMES];
            uint32_t TextureHandle = 0;
            uint32_t BufferHandle = 0;
            uint32_t ComputeHandle = 0;

            BufferHelper::Buffer BindlessBuffer;
    };
}
