#pragma once

// C++
#include <stdint.h>

struct SM_Shader_Base
{
	virtual ~SM_Shader_Base() = 0;

	virtual const char* GetName() = 0;

	SM_Shader_Base* pNext = nullptr;
	uint32_t lowerNameHash = 0;
	uint32_t shaderNumber = 0;
	uint32_t vsHandle = 0;
	uint32_t psHandle = 0;
};

/** Get number of shaders ingame */
uint32_t SM_ShaderCount();

/** Find a shader using a specific crc32 hash */
SM_Shader_Base* SM_FindShaderWrapper(const uint32_t NameHash);

/** Find a shader using a specific shader index */
SM_Shader_Base* SM_FindShaderWrapperByNumber(const uint32_t ShaderIdx);

void TestLSShader();

void TestLSShaderData();
