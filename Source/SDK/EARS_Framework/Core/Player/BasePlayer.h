#pragma once

namespace EARS
{
	namespace Framework
	{
		/**
		 * The type to inherit if you'd like to register as a Player,
		 * using EARS::Framework::PlayerManager.
		 */
		class BasePlayer
		{
		private:

			virtual ~BasePlayer() = 0;
		};
	} // Framework
} // EARS
