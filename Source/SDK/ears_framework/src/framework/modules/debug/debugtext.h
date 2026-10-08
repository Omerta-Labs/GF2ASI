#pragma once

// EARS_Framework
#include "ears_common/string.h"
#include "framework/core/attributehandler/cclassfactory.h"
#include "framework/core/base/base.h"

namespace EARS::Framework::Modules
{
	// Reimplements the DebugText object found in debug versions
	// It routes text through logging systems
	class DebugText : public EARS::Framework::Base
	{
	public:

		DebugText() = delete;
		DebugText(const RWS::CAttributePacket& InAttr);
		virtual ~DebugText();

		//~ Begin EARS::Framework::Base Interface
		virtual void HandleAttributes(const RWS::CAttributePacket& InPacket) override;
		virtual void HandleEvents(const RWS::CMsg& MsgEvent) override;
		//~ End EARS::Framework::Base Interface

		RWS_MAKENEWCLASS(DebugText);

		// TODO: Convert this to original macro format
		enum LoadParameterTypes
		{
			CMD_SetTargetName,
			CMD_SetText,
			CMD_SetDisplayTime,
			CMD_SetOptions,
			SIZE_ATTRIBUTE,
			LAST_ATTRIBUTE = 2147483647						// RWFORCEENUMSIZEINT
		};

	private:

		enum
		{
			SCREEN = 0x1,
			MESSAGE_LOG = 0x2,
		};

		// event to trigger the debug text
		// (it will either fire immediately or start the timer)
		RWS::CEventId m_TargetName;

		// DEVIATION: This was original const char, but it caused problems
		// when allocating. To my knowledge, storing as String has no other side effects.
		String m_DebugString;

		// How long it shows on screen for
		float m_DisplayTime = 0.0f;

		// How long it will remain on screen for
		float m_TimeRemaining = 0.0f;

		// Flags for operation; see SCREEN or MESSAGE_LOG
		uint32_t m_Options = 0;
	};
} //~ EARS::Framework::Modules
