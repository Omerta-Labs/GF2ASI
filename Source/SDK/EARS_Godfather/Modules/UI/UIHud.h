#pragma once

// CPP
#include <stdint.h>

namespace EARS
{
	namespace Modules
	{
		class NPC;
	}

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

			// Add a crew member to the UIHUD element
			// Handy for updating their element if the crew member has been modified
			void AddCrew(EARS::Modules::NPC* CrewMember) const;

			// Remove a crew member to the UIHUD element
			// Handy for updating their element if the crew member has been modified
			void RemoveCrew(EARS::Modules::NPC* CrewMember) const;

			/**
			 * Update a Crew Members specialties on the HUD.
			 * (Part of Engine code)
			 */
			void AddCrewSpecialty(EARS::Modules::NPC* CrewNPC, uint32_t NewSpecialty);

			// Get the instance of this object
			static EARS::Apt::UIHUD* GetInstance();
		};
	} // Apt
} // EARS
