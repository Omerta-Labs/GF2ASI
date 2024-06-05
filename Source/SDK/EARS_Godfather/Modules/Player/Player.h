#pragma once

// SDK
#include "SDK/EARS_Common/SafePtr.h"
#include "SDK/EARS_Framework/Game_Framework/Core/EventHandler/CEventHandler.h"
#include "SDK/EARS_Physics/Characters/CharacterProxy.h"

// CPP
#include <cstdint>

// TODO: Move elsewhere when we find the code file for it
struct guid128_t
{
	uint32_t a, b, c, d = 0;
};

// TODO: Move elsewhere when we find the code file for it
struct String
{
	const char* m_pCStr;
	unsigned __int32 m_len;
	unsigned __int32 m_allocLen;
	void* m_Unk0;
};

namespace EARS
{
	namespace Vehicles { class WhiteboxCar; }

	namespace Modules
	{
		class StandardDamageComponent;

		/**
		 * The Player class for The Godfather II
		 */
		class Player
		{
		public:

			void TrySwapPlayerModel(const char* AssemblyName, const char* PresetName);

			// TODO: This should be a virtual method, once we split up the Player class to PartedAnimated, Sentient, Entity etc.
			void Translate(const float X, const float Y, const float Z);

			// Getters
			EARS::Vehicles::WhiteboxCar* GetVehicle() const { return (EARS::Vehicles::WhiteboxCar*)(m_Vehicle.m_Obj - 0x12); }
			EARS::Havok::CharacterProxy* GetCharacterProxy() { return &m_CharacterProxy; }
			EARS::Modules::StandardDamageComponent* GetDamageComponent() const { return m_Damage; }

			/**
			 * Get the Local Player
			 * *NOT PART OF MAIN GAME, UTILITY FUNCTION*
			 * @return Player - The Local Player
			 */
			static Player* GetLocalPlayer();

		private:

			void* VTABLE = nullptr; // Player vtable
			char m_Padding_0[0x2AC];
			String m_PartsPresetName; // 0x2B0 // (EARS::Modules::PartedAnimated)
			String m_PartsThemeName; // 0x2C0 // (EARS::Modules::PartedAnimated)
			String m_PartsSwapSetName; // 0x2D0 // (EARS::Modules::PartedAnimated)
			String m_PartsAssemblyName; // 0x2E0 // (EARS::Modules::PartedAnimated)
			void* m_PartsPresetInstance = nullptr; // 0x2F0 - 0x2F4 // (EARS::Modules::PartedAnimated)
			char m_Padding_1[0xCC];
			EARS::Havok::CharacterProxy m_CharacterProxy;		// 0x3C0 - 0x4AC // (EARS::Modules::Sentient)
			char m_Padding_2[0x148];
			EARS::Modules::StandardDamageComponent* m_Damage = nullptr; // 0x5F4 - 0x5F8 (EARS::Modules::Sentient)
			char m_Padding_3[0x1848];
			SafePtr<EARS::Vehicles::WhiteboxCar> m_Vehicle;					// 0x1E40 - 0x1E48
			char m_Padding_4[0x648];
			uint32_t m_CurrentModelType = 0;		// 0x2490 - 0x2494
			uint32_t m_PlayerID = 0;				// 0x2494 - 0x2498
			uint32_t m_ControllerID = 0;			// 0x2498 - 0x249C
			uint32_t m_PlayerFlags[3];				// 0x249C - 0x24A8
			char m_Padding_5[0x460];
			guid128_t m_ModelGUID;
			uint32_t m_ModelStreamHandle = 0;
			guid128_t m_PlayerRCB_GUID; 
			guid128_t m_FatherModelGUID;
			guid128_t m_FatherRCB_GUID;
			int32_t m_PlayerDisableCount = 0;

		};
	} // Modules
} // EARS
