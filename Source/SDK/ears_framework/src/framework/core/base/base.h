#pragma once

// SDK
#include "ears_common/safeptr.h"
#include "framework/core/attributehandler/cattributehandler.h"
#include "framework/core/eventhandler/ceventhandler.h"

namespace EARS
{
	namespace Framework
	{
		struct BaseAllocationPolicy {};

		class Base : public RWS::CAttributeHandler, public RWS::CEventHandler, public SafeObj, public BaseAllocationPolicy
		{
		public:

			Base() = delete;
			Base(const RWS::CAttributePacket& InAttr);
			Base(const EARS::Common::guid128_t* InGuid, const uint32_t InStreamHandle);
			virtual ~Base();

			//~ Begin RWS::CEventHandler Interface
			virtual void HandleEvents(const RWS::CMsg& MsgEvent) override;
			virtual void DisableMessages() override;
			virtual void EnableMessages() override;
			//~ End RWS::CEventHandler Interface

			// Convert this object to a given class
			virtual bool QueryInterface(const uint32_t ClassID, void** OutObjectPtr) const;

			// assuming this registers a tick event to this object
			virtual void LinkTick() { /* nothing by default */ }

			// assuming this unregisters a tick event to this object
			virtual void UnLinkTick() { /* nothing by default */ }

			static bool IsEventHandlerBase(const RWS::CEventHandler& InHandler);

		private:

			// We add flag to EventHandler to determine type
			// This is a continuation of CEventHandlerFlags
			enum
			{
				CEVENTHANDLER_FLAG_BASE = 0x10
			};

			// I'm assuming this is 0x50 in size
		};

		// utility functions for EARS::Framework::Base
		// NB: Ensure type safety because this does not!
		// This exists in engine code too (excluding the assert)
		template<typename T>
		T* _QueryInterface(const EARS::Framework::Base* InBase, const uint32_t InClassID)
		{
			void* ObjectPtr;
			if (InBase->QueryInterface(InClassID, &ObjectPtr))
			{
				return reinterpret_cast<T*>(ObjectPtr);
			}

			return nullptr;
		}

		// utility functions for EARS::Framework::Base
		// NB: Ensure type safety because this does not!
		// This exists in engine code too (excluding the assert)
		template<typename T>
		T* _GetInterface(const EARS::Framework::Base* InBase, const uint32_t InClassID)
		{
			void* ObjectPtr;
			if (InBase->QueryInterface(InClassID, &ObjectPtr))
			{
				return reinterpret_cast<T*>(ObjectPtr);
			}

			return nullptr;
		}

		static_assert(sizeof(Base) == 0x50, "EARS::Framework::Base must equal 0x50");
	} // Framework
} // EARS
