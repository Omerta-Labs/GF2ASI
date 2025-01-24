#pragma once

// Framework
#include <SDK/EARS_Framework/Core/EventHandler/CEventHandler.h>

// forward declares
namespace RWS { class CAttributeHandler; }

namespace EARS
{
	namespace Framework
	{
		class ComponentListRecord
		{
		public:

			uint16_t GetIndex(const uint32_t RegisteredIndex) const { return m_Map[RegisteredIndex]; }

		private:

			char m_Padding_0[0xE];
			uint16_t m_Map[255];
		};

		/**
		 * Contains shareable functionality across entity types
		 * Stored within an Attribute Handler.
		 */
		class Component  : public RWS::CEventHandler
		{
		public:

		private:

			RWS::CAttributeHandler* m_GameObject = nullptr;
		};
	} // Framework
} // EARs
