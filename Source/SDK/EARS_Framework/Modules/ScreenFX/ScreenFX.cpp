#include "ScreenFX.h"

#include "addons/hook.h"

namespace PrivateEvents
{
	static hook::Type<RWS::CEventId> RunningTickEvent = hook::Type<RWS::CEventId>(0x012069C4);
	static hook::Type<RWS::CEventId> PausedTickEvent = hook::Type<RWS::CEventId>(0x012069B4);
	static hook::Type<RWS::CEventId> DoRenderEvent = hook::Type<RWS::CEventId>(0x01206970);
}

namespace EARS::Modules
{
	void ScreenFX::Effect::Start(float InDuration, DIRECTION InDirection, bool bInHold)
	{
		m_Time = 0.0f;
		m_Duration = InDuration;
		m_Direction = InDirection;
		m_bHold = bInHold;
		m_bActive = true;
		m_bComplete = false;

		ScreenFX::GetInstance()->Kick();
	}

	void ScreenFX::Effect::Stop(float InDuration)
	{
		if (m_bActive)
		{
			m_Time = 0.0f;
			m_Duration = InDuration;
			m_Direction = DIRECTION::DIR_OUT;
			m_bHold = false;
		}
	}

	void ScreenFX::LetterBoxBegin(float InDuration)
	{
		m_Letterbox.Start(InDuration, DIRECTION::DIR_IN, true);
	}

	void ScreenFX::LetterBoxEnd(float InDuration)
	{
		m_Letterbox.Stop(InDuration);
	}

	void ScreenFX::Blur_Start(float InDuration)
	{
		m_Blur.Start(InDuration, DIRECTION::DIR_IN, true);
	}

	void ScreenFX::Blur_Stop(float InDuration)
	{
		m_Blur.Stop(InDuration);
	}

	void ScreenFX::Bloom_Start(float InDuration)
	{
		m_Bloom.Start(InDuration, DIRECTION::DIR_IN, true);
	}

	void ScreenFX::Bloom_Stop(float InDuration)
	{
		m_Bloom.Stop(InDuration);
	}

	void ScreenFX::Depth_Start(float InDuration)
	{
		m_DepthOfField.Start(InDuration, DIRECTION::DIR_IN, true);
	}

	void ScreenFX::Depth_Stop(float InDuration)
	{
		m_DepthOfField.Stop(InDuration);
	}

	void ScreenFX::Depth_SetDirection(DIRECTION InDirection)
	{
		m_DepthOfField.SetDirection(InDirection);
	}

	void ScreenFX::Depth_SetFocus(float InFocus)
	{
		m_DepthOfField.SetFocus(InFocus);
	}

	void ScreenFX::Depth_SetHold(bool bInHold)
	{
		m_DepthOfField.SetHold(bInHold);
	}

	void ScreenFX::Depth_SetIntensity(float InIntensity)
	{
		m_DepthOfField.SetIntensity(InIntensity);
	}

	void ScreenFX::Kick()
	{
		if (!m_bActive)
		{
			m_bActive = true;

			if (PrivateEvents::RunningTickEvent.get().GetMsgId() != 0)
			{
				LinkMsg(&PrivateEvents::RunningTickEvent, 0x8000);
			}

			if (PrivateEvents::PausedTickEvent.get().GetMsgId() != 0)
			{
				LinkMsg(&PrivateEvents::PausedTickEvent, 0x8000);
			}

			if (PrivateEvents::DoRenderEvent.get().GetMsgId() != 0)
			{
				LinkMsg(&PrivateEvents::DoRenderEvent, 0x8000);
			}
		}
	}

	void ScreenFX::Stop()
	{
		if (m_bActive)
		{
			m_bActive = false;

			UnlinkMsg(&PrivateEvents::RunningTickEvent);
			UnlinkMsg(&PrivateEvents::PausedTickEvent);
			UnlinkMsg(&PrivateEvents::DoRenderEvent);
		}
	}

	ScreenFX* ScreenFX::GetInstance()
	{
		return *(ScreenFX**)0x1223394;
	}
}
