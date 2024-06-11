#pragma once

namespace EARS
{
	namespace Apt
	{
		/**
		 * An Object to maintain the HUD elements ingame, using APT.
		 */
		class UIHUD
		{
		public:

			// Increment the 'suppress' counter.
			// If Suppress > 0, hide hud
			void Suppress();

			// Decrement the 'suppress' counter
			// If Suppress == 0, show hud
			void Unsuppress();

			// Get the instance of this object
			static EARS::Apt::UIHUD* GetInstance();
		};
	} // Apt
} // EARS
