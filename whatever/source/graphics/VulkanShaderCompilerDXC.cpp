#include "VulkanShaderCompilerDXC.h"
#include "util/StringUtils.h"

#include <Windows.h>
#include "VulkanShader.h"
#include <iostream>
namespace wtv
{
	VulkanShaderCompilerDXC::VulkanShaderCompilerDXC()
	{
		HRESULT hres;

		// Initialize DXC library
		hres = DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(&m_library));
		if (FAILED(hres)) {
			// Log error // throw std::runtime_error("Could not init DXC Library");
		}

		// Initialize DXC compiler
		hres = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&m_compiler));
		if (FAILED(hres)) {
			// Log error // throw std::runtime_error("Could not init DXC Compiler");
		}

		// Initialize DXC utility
		hres = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&m_utils));
		if (FAILED(hres)) {
			// Log error // throw std::runtime_error("Could not init DXC Utiliy");
		}

	}
	RefPtr<VulkanShader> VulkanShaderCompilerDXC::CreateShaderFromFile(const CompilationParams& params)
	{
		std::wstring filename = params.sourcePath.wstring();
		HRESULT hres;
		// Load the HLSL text shader from disk
		uint32_t codePage = DXC_CP_ACP;
		CComPtr<IDxcBlobEncoding> sourceBlob;
		hres = m_utils->LoadFile(filename.c_str(), &codePage, &sourceBlob);
		if (FAILED(hres)) {
			throw std::runtime_error("Could not load shader file");
		}

		// Select target profile based on shader file extension
		LPCWSTR targetProfile{};
		switch (params.stage)
		{
		case ShaderStage::Vertex:
		{
			targetProfile = L"vs_6_1";
			break;
		}
		case ShaderStage::Fragment:
		{
			targetProfile = L"ps_6_1";
			break;
		}
		default:
			assert(false);
			break;
		}

		// Configure the compiler arguments for compiling the HLSL shader to SPIR-V
		std::wstring entryPoint = StringToWstring(params.entryPoint);
		std::vector<LPCWSTR> arguments = {
			// (Optional) name of the shader file to be displayed e.g. in an error message
			filename.c_str(),
			// Shader main entry point
			L"-E", entryPoint.c_str(),
			// Shader target profile
			L"-T", targetProfile,
			// Compile to SPIRV
			L"-spirv",
			L"-fspv-debug=vulkan-with-source", // Embeds HLSL source and line info into SPIR-V
			L"-Zi",                           // Standard "Enable Debug Info" flag
			L"-Od"
		};

		// Compile shader
		DxcBuffer buffer{};
		buffer.Encoding = DXC_CP_ACP;
		buffer.Ptr = sourceBlob->GetBufferPointer();
		buffer.Size = sourceBlob->GetBufferSize();

		CComPtr<IDxcResult> result{ nullptr };
		hres = m_compiler->Compile(
			&buffer,
			arguments.data(),
			(uint32_t)arguments.size(),
			nullptr,
			IID_PPV_ARGS(&result));

		if (SUCCEEDED(hres)) {
			result->GetStatus(&hres);
		}

		// Output error if compilation failed
		if (FAILED(hres) && (result)) {
			CComPtr<IDxcBlobEncoding> errorBlob;
			hres = result->GetErrorBuffer(&errorBlob);
			if (SUCCEEDED(hres) && errorBlob) {
				// Log Error
				std::cerr << "Shader compilation failed :\n\n" << (const char*)errorBlob->GetBufferPointer();
				//throw std::runtime_error("Compilation failed");
			}
		}

		// Get compilation result
		CComPtr<IDxcBlob> code;
		result->GetResult(&code);

		VulkanShader::CreationParams shaderParams;
		shaderParams.stage = params.stage;
		shaderParams.spirvCode = (uint32_t*)code->GetBufferPointer();
		shaderParams.codeLength = code->GetBufferSize();
		shaderParams.engine = params.engine;
		shaderParams.shaderName = params.sourcePath.filename().string();
		return MakeRef<VulkanShader>(shaderParams);
	}
}