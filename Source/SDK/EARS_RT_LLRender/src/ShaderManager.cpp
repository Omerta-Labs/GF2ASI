#include "SDK/EARS_RT_LLRender/include/ShaderManager.h"

#include "Addons/Hook.h"
#include "Addons/tConsole.h"

// C++
#include <d3d9.h>
#include <d3dx9shader.h>
#include <format>

// internal type to describe a registered shader
struct LS_Shader
{
	void* shader = nullptr;
	ID3DXConstantTable* constantTable = nullptr;
	IDirect3DVertexDeclaration9* vertDeclaration = nullptr;
	uint8_t Unk0;
	uint8_t Unk1;
	uint8_t Unk2;
	uint8_t Unk3;
};

struct SM_ShaderData
{
	const char* m_Name = nullptr;
	const void* m_VSCode = nullptr;
	void* m_Unk8 = nullptr; // Should be vertex definition
	uint32_t m_UnkC = 0; // should be vertex related
	uint32_t m_Unk10 = 0; // Should be vertex related
	const void* m_PSCode = nullptr;
	void* m_Unk18 = nullptr; // unknown
	void* m_Unk1C = nullptr; // unknown;
	uint8_t m_Unk20 = 0; // flag
	uint8_t m_Unk21 = 0; // flag
	uint16_t m_Padding = 0; // padding to complete struct (eg. align)
};

hook::Type<uint32_t> ShaderWrapperListCount = hook::Type<uint32_t>(0x12058F8);
hook::Type<SM_Shader_Base*> pShaderWrapperList = hook::Type<SM_Shader_Base*>(0x12058EC);
hook::Type<LS_Shader[2048]> LS_ShaderList = hook::Type<LS_Shader[2048]>(0x11EB900);
hook::Type<SM_ShaderData[38]> kShaderData = hook::Type<SM_ShaderData[38]>(0xE7B3C0);

uint32_t SM_ShaderCount()
{
	return ShaderWrapperListCount.get();
}

SM_Shader_Base* SM_FindShaderWrapper(const uint32_t NameHash)
{
	for (SM_Shader_Base* Shader = pShaderWrapperList.get(); Shader; Shader = Shader->pNext)
	{
		if (Shader->lowerNameHash == NameHash)
		{
			return Shader;
		}
	}

	return nullptr;
}

SM_Shader_Base* SM_FindShaderWrapperByNumber(const uint32_t ShaderIdx)
{
	for (SM_Shader_Base* Shader = pShaderWrapperList.get(); Shader; Shader = Shader->pNext)
	{
		if (Shader->shaderNumber == ShaderIdx)
		{
			return Shader;
		}
	}

	return nullptr;
}

