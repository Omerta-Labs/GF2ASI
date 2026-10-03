#pragma once

// C++
#include <stdint.h>

namespace EARS
{
	namespace Modules
	{
		class TimeOfDayManager
		{
		public:

			// declare internal types earlier
			struct GameTime
			{
				int32_t m_Year = 0;
				int32_t m_Day = 0;
				int32_t m_Hour = 0;
				int32_t m_Minute = 0;
				int32_t m_Second = 0;
			};

			/**
			 * Fetch the base GameTime
			 * Does not include 'apparent' values or overrides
			 */
			GameTime GetGameTime() const;

			/**
			 * Set the base GameTime
			 * Does not include 'apparent' values or overrides
			 */
			void SetGameTime(const GameTime& InGameTime);

			/**
			 * Fetch the Singleton instance of the TimeOfDay manager
			 */
			static TimeOfDayManager* GetInstance();

		private:

			char m_Padding_0[0x5C];
			GameTime m_BaseGameTime;
			float m_TimeScale = 0.0f;
			float m_CachedTimeScale = 0.0f;
			float m_GameClockMultiplier = 0.0f;
			uint32_t m_ElapsedGameTicks = 0;
			float m_ElapsedGameTicksCarriedOver = 0;
			uint32_t m_Flags = 0;
			bool m_bApparentOverride = false;
			bool m_bIncApparentGameTime = false;
			GameTime m_ApparentGameTime;
			GameTime m_ApparentGameTimeInc;
		};
	} // Modules
} // EARS
