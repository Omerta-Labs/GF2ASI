#pragma once

// SDK (Common)
#include "SDK/EARS_Common/Array.h"
#include "SDK/EARS_Common/Singleton.h"

// SDK (Framework)
#include "SDK/EARS_Framework/Core/Persistence/PersistenceRegistry.h"

// CPP
#include <functional>
#include <stdint.h>

struct String;

namespace EARS
{
	namespace Modules
	{
		class City;

		/**
		 * A Manager consisting of all the Cities the Player can visit
		 * during gameplay.
		 */
		class CityManager : public Singleton<CityManager>, public EARS::Framework::IPersistable
		{
		public:

			/**
			 * Fetch the ID of the Current City.
			 * Current City is the Active City.
			 */
			uint32_t GetCurrentCity() const { return m_CurrCityID; }

			/**
			 * Fetch the Display Name of the given City ID.
			 */
			String* GetDisplayName(const uint32_t CityID) const;

			/**
			 * Utility function to iterate through all loaded Cities
			 * All const, we do not expect to modify any of the assemblies during iteration.
			 * All headers pass-by-ref, so none of them should be nullptr.
			 * Function does not exist in GF2 exe, merely utility header for us to use.
			 */
			typedef std::function<void(City&)> TVisitCityFunctor;
			void ForEachCity(const TVisitCityFunctor& InFunction);

			// Get the CityManager instance
			static CityManager* GetInstance();

		private:

			/**
			 * Find the Index of the City ID within the Cities array.
			 * Returns a valid index to the City in the array.
			 * If -1 is returned, then no City was found.
			 */
			int32_t FindCityIndex(const uint32_t InCityID) const;

			uint32_t m_CurrCityID = 0;
			uint32_t m_PrevCityID = 0;
			Array<EARS::Modules::City*> m_Cities;
		};
	} // Modules
} // EARS