void TestLSShader()
{
#if DEBUG
	const LS_Shader* Shaders = LS_ShaderList.ptr();

	typedef HRESULT(WINAPI* pfnDisasm)(const DWORD*, BOOL, const char*, ID3DXBuffer**);
	pfnDisasm pDisasm = (pfnDisasm)GetProcAddress(GetModuleHandle("d3dx9_40.dll"), "D3DXDisassembleShader");

	for (SM_Shader_Base* Shader = pShaderWrapperList.get(); Shader; Shader = Shader->pNext)
	{
		const char* Name = Shader->GetName();
		const LS_Shader& VertexShader = Shaders[Shader->psHandle];
		const LS_Shader& ProgramShader = Shaders[Shader->vsHandle];

		const std::string ShaderPath = "J://GF2_Shaders//";

		if (VertexShader.shader)
		{
			IDirect3DVertexShader9* AsVertexShader = reinterpret_cast<IDirect3DVertexShader9*>(VertexShader.shader);

			uint32_t size = 0;
			AsVertexShader->GetFunction(nullptr, &size);

			uint8_t* vtxdata = new uint8_t[size];
			AsVertexShader->GetFunction(vtxdata, &size);

			const std::string Path = std::format("{}{}_Vertex.bin", ShaderPath, Name);
			FILE* Stream = fopen(Path.data(), "wb");
			fwrite(vtxdata, 1, size, Stream);
			fclose(Stream);

			const std::string Path2 = std::format("{}{}_Vertex_decomp.bin", ShaderPath, Name);

			ID3DXBuffer* buf = NULL;
			pDisasm((DWORD*)vtxdata, FALSE, NULL, &buf);
			FILE* StreamR = fopen(Path2.data(), "w");
			fwrite(buf->GetBufferPointer(), 1, buf->GetBufferSize(), StreamR);
			fclose(StreamR);
			buf->Release();

			delete[] vtxdata;
		}

		if (ProgramShader.shader)
		{
			IDirect3DPixelShader9* AsPixelShader = reinterpret_cast<IDirect3DPixelShader9*>(ProgramShader.shader);

			uint32_t size = 0;
			AsPixelShader->GetFunction(nullptr, &size);

			uint8_t* vtxdata = new uint8_t[size];
			AsPixelShader->GetFunction(vtxdata, &size);

			const std::string Path = std::format("{}{}_Pixel.bin", ShaderPath, Name);
			FILE* Stream = fopen(Path.data(), "wb");
			fwrite(vtxdata, 1, size, Stream);
			fclose(Stream);

			const std::string Path2 = std::format("{}{}_Pixel_decomp.bin", ShaderPath, Name);

			ID3DXBuffer* buf = NULL;
			pDisasm((DWORD*)vtxdata, FALSE, NULL, &buf);
			FILE* StreamR = fopen(Path2.data(), "w");
			fwrite(buf->GetBufferPointer(), 1, buf->GetBufferSize(), StreamR);
			fclose(StreamR);
			buf->Release();

			delete[] vtxdata;
		}

		tConsole::fPrintf("START_SHADER");
		tConsole::fWriteLine(Name);

		if (ID3DXConstantTable* pConstantTable = VertexShader.constantTable)
		{
			D3DXCONSTANTTABLE_DESC Desc1;
			pConstantTable->GetDesc(&Desc1);
			for (UINT i = 0; i < Desc1.Constants; ++i)
			{
				D3DXHANDLE hConstant = pConstantTable->GetConstant(NULL, i);  // NULL = top-level
				if (hConstant == nullptr) continue;

				D3DXCONSTANT_DESC desc;
				UINT count = 1;  // GetConstantDesc may return more than one if the constant is an array

				HRESULT hr = pConstantTable->GetConstantDesc(hConstant, &desc, &count);
				if (SUCCEEDED(hr)) {
					// Print constant info
					tConsole::fPrintf("%s %d %u %u %d %d",
						desc.Name,
						desc.RegisterSet,
						desc.RegisterIndex,
						desc.RegisterCount,
						desc.Type,
						desc.Class);
				}
			}

			int z = 0;
		}

		if (ID3DXConstantTable* pConstantTable = ProgramShader.constantTable)
		{
			D3DXCONSTANTTABLE_DESC Desc1;
			pConstantTable->GetDesc(&Desc1);
			for (UINT i = 0; i < Desc1.Constants; ++i)
			{
				D3DXHANDLE hConstant = pConstantTable->GetConstant(NULL, i);  // NULL = top-level
				if (hConstant == nullptr) continue;

				D3DXCONSTANT_DESC desc;
				UINT count = 1;  // GetConstantDesc may return more than one if the constant is an array

				HRESULT hr = pConstantTable->GetConstantDesc(hConstant, &desc, &count);
				if (SUCCEEDED(hr)) {
					// Print constant info
					tConsole::fPrintf("%s %d %u %u %d %d",
						desc.Name,
						desc.RegisterSet,
						desc.RegisterIndex,
						desc.RegisterCount,
						desc.Type,
						desc.Class,
						desc.DefaultValue);
				}
			}
		}

		tConsole::fWriteLine("END_SHADER");
	}
#endif // DEBUG
}

void TestLSShaderData()
{
	SM_ShaderData* StartPtr = kShaderData.ptr();
	for (uint32_t Idx = 0; Idx < 38; Idx++)
	{
		const SM_ShaderData& CurrentShader = StartPtr[Idx];
		int z = 0;
	}
}
