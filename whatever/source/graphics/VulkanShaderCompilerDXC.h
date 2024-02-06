#include "IVulkanShaderCompiler.h"
#include "IService.h"

#include <Windows.h>

#include <stdexcept>
#include "vulkan/vulkan.h"
#include "dxc/dxcapi.h"
#include "atlbase.h"
namespace wtv
{
	class VulkanShaderCompilerDXC : public IVulkanShaderCompiler
	{
	public:
		std::shared_ptr<VulkanShader> CreateShaderFromFile(const CompilationParams& params) override;
		VulkanShaderCompilerDXC();

	private:
		CComPtr<IDxcLibrary> m_library;
		CComPtr<IDxcCompiler3> m_compiler;
		CComPtr<IDxcUtils> m_utils;
	};
}