#pragma once

// C++
#include <stdint.h>

namespace EARS
{
	namespace Modules
	{
		/**
		 * Rank for Sentient types
		 */
		enum class SentientRank : uint32_t
		{
			SentientRank_REF = 0xFFFFFFFF,
			RANK_CIVILIAN = 0x0,
			RANK_ASSOCIATE = 0x1,
			RANK_ASSOCIATE_STRONG = 0x2,
			RANK_SOLDIER = 0x3,
			RANK_CAPO = 0x4,
			RANK_UNDERBOSS = 0x5,
			RANK_CONSIGLIERE = 0x6,
			RANK_DON = 0x7,
			SentientRank_MAX_VALUE = 0x8,
		};
	}
}