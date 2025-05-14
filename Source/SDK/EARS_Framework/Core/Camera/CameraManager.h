#pragma once

// SDK
#include "SDK/EARS_Common/RwMaths.h"
#include "SDK/EARS_Common/Singleton.h"
#include "SDK/EARS_Framework/Core/EventHandler/CEventHandler.h"

namespace EARS
{
	namespace Framework
	{
		class CameraBlender;

		/**
		 * Core Camera class to maintain viewports and POV for the engine.
		 */
		class CameraManager : public RWS::CEventHandler, public Singleton<CameraManager>
		{
		public:

			/**
			 * Allows the Camera Manager the ability to update
			 * NB: Internally, this is a counter. Calling EnableUpdate() increments this Counter
			 */
			void EnableUpdate();

			/**
			 * Allows the Camera Manager the ability to update
			 * NB: Internally, this is a counter. Calling DisableUpdate() decrements this Counter
			 */
			void DisableUpdate();

			/**
			 * Fetch the active Field of View for the specific Player
			 */
			bool GetActiveCamFov(float& OutFov, uint32_t InPlayerID) const;

			/**
			 * Fetch the active rotation for the specific Player.
			 */
			bool GetActiveCamRot(RwV3d& OutRotation, uint32_t InPlayerID) const;

			/**
			 * Fetch the instance of this class
			 */
			static CameraManager* GetInstance();

		private:

			// convert PlayerID to ViewportID. Useful when requesting blender
			uint32_t ViewportIDFromPlayerID(uint32_t InPlayerID) const;

			CameraBlender* m_CameraBlenders = nullptr;				// 0x10
			char m_Padding_CameraMgr[0x13C];
			uint32_t m_MaxViewports = 0;							// 0x150
			uint32_t m_NumViewports = 0;							// 0x154
			uint32_t m_PlayerIDs[4];								// 0x158
			char m_Padding2_CameraMgr[0x4];
			uint32_t m_DisableUpdate = 0;							// 0x16C

		};

		static_assert(sizeof(EARS::Framework::CameraManager) == 0x170, "EARS::Framework::CameraManager must equal 0x170");
	} // Framework
} // EARS
