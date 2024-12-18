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

			virtual bool QueryInterface(const uint32_t ClassID, void** OutObjectPtr) = 0;
			virtual void LinkTick() = 0;
			virtual void UnLinkTick() = 0;

		private:
			// I'm assuming this is 0x50 in size
		};

		// utility functions for EARS::Framework::Base
		// NB: Ensure type safety because this does not!
		// This exists in engine code too (excluding the assert)
		template<typename T>
		T* _QueryInterface(EARS::Framework::Base* InBase, const uint32_t InClassID)
		{
			void* ObjectPtr;
			if (InBase->QueryInterface(InClassID, &ObjectPtr))
			{
				return reinterpret_cast<T*>(ObjectPtr);
			}

			return nullptr;
		}
	} // Framework
} // EARS
