#pragma once

// Framework
#include <SDK/EARS_Framework/Core/EventHandler/CEventHandler.h>

// forward declares
namespace RWS { class CAttributeHandler; }

namespace EARS
{
	namespace Framework
	{
		class Component  : public RWS::CEventHandler
		{
		public:

		private:

			RWS::CAttributeHandler* m_GameObject = nullptr;
		};
	} // Framework
} // EARs
