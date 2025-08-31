#include "anthraxAI/gfx/vkdescriptors.h"
#include "anthraxAI/gfx/renderhelpers.h"
#include "anthraxAI/gfx/vkdefines.h"
#include "anthraxAI/gfx/vkdevice.h"
#include "anthraxAI/gfx/vkrenderer.h"
#include <cstdint>
#include <cstdio>

size_t Gfx::DescriptorsBase::PadUniformBufferSize(size_t originalsize)
{
	size_t minalignment = Gfx::Device::GetInstance()->MinUniformBufferOffsetAlignment;
	size_t alignedsize = originalsize;
	if (minalignment > 0) {
		alignedsize = (alignedsize + minalignment - 1) & ~(minalignment - 1);
	}
	return alignedsize;
}

VkDescriptorSetLayoutBinding DescriptorLayoutBinding(VkDescriptorType type, VkShaderStageFlags stageFlags, uint32_t binding)
{
	VkDescriptorSetLayoutBinding setbind = {};
	setbind.binding = binding;
	setbind.descriptorCount = 1000;
	setbind.descriptorType = type;
	setbind.pImmutableSamplers = nullptr;
	setbind.stageFlags = stageFlags;
	return setbind;
}

void Gfx::DescriptorsBase::ClearTextures()
{
    for (int i = 0; i < MAX_FRAMES; i++) {
        TextureBindings[i].clear();
    }
    TextureHandle = 0;

    for (int i = 0; i < MAX_FRAMES; i++) {
        BufferBindings[i].clear();
    }
    BufferHandle = 0;

}

uint32_t Gfx::DescriptorsBase::UpdateTexture(VkImageView imageview, VkSampler sampler, const std::string& name, uint32_t frame)
{
    bool force_update = Gfx::Renderer::GetInstance()->GetUpdateSamplers();
    uint32_t bind;
    auto it = std::find_if(TextureBindings[frame].begin(), TextureBindings[frame].end(), [&, name](const auto& n) { return n.first == name; });
	if (!force_update && it != TextureBindings[frame].end()) {
        return it->second;
    }

    VkDescriptorImageInfo imageinfo{};
	imageinfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	imageinfo.imageView = imageview;
	imageinfo.sampler = sampler;

	VkWriteDescriptorSet write{};
	write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	write.dstBinding = TextureBinding;
	write.dstSet = BindlessDescriptor[frame];
	write.descriptorCount = 1;
	write.pImageInfo = &imageinfo;
    
    bind = TextureHandle;
    if (force_update && it != TextureBindings[frame].end()) {
        bind = it->second;
    }
	write.dstArrayElement = bind;

	vkUpdateDescriptorSets(Gfx::Device::GetInstance()->GetDevice(), 1, &write, 0, nullptr);

    if (force_update) {
        TextureBindings[frame][name] = bind; 
        return bind;
    }
	TextureHandle++;
    
    TextureBindings[frame][name] = TextureHandle - 1;
	return TextureHandle - 1;
}

uint32_t Gfx::DescriptorsBase::UpdateBuffer(VkBuffer buffer, VkBufferUsageFlagBits usage, const std::string& name, uint32_t frame)
{
    if (!name.empty()) {
        auto it = std::find_if(BufferBindings[frame].begin(), BufferBindings[frame].end(), [&, name](const auto& n) { return n.first == name; });
	    if (it != BufferBindings[frame].end()) {
            return it->second;
        }
    }
	VkWriteDescriptorSet writes{};
	VkDescriptorBufferInfo bufferinfo{};
	bufferinfo.buffer = buffer;
	bufferinfo.offset = 0;
	bufferinfo.range = VK_WHOLE_SIZE;
	writes.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writes.dstSet = BindlessDescriptor[frame];
	writes.descriptorCount = 1;
	writes.dstArrayElement = BufferHandle;
	writes.pBufferInfo = &bufferinfo;

	if ((usage & VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT) == VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT) {
		writes.dstBinding = UniformBinding;
		writes.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	}
	if ((usage & VK_BUFFER_USAGE_STORAGE_BUFFER_BIT) == VK_BUFFER_USAGE_STORAGE_BUFFER_BIT) {
		writes.dstBinding = StorageBinding;
		writes.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	}

	vkUpdateDescriptorSets(Gfx::Device::GetInstance()->GetDevice(), 1, &writes, 0, nullptr);
	BufferHandle++;

    if (!name.empty()) {
        BufferBindings[frame][name] = BufferHandle - 1;
    }
    return BufferHandle - 1;
}

