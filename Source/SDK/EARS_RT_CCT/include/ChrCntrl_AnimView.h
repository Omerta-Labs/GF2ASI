#pragma once

// CCT
#include "ChrCntrl_AnimState.h"

// forward declares
class ChrCntl_ChrInfo_s;

namespace EA 
{
	namespace CCT
	{
		class AnimView : public EA::CCT::Internal::AnimState
		{
		public:

			virtual ~AnimView() { /* filled in by game code */ }

			/**
			 * Determine whether the Anim State is running a specific animation
			 * @param AnimID The Animation we want to check is playing
			 * @bool Whether or not the Animation is playing.
			 */
			bool IsPlayingAnim(const uint32_t AnimID) const { return m_RootStateID == AnimID; }

			// getters
			const ChrCntl_ChrInfo_s* GetCharacter() const { return m_Character; }

		private:

			ChrCntl_ChrInfo_s* m_Character = nullptr;
			uint16_t m_ModelIndex = 0;
			uint16_t m_SkelIndex = 0;
			float m_CurTime = 0.0f;
			EA::CCT::Internal::AnimState* m_ActiveState = nullptr;
			EA::CCT::Transform m_GameTransform;
			void* m_BoneMods = nullptr;
			void* m_BonePostMods = nullptr;

			// 0x70 - character
			// 0x74 - model index
			// 0x76 - skel index
			// 0x78 - cur time
			// 0x7C - active state
			// 0x80 - game movement
			// 0x100 - parent object
			// 0xA0 - bone mods
			// 0xA4 - bone post mods
			// 0xAC - num max bone mods
			// 0xAE - enable bone mods
			// 0x140 - GMCacheEntry[8] (float 0x0, frmae num 0x4)


		};
	} // CCT
} // EARS
