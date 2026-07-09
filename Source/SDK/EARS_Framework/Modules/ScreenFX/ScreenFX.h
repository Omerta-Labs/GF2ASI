#pragma once

#include "SDK/EARS_Common/Singleton.h"
#include "SDK/EARS_Framework/Core/EventHandler/CEventHandler.h"

namespace EARS::Modules
{
	class ScreenFX : RWS::CEventHandler, public Singleton<ScreenFX>
	{
	public:
		
		enum class DIRECTION
		{
			DIR_IN = 0x0,
			DIR_OUT = 0x1,
		};

		enum class SCREENFX_CONTEXT
		{
			CONTEXT_DEFAULT = 0x1,
			CONTEXT_1 = 0x2,
			CONTEXT_HUD_MAJOR_ACC = 0x4,
			CONTEXT_FRONT_END_LEVEL = 0x8,
			CONTEXT_IN_GAME_VIDEO = 0x10,
			CONTEXT_MOBFACE = 0x20,
		};

		struct Effect
		{
		public:

			virtual ~Effect() = 0;

			void Start(float InDuration, DIRECTION InDirection, bool bInHold);
			void Stop(float InDuration);

			void SetDirection(DIRECTION InDirection) { m_Direction = InDirection; }
			void SetHold(bool bInHold) { m_bHold = bInHold; }
			void SetIntensity(float InIntensity) { m_Intensity = InIntensity; }

		private:

			bool m_bBlackScreen = false;
			float m_BlackScreenElapsedTime = 0.0f;
			unsigned int m_Context = 0;
			bool m_bActive = false;
			bool m_bHold = false;
			EARS::Modules::ScreenFX::DIRECTION m_Direction;
			float m_Duration = 0.0f;
			float m_Intensity = 0.0f;
			uint32_t m_color;				// RwRGBATag
			bool m_bComplete = false;
			uint32_t m_SuppressedLoadingScreenContexts = 0;
			float m_Time = 0.0f;
		};

		struct LetterBox : Effect 
		{
		public:
		};

		struct ScreenFade : Effect
		{
		public:

		private:

			bool m_bIsRenderAfterPostEffects = false;
		};

		struct ScreenBlur : Effect {};

		struct ScreenBloom : Effect {};

		struct ScreenDepthOfField : Effect
		{
		public:

			void SetFocus(float InFocus) { m_Focus = InFocus; }

		private:

			float m_Focus = 0.0f;
		};

		void LetterBoxBegin(float InDuration);
		void LetterBoxEnd(float InDuration);

		void Blur_Start(float InDuration);
		void Blur_Stop(float InDuration);

		void Bloom_Start(float InDuration);
		void Bloom_Stop(float InDuration);

		void Depth_Start(float InDuration);
		void Depth_Stop(float InDuration);
		void Depth_SetDirection(DIRECTION InDirection);
		void Depth_SetFocus(float InFocus);
		void Depth_SetHold(bool bInHold);
		void Depth_SetIntensity(float InIntensity);

		// Link this handler to the running/paused tick and render events so the
		// effects begin updating. Effects tick on both running AND paused ticks,
		// so they keep animating while gameplay is paused.
		void Kick();

		// Counterpart to Kick - unlink from all events and stop updating.
		// The game also does this automatically once every effect has completed.
		void Stop();

		// ASI helpers, not part of the original game API. The game only exposes
		// intensity setters for depth of field, but the other effects support it too.
		Effect& GetBlurEffect() { return m_Blur; }
		Effect& GetBloomEffect() { return m_Bloom; }

		static ScreenFX* GetInstance();

	private:

		bool m_bActive = false;
		EARS::Modules::ScreenFX::LetterBox m_Letterbox;
		EARS::Modules::ScreenFX::ScreenFade m_Fade;
		EARS::Modules::ScreenFX::ScreenBlur m_Blur;
		EARS::Modules::ScreenFX::ScreenBloom m_Bloom;
		EARS::Modules::ScreenFX::ScreenDepthOfField m_DepthOfField;

		// 0xFF000000
	};

	static_assert(sizeof(ScreenFX) == 268);
}
