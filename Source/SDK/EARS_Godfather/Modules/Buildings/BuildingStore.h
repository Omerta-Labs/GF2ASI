#pragma once

#include "SDK/EARS_Framework/Core/EventHandler/CEventHandler.h"

namespace EARS
{
	namespace Modules
	{
		struct ISimNPCListener
		{

		};

		class BuildingStore : public ISimNPCListener, public RWS::CEventHandler
		{
		public:

			void ChangeOwnership(uint32_t NewFamilyID, const bool bShowFanfare, const void* pHeader, const bool bExtortion); /* pHeader -> EARS::Modules::BuildingPersistHeader */

		private:
		};
	}
}