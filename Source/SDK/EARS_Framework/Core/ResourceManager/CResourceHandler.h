#pragma once

// SDK
#include "SDK/EARS_Common/Guid.h"

// C++
#include <stdint.h>

namespace RWS
{ 
	class CResourceHandler
	{
	public:

		class CResourceLoadInfo
		{
		public:

			uint8_t* BindResourceData();

		private:

			void* m_Resource = nullptr;
			EARS::Common::guid128_t* m_GUID = nullptr;
			uint32_t m_TypeID = 0;
			const char* m_TypeName = nullptr;
			void* m_Allocator = nullptr;
			void* m_RawData = nullptr;
			uint32_t m_RawDataSize = 0;
			void* m_BoundData = nullptr;
			uint32_t m_Flags = 0;
		};

		virtual ~CResourceHandler() { /* implemented by game code */ }

	private:

		void* m_HandledTypeList = nullptr; // RWS::CResourceTypeInfo
	};
}
