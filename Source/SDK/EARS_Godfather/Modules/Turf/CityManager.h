#pragma once

// SDK
#include "SDK/EARS_Common/Array.h"

// CPP
#include <functional>
#include <stdint.h>

template <typename T>
class Singleton
{
public:

	virtual ~Singleton() { }

private:


};

namespace EARS
{
	namespace Modules
	{
		class City;

		/**
		 * A Manager consisting of all the Cities the Player can visit
		 * during gameplay.
		 */
		class CityManager : public Singleton<CityManager>
		{
		public:

			/**
			 * Utility function to iterate through all loaded Cities
			 * All const, we do not expect to modify any of the assemblies during iteration.
			 * All headers pass-by-ref, so none of them should be nullptr.
			 * Function does not exist in GF2 exe, merely utility header for us to use.
			 */
			typedef std::function<void(const City&)> TVisitCityFunctor;
			void ForEachCity(const TVisitCityFunctor& InFunction) const;

			// Get the CityManager instance
			static CityManager* GetInstance();

		private:

			void* VTABLE2 = nullptr;
			uint32_t m_uCurrCityID = 0;
			uint32_t m_uPrevCityID = 0;
			Array<EARS::Modules::City*> m_Cities;
		};
	} // Modules
} // EARS