uint32_t Gfx::DescriptorsBase::UpdateCompute(VkBuffer buffer, VkBufferUsageFlagBits usage, const std::string& name, uint32_t frame)
{
    if (!name.empty()) {
        auto it = std::find_if(ComputeBindings[frame].begin(), ComputeBindings[frame].end(), [&, name](const auto& n) { return n.first == name; });
	    if (it != ComputeBindings[frame].end()) {
            return it->second;
        }
    }
	VkWriteDescriptorSet writes{};
	VkDescriptorBufferInfo bufferinfo{};
	bufferinfo.buffer = buffer;
	bufferinfo.offset = 0;
	bufferinfo.range = VK_WHOLE_SIZE;
	writes.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	writes.dstSet = BindlessDescriptor[frame];
	writes.descriptorCount = 1;
	writes.dstArrayElement = ComputeHandle;
	writes.pBufferInfo = &bufferinfo;

	if ((usage & VK_BUFFER_USAGE_STORAGE_BUFFER_BIT) == VK_BUFFER_USAGE_STORAGE_BUFFER_BIT) {
		writes.dstBinding = ComputeBinding;
		writes.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	}

	vkUpdateDescriptorSets(Gfx::Device::GetInstance()->GetDevice(), 1, &writes, 0, nullptr);
	ComputeHandle++;

    if (!name.empty()) {
        ComputeBindings[frame][name] = ComputeHandle - 1;
    }
    return ComputeHandle - 1;
}


