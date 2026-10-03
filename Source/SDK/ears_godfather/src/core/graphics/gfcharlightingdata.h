#pragma once

// SDK Framework
#include "framework/core/eventhandler/ceventhandler.h"

// C++
#include <stdint.h>

namespace EARS
{
	namespace Godfather
	{
		/**
		 * Attribute handler for the character lighting block.
		 *
		 * This is not an object - the engine has no instance of it anywhere. It exists only
		 * as a static packet handler that writes straight into the GlobalCharLightingData
		 * singleton and then broadcasts iMsgGFCharLightingDataUpdated so anything caching
		 * the values knows to refresh.
		 *
		 * The handler ignores any op code above 1.
		 */
		class GFCharLightingData
		{
		public:

			/** Attribute class the packet parser matches on */
			static constexpr uint32_t CLASS_ID = 0x25232BEB;

			/**
			 * Command ids inside the packet. Note that a direction is authored as three
			 * separate float commands rather than one vector command.
			 */
			enum class Command : uint32_t
			{
				Command_REF = 0xFFFFFFFF,

				RIM_LIGHT_DIRECTION_X = 0x0,
				RIM_LIGHT_DIRECTION_Y = 0x1,
				RIM_LIGHT_DIRECTION_Z = 0x2,
				RIM_LIGHT_DIFFUSE_SPEC_PARAM = 0x3,

				MOVIE_LIGHT_DIRECTION_X = 0x4,
				MOVIE_LIGHT_DIRECTION_Y = 0x5,
				MOVIE_LIGHT_DIRECTION_Z = 0x6,

				SPEC_LIGHT_0_DIRECTION_X = 0x7,
				SPEC_LIGHT_0_DIRECTION_Y = 0x8,
				SPEC_LIGHT_0_DIRECTION_Z = 0x9,
				SPEC_LIGHT_0_INTENSITY = 0xA,
				SPEC_LIGHT_0_EXPONENT = 0xB,

				SPEC_LIGHT_1_DIRECTION_X = 0xC,
				SPEC_LIGHT_1_DIRECTION_Y = 0xD,
				SPEC_LIGHT_1_DIRECTION_Z = 0xE,
				SPEC_LIGHT_1_INTENSITY = 0xF,
				SPEC_LIGHT_1_EXPONENT = 0x10,

				Command_MAX_VALUE = 0x11,
			};

			/**
			 * The event broadcast after a packet has been applied.
			 * Send this yourself after writing GlobalCharLightingData by hand so listeners
			 * pick the change up the same way they would an authored one.
			 */
			static const RWS::CEventId& GetDataUpdatedEvent();
		};
	} // Godfather
} // EARS
