#pragma once

// Hook installation. The memory and call primitives moved to
// Platform/MemUtils.h, which the SDK depends on; nothing below this layer needs
// to install a hook, so these stayed here.
#include "Platform/MemUtils.h"

const std::uint32_t JUMP_OPCODE = 0xE9;
const std::uint32_t NOP_OPCODE = 0x90;

#pragma pack(push, 1)
struct SHook
{
	unsigned char jumpOpCode;
	unsigned int jumpLocation;
	unsigned char possibleNops[47] = { 0 }; // maximum size for nops

	SHook()
	{
		jumpOpCode = 0xE9;
		jumpLocation = 0;
	}
};
#pragma pack(pop)

template<typename T>
void HookInstall(DWORD installAddress, T addressToJumpTo, int iJmpCodeSize = 5)
{
	DWORD dwAddressToJumpTo;
	_asm
	{
		mov		eax, addressToJumpTo
		mov		dwAddressToJumpTo, eax
	}

	const DWORD x86FixedJumpSize = 5;
	SHook theHook;

	theHook.jumpLocation = (DWORD)dwAddressToJumpTo - (DWORD)installAddress - (DWORD)x86FixedJumpSize;
	memset(theHook.possibleNops, 0x90, iJmpCodeSize - x86FixedJumpSize);

	DWORD dwProtect[2];
	VirtualProtect((void*)installAddress, 5, PAGE_EXECUTE_READWRITE, &dwProtect[0]);
	memcpy((void*)installAddress, &theHook, iJmpCodeSize);
	VirtualProtect((void*)installAddress, 5, dwProtect[0], &dwProtect[1]);
}
