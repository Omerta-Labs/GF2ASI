#include "CameraManager.h"

#include "Addons/Hook.h"

// SDK
#include "SDK/EARS_Framework/Core/Camera/CameraBlender.h"

// C++
#include "assert.h"

namespace EARS
{
	namespace Framework
	{
		void CameraManager::EnableUpdate()
		{
			assert(m_DisableUpdate >= 0);

			--m_DisableUpdate;
		}

		void CameraManager::DisableUpdate()
		{
			++m_DisableUpdate;
		}

		bool CameraManager::GetActiveCamFov(float& OutFov, uint32_t InPlayerID) const
		{
			// TODO: Implement debug (such as directors camera and debug mode)
			const uint32_t ViewportID = ViewportIDFromPlayerID(InPlayerID);
			assert(ViewportID < 4 && "viewport < MAX_VIEWPORTS");

			// NB: This is what the game does, RwV3d does not exist in CameraBlender
			const CameraBlender& ViewportBlender = m_CameraBlenders[ViewportID];
			OutFov = ViewportBlender.GetActiveCamFov();

			return true;
		}

		bool CameraManager::GetActiveCamRot(RwV3d& OutRotation, uint32_t InPlayerID) const
		{	
			// TODO: Implement debug (such as directors camera and debug mode)
			const uint32_t ViewportID = ViewportIDFromPlayerID(InPlayerID);
			assert(ViewportID < 4 && "viewport < MAX_VIEWPORTS");

			// NB: This is what the game does, RwV3d does not exist in CameraBlender
			const CameraBlender& ViewportBlender = m_CameraBlenders[ViewportID];
			const float* Rotation = ViewportBlender.GetActiveCamRot();
			OutRotation = RwV3d(Rotation[0], Rotation[1], Rotation[2]);

			return true;
		}

		uint32_t CameraManager::ViewportIDFromPlayerID(uint32_t InPlayerID) const
		{
			assert(InPlayerID == 0 && "InPlayerID != PlayerManager::INVALID_ID");

			// Viewport Idx is stored in player array
			for (uint32_t Idx = 0; Idx < m_NumViewports; Idx++)
			{
				if (m_PlayerIDs[Idx] == InPlayerID)
				{
					return Idx;
				}
			}

			// by default the game uses 4
			return 4;
		}

		/* static */
		CameraManager* CameraManager::GetInstance()
		{
			return *(CameraManager**)0x122339C;
		}
	} // Framework
} // EARS
