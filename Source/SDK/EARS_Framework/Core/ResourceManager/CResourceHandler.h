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

		/**
		 * Partial.
		 *
		 * The X360 debug build lays this out as
		 *   { const char* m_pName; uint32_t m_typeID; const char* m_pTypeName; void* m_pData; guid128_t* m_pGUID }
		 * but the PC build reads the resource pointer at +0x08 rather than +0x0C, so PC is
		 * short one of the leading fields. Only m_pData is confirmed here (via
		 * EARS::Modules::ControllerDataManager::UnloadResource); the surrounding fields and
		 * the overall size are left opaque deliberately. It is only ever passed by
		 * reference, so the unverified tail size does not matter.
		 */
		class CResourceUnloadInfo
		{
		public:

			void* GetResourceData() const { return m_pData; }

		private:

			char m_Padding_UnloadInfo[0x8];		// 0x00
			void* m_pData = nullptr;			// 0x08
			char m_Padding_UnloadInfo1[0x8];	// 0x0C
		};

		virtual ~CResourceHandler() { /* implemented by game code */ }

	private:

		void* m_HandledTypeList = nullptr; // RWS::CResourceTypeInfo
	};
}
