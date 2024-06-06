#pragma once

// SDK
#include "SDK/EARS_Common/Array.h"

// CPP
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
