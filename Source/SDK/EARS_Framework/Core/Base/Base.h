#pragma once

// SDK
#include "SDK/EARS_Common/SafePtr.h"
#include "SDK/EARS_Framework/Core/AttributeHandler/CAttributeHandler.h"
#include "SDK/EARS_Framework/Core/EventHandler/CEventHandler.h"

namespace EARS
{
	namespace Framework
	{
		struct BaseAllocationPolicy {};

		class Base : public RWS::CAttributeHandler, public RWS::CEventHandler, public SafeObj, public BaseAllocationPolicy
		{
		public:

			virtual ~Base() = 0;

			virtual void QueryInterface() = 0;
			virtual void LinkTick() = 0;
			virtual void UnLinkTick() = 0;

		private:
			// I'm assuming this is 0x50 in size
		};
	} // Framework
} // EARS
