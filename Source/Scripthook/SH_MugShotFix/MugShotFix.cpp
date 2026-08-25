#include "MugShotFix.h"

// Addons
#include "Addons/Hook.h"
#include "Addons/Settings.h"
#include "Addons/tConsole.h"

#include <algorithm>
#include <cstdint>

namespace
{
	// Game globals (Steam exe)

	// The two "push 128" immediates inside MobFaceManager::LoadResource
	// (0x9BD020) that give TX_AllocTexture the head shot dimensions. The call
	// is __cdecl, so the arguments are pushed right to left:
	//
	//   0x9BD100  push -1     reservedTexture
	//   0x9BD102  push 1      numMipLevels
	//   0x9BD104  push 128    height
	//   0x9BD109  push 128    width
	//   0x9BD10E  push 0x18   options
	//   0x9BD110  push 3      format (A8R8G8B8)
	//   0x9BD112  call TX_AllocTexture
	//
	// Only the operands are touched; the surrounding call is left alone, so
	// everything downstream - the render target switch in Pres_DirectRenderModel
	// and the UI quad that samples it - keeps working off the texture's own
	// recorded dimensions.
	constexpr uintptr_t HEADSHOT_SIZE_PUSHES[] = { 0x9BD104, 0x9BD109 };

	// push imm32, so the operand starts one byte in
	constexpr uint8_t PUSH_IMM32_OPCODE = 0x68;
	constexpr uintptr_t PUSH_IMM32_OPERAND_OFFSET = 1;

	// What both operands hold in the stock exe
	constexpr uint32_t STOCK_HEADSHOT_SIZE = 128;

	// Below the stock size there is nothing to gain, and 4096 is the largest
	// square render target that D3D9 hardware is reliably guaranteed to take.
	// The texture is A8R8G8B8 with a single mip, so the cost is size * size * 4:
	// 1 MB at 512, 4 MB at 1024, 64 MB at 4096.
	constexpr uint32_t MIN_HEADSHOT_SIZE = STOCK_HEADSHOT_SIZE;
	constexpr uint32_t MAX_HEADSHOT_SIZE = 4096;

	// Confirms a site still looks like the stock instruction, so a different
	// build of the exe gets a log line rather than a corrupted instruction.
	bool IsStockSizePush(const uintptr_t InPushAddress)
	{
		const uint8_t Opcode = *reinterpret_cast<const uint8_t*>(InPushAddress);
		const uint32_t Operand = *reinterpret_cast<const uint32_t*>(InPushAddress + PUSH_IMM32_OPERAND_OFFSET);

		return Opcode == PUSH_IMM32_OPCODE && Operand == STOCK_HEADSHOT_SIZE;
	}
}

void Mod::MugShotFix::StaticApplyHooks()
{
	const MugShotTuning& Tuning = Settings::GetCheckedRef().GetMugShotTuning();
	if (!Tuning.bEnable)
	{
		return;
	}

	const uint32_t Resolution = std::clamp(Tuning.Resolution, MIN_HEADSHOT_SIZE, MAX_HEADSHOT_SIZE);
	if (Resolution != Tuning.Resolution)
	{
		tConsole::fPrintf("MugShotFix: resolution %u out of range, using %u", Tuning.Resolution, Resolution);
	}

	if (Resolution == STOCK_HEADSHOT_SIZE)
	{
		tConsole::fWriteLine("MugShotFix: resolution matches the stock size, leaving the head shot alone");
		return;
	}

	// Check both sites before writing either, so a partial patch can never
	// leave the texture with a width and height that disagree.
	for (const uintptr_t PushAddress : HEADSHOT_SIZE_PUSHES)
	{
		if (!IsStockSizePush(PushAddress))
		{
			tConsole::fPrintf("MugShotFix: unexpected instruction at 0x%X, skipping fix", PushAddress);
			return;
		}
	}

	for (size_t PushIndex = 0; PushIndex < ARRAYSIZE(HEADSHOT_SIZE_PUSHES); ++PushIndex)
	{
		const uintptr_t OperandAddress = HEADSHOT_SIZE_PUSHES[PushIndex] + PUSH_IMM32_OPERAND_OFFSET;
		if (MemUtils::WriteMemory<uint32_t>(OperandAddress, Resolution))
		{
			continue;
		}

		tConsole::fPrintf("MugShotFix: could not write head shot size at 0x%X, skipping fix", OperandAddress);

		// Put back whatever already went in, a mismatched width and height
		// would stretch the portrait rather than just leave it soft.
		for (size_t RollbackIndex = 0; RollbackIndex < PushIndex; ++RollbackIndex)
		{
			MemUtils::WriteMemory<uint32_t>(HEADSHOT_SIZE_PUSHES[RollbackIndex] + PUSH_IMM32_OPERAND_OFFSET, STOCK_HEADSHOT_SIZE);
		}

		return;
	}

	tConsole::fPrintf("MugShotFix: head shot capture raised from %ux%u to %ux%u (%u KB)",
		STOCK_HEADSHOT_SIZE, STOCK_HEADSHOT_SIZE, Resolution, Resolution, (Resolution * Resolution * 4) / 1024);
}
