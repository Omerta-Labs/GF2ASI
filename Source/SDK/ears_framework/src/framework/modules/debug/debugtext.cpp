#include "debugtext.h"

// Addons
#include "Platform/Diagnostics.h"
#include "Platform/MemUtils.h"

// SDK
#include "framework/mainloop/logic.h"

// C++
#include <string>

namespace EARS::Framework::Modules
{
	DebugText::DebugText(const RWS::CAttributePacket& InAttr)
		: Base(InAttr)
		, m_DebugString(nullptr)
		, m_DisplayTime(4.0f)
		, m_TimeRemaining(m_DisplayTime)
		, m_Options(SCREEN)
	{
	}

	DebugText::~DebugText()
	{
		if (m_DebugString)
		{
			delete[] m_DebugString;
			m_DebugString = nullptr;
		}

		// TODO: Unlink messages
	}

	void DebugText::HandleAttributes(const RWS::CAttributePacket& InPacket)
	{
		Base::HandleAttributes(InPacket);

		RWS::CAttributeCommandIterator CommandIt = RWS::CAttributeCommandIterator(InPacket, 0x862623C0);
		while (!CommandIt.IsFinished())
		{
			switch (CommandIt->GetCommandId())
			{
				case LoadParameterTypes::CMD_SetTargetName:
				{
					const char* Message = CommandIt->GetAs_char_ptr();
					ReplaceLinkedMsg(m_TargetName, Message, "BasePtrMsgData*");
					break;
				}
				case LoadParameterTypes::CMD_SetText:
				{
					// we need to own it so copy
					const char* Text = CommandIt->GetAs_char_ptr();

					// The game code only applies changes if we have text
					if (Text)
					{
						const uint32_t Length = strlen(Text);

						// Make sure to delete existing
						if (m_DebugString)
						{
							delete[] m_DebugString;
							m_DebugString = nullptr;
						}

						// allocate and copy
						m_DebugString = new char[Length];
						std::strncpy(m_DebugString, Text, Length);
					}

					break;
				}
				case LoadParameterTypes::CMD_SetDisplayTime:
				{
					m_DisplayTime = CommandIt->GetAs_float();
					break;
				}
				case LoadParameterTypes::CMD_SetOptions:
				{
					m_Options = CommandIt->GetAs_uint32();

					// TEMP
					m_Options |= MESSAGE_LOG;
					break;
				}
				default:
				{
					break;
				}
			}
		}
	}

	void DebugText::HandleEvents(const RWS::CMsg& MsgEvent)
	{
		const float SimFrameTimeInSec = hook::Type<float>(0x12067E8);

		// MAYBE_BUG: Original game code does not trigger base class
		if (MsgEvent.IsEvent(m_TargetName))
		{
			if ((m_Options & SCREEN) != 0)
			{
				LinkMsgOnce(RWS::iMsgRunningTick);
				LinkMsgOnce(RWS::iMsgPausedTick);

				m_TimeRemaining = m_DisplayTime;
			}

			if (m_DebugString && ((m_Options & MESSAGE_LOG) != 0))
			{
				EARS::Diag::Printf(m_DebugString);
			}
		}
		else if (MsgEvent.IsEvent(RWS::iMsgRunningTick))
		{
			if (m_DebugString)
			{
				// TODO: String 
				//EARS::Diag::Printf(m_DebugString);
			}

			m_TimeRemaining -= SimFrameTimeInSec;

			if (m_TimeRemaining <= 0.0f)
			{
				UnlinkMsg(&RWS::iMsgRunningTick);
				UnlinkMsg(&RWS::iMsgPausedTick);
			}
		}
		else if (MsgEvent.IsEvent(RWS::iMsgPausedTick))
		{
			if (m_DebugString)
			{
				// TODO: Printf
				//EARS::Diag::Printf(m_DebugString);
			}

			m_TimeRemaining -= SimFrameTimeInSec;

			if (m_TimeRemaining < 0.0f)
			{
				UnlinkMsg(&RWS::iMsgRunningTick);
				UnlinkMsg(&RWS::iMsgPausedTick);
			}
		}
	}
} //~ EARS::Framework::Modules
