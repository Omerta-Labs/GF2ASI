#pragma once

// CPP
#include <stdint.h>

namespace EARS
{
	namespace Modules
	{
		struct ScoreEventOperation
		{
		public:

			// All score opcodes in GF2.
			// If this code is moving between games, ensure this is updated to match
			enum class OpCode : int8_t
			{
				OPCODE_NOP = 0x0,
				OPCODE_SEND_MESSAGE = 0x1,
				OPCODE_SEND_EVENT = 0x2,
				OPCODE_SEND_TELEMETRY = 0x3,
				OPCODE_DISPLAY = 0x4,
				OPCODE_SIMPLE_SET_CONSTANT = 0x5,
				OPCODE_SIMPLE_ADD_CONSTANT = 0x6,
				OPCODE_SIMPLE_MUL_CONSTANT = 0x7,
				OPCODE_SIMPLE_MIN_CONSTANT = 0x8,
				OPCODE_SIMPLE_MAX_CONSTANT = 0x9,
				OPCODE_SIMPLE_SET = 0xA,
				OPCODE_SIMPLE_ADD = 0xB,
				OPCODE_SIMPLE_MUL = 0xC,
				OPCODE_SIMPLE_MIN = 0xD,
				OPCODE_SIMPLE_MAX = 0xE,
				OPCODE_SIMPLE_SET_TABLE_LOOKUP = 0xF,
				OPCODE_SIMPLE_ADD_TABLE_LOOKUP = 0x10,
				OPCODE_SIMPLE_MUL_TABLE_LOOKUP = 0x11,
				OPCODE_SIMPLE_SET_CONSTANT_CONTEXT = 0x12,
				OPCODE_SIMPLE_ADD_CONSTANT_CONTEXT = 0x13,
				OPCODE_SIMPLE_MUL_CONSTANT_CONTEXT = 0x14,
				OPCODE_SIMPLE_MIN_CONSTANT_CONTEXT = 0x15,
				OPCODE_SIMPLE_MAX_CONSTANT_CONTEXT = 0x16,
				OPCODE_SIMPLE_SET_CONTEXT = 0x17,
				OPCODE_SIMPLE_ADD_CONTEXT = 0x18,
				OPCODE_SIMPLE_MUL_CONTEXT = 0x19,
				OPCODE_SIMPLE_MIN_CONTEXT = 0x1A,
				OPCODE_SIMPLE_MAX_CONTEXT = 0x1B,
				OPCODE_SIMPLE_SET_ITEM_COUNT = 0x1C,
				OPCODE_SIMPLE_UNLOCK_SAFEHOUSE = 0x1D,
				OPCODE_SIMPLE_FOR_SALE_SAFEHOUSE = 0x1E,
				OPCODE_SIMPLE_UNLOCK_ACHIEVEMENT = 0x1F,
				OPCODE_SIMPLE_UNLOCK_TROPHY = 0x20,
				OPCODE_SIMPLE_ROUND_CONSTANT = 0x21,
			};

			// size of the score event
			uint16_t m_Size = 0;

			// The opcode of the operation
			ScoreEventOperation::OpCode m_Opcode = OpCode::OPCODE_NOP;

			// The flags for this specific operation
			// TODO: What flags do we actually know?
			uint8_t m_Flags = 0;

			// NB: This is only to be used as an offset to the data.
			// In GF2 the data is immediately after the meta data.
			// As of writing this, the only known use is the Achievement/Unlockables
			// Which stores them as uint32_t
			void* m_Data = nullptr;
		};
	}
}