void Gfx::DescriptorsBase::AllocateDataBuffers()
{
	const size_t cambuffersize = (sizeof(CameraData));
    for (int i = 0; i < MAX_FRAMES; i++) {
	    BufferHelper::CreateBuffer(CameraBuffer[i], cambuffersize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
        CameraBuffer[i].tag = "camera";
    }
    VkDebugUtilsObjectNameInfoEXT info;
	info.pNext = nullptr;
	info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
	info.objectHandle = reinterpret_cast<uint64_t>(CameraBuffer[0].DeviceMemory);
	info.objectType = VK_OBJECT_TYPE_DEVICE_MEMORY;;
	info.pObjectName = "data buffer #1 frame";
	Gfx::Vulkan::GetInstance()->SetDebugName(info);

    for (int i = 0; i < MAX_FRAMES; i++) {
	    Core::Deletor::GetInstance()->Push(Core::Deletor::Type::NONE, [=, this]() {
		    vkDestroyBuffer(Gfx::Device::GetInstance()->GetDevice(), CameraBuffer[i].Buffer, nullptr);
    	    vkFreeMemory(Gfx::Device::GetInstance()->GetDevice(), CameraBuffer[i].DeviceMemory, nullptr);
	    });
    }
}
void Gfx::DescriptorsBase::AllocateComputeBuffers()
{
	size_t buffersize = (sizeof(ComputeData));
    for (int i = 0; i < MAX_FRAMES; i++) {
	    BufferHelper::CreateBuffer(ComputeBuffer[i], buffersize,static_cast<VkBufferUsageFlagBits>(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT));
        ComputeBuffer[i].tag = "compute";
    }
    VkDebugUtilsObjectNameInfoEXT info;
	info.pNext = nullptr;
	info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
	info.objectHandle = reinterpret_cast<uint64_t>(ComputeBuffer[0].DeviceMemory);
	info.objectType = VK_OBJECT_TYPE_DEVICE_MEMORY;;
	info.pObjectName = "compute buffer #1 frame";
	Gfx::Vulkan::GetInstance()->SetDebugName(info);

    for (int i = 0; i < MAX_FRAMES; i++) {
    	Core::Deletor::GetInstance()->Push(Core::Deletor::Type::NONE, [=, this]() {
    		vkDestroyBuffer(Gfx::Device::GetInstance()->GetDevice(), ComputeBuffer[i].Buffer, nullptr);
        	vkFreeMemory(Gfx::Device::GetInstance()->GetDevice(), ComputeBuffer[i].DeviceMemory, nullptr);
    	});
    }
}

void Gfx::DescriptorsBase::AllocateStorageBuffers()
{
	size_t buffersize = (sizeof(StorageData));
    for (int i = 0; i < MAX_FRAMES; i++) {
	    BufferHelper::CreateBuffer(StorageBuffer[i], buffersize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
        StorageBuffer[i].tag = "storage";
    }
    VkDebugUtilsObjectNameInfoEXT info;
	info.pNext = nullptr;
	info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
	info.objectHandle = reinterpret_cast<uint64_t>(StorageBuffer[0].DeviceMemory);
	info.objectType = VK_OBJECT_TYPE_DEVICE_MEMORY;;
	info.pObjectName = "storage buffer #1 frame";
	Gfx::Vulkan::GetInstance()->SetDebugName(info);

    for (int i = 0; i < MAX_FRAMES; i++) {
    	Core::Deletor::GetInstance()->Push(Core::Deletor::Type::NONE, [=, this]() {
    		vkDestroyBuffer(Gfx::Device::GetInstance()->GetDevice(), StorageBuffer[i].Buffer, nullptr);
        	vkFreeMemory(Gfx::Device::GetInstance()->GetDevice(), StorageBuffer[i].DeviceMemory, nullptr);
    	});
    }

    buffersize = sizeof(InstanceData) * MAX_INSTANCES ;
    for (int i = 0; i < MAX_FRAMES; i++) {
	    BufferHelper::CreateBuffer(InstanceBuffer[i], buffersize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
        InstanceBuffer[i].tag = "instance";
    }
	info.pNext = nullptr;
	info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
	info.objectHandle = reinterpret_cast<uint64_t>(InstanceBuffer[0].DeviceMemory);
	info.objectType = VK_OBJECT_TYPE_DEVICE_MEMORY;;
	info.pObjectName = "INSTANCE buffer #1 frame";
	Gfx::Vulkan::GetInstance()->SetDebugName(info);

    for (int i = 0; i < MAX_FRAMES; i++) {
        Core::Deletor::GetInstance()->Push(Core::Deletor::Type::NONE, [=, this]() {
	    	vkDestroyBuffer(Gfx::Device::GetInstance()->GetDevice(), InstanceBuffer[i].Buffer, nullptr);
        	vkFreeMemory(Gfx::Device::GetInstance()->GetDevice(), InstanceBuffer[i].DeviceMemory, nullptr);
	    });
    }

#ifdef COMPUTE_MTX

    buffersize = sizeof(AnimationComputeData) * MAX_INSTANCES ;
    for (int i = 0; i < MAX_FRAMES; i++) {
	    BufferHelper::CreateBuffer(AnimationBuffer[i], buffersize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
        AnimationBuffer[i].tag = "animation";
    }
	info.pNext = nullptr;
	info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
	info.objectHandle = reinterpret_cast<uint64_t>(AnimationBuffer[0].DeviceMemory);
	info.objectType = VK_OBJECT_TYPE_DEVICE_MEMORY;;
	info.pObjectName = "animation buffer #1 frame";
	Gfx::Vulkan::GetInstance()->SetDebugName(info);

    for (int i = 0; i < MAX_FRAMES; i++) {
        Core::Deletor::GetInstance()->Push(Core::Deletor::Type::NONE, [=, this]() {
	    	vkDestroyBuffer(Gfx::Device::GetInstance()->GetDevice(), AnimationBuffer[i].Buffer, nullptr);
        	vkFreeMemory(Gfx::Device::GetInstance()->GetDevice(), AnimationBuffer[i].DeviceMemory, nullptr);
	    });
    }

#endif // COMPUTE_MTX
}

void Gfx::DescriptorsBase::CleanAll()
{
	TextureHandle = 0;
    BufferHandle = 0;
    
    ClearTextures();

    for (int i = 0; i < MAX_FRAMES; i++) {
	    vkDestroyDescriptorPool(Gfx::Device::GetInstance()->GetDevice(), Pool[i], nullptr);
    }
	vkDestroyDescriptorSetLayout(Gfx::Device::GetInstance()->GetDevice(), BindlessLayout, nullptr);
}

void Gfx::DescriptorsBase::CleanBindless()
{
	vkDestroyBuffer(Gfx::Device::GetInstance()->GetDevice(), BindlessBuffer.Buffer, nullptr);
    vkFreeMemory(Gfx::Device::GetInstance()->GetDevice(), BindlessBuffer.DeviceMemory, nullptr);
}

void Gfx::DescriptorsBase::Init()
{
// bindless descriptor setup

	VkDescriptorSetLayoutBinding bindings[MAX_BINDING];
	VkShaderStageFlags stageflags[MAX_BINDING] = {
		VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
		VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
		VK_SHADER_STAGE_FRAGMENT_BIT,
		VK_SHADER_STAGE_COMPUTE_BIT | VK_SHADER_STAGE_VERTEX_BIT,
	};
	VkDescriptorBindingFlags flags[MAX_BINDING];
	VkDescriptorType types[MAX_BINDING] = {
		VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
		VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
		VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
	};

// descriptor pool

	VkDescriptorPoolSize sizes[MAX_BINDING] = {
		{ types[0], 1000 }, { types[1], 1000 }, { types[2], 1000 }, { types[3], 1000 }
	};

	VkDescriptorPoolCreateInfo poolinfo{};
	poolinfo.flags = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT;
	poolinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolinfo.maxSets = 5000;
	poolinfo.poolSizeCount = MAX_BINDING;
	poolinfo.pPoolSizes = sizes;
    for (int i = 0; i < MAX_FRAMES; i++) {
	    vkCreateDescriptorPool(Gfx::Device::GetInstance()->GetDevice(), &poolinfo, nullptr, &Pool[i]);
    }
// bindless layout

	for (int i = 0; i < MAX_BINDING; i++) {
		bindings[i] = DescriptorLayoutBinding(types[i], stageflags[i], i);
		flags[i] = VK_DESCRIPTOR_BINDING_PARTIALLY_BOUND_BIT | VK_DESCRIPTOR_BINDING_UPDATE_AFTER_BIND_BIT;
	}

	VkDescriptorSetLayoutBindingFlagsCreateInfo bindingflags{};
	bindingflags.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_BINDING_FLAGS_CREATE_INFO;
	bindingflags.pNext = nullptr;
	bindingflags.pBindingFlags = flags;
	bindingflags.bindingCount = MAX_BINDING;

	VkDescriptorSetLayoutCreateInfo createinfo{};
	createinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	createinfo.bindingCount = MAX_BINDING;
	createinfo.pBindings = bindings;
	createinfo.flags = VK_DESCRIPTOR_SET_LAYOUT_CREATE_UPDATE_AFTER_BIND_POOL_BIT;
	createinfo.pNext = &bindingflags;

	vkCreateDescriptorSetLayout(Gfx::Device::GetInstance()->GetDevice(), &createinfo, nullptr, &BindlessLayout);

// bindless descriptor

    for (int i = 0; i < MAX_FRAMES; i++) {
    	VkDescriptorSetAllocateInfo allocinfo{};
    	allocinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    	allocinfo.pNext = nullptr;
    	allocinfo.descriptorPool = Pool[i];
    	allocinfo.pSetLayouts = &BindlessLayout;
    	allocinfo.descriptorSetCount = 1;
    	vkAllocateDescriptorSets(Gfx::Device::GetInstance()->GetDevice(), &allocinfo, &BindlessDescriptor[i]);
    }
}

void Gfx::DescriptorsBase::AllocateBuffers()
{
    AllocateDataBuffers();
    AllocateStorageBuffers();
    AllocateComputeBuffers();
}
