#pragma once

// SDK.h
#include "ears_common/guid.h"
#include "ears_common/string.h"
#include "framework/core/base/base.h"

namespace EARS
{
	namespace Modules
	{
		class DemoPackage : public EARS::Framework::Base
		{
		public:

			/** resolve the name of this package */
			const String* GetName() const { return &m_Name; }

			const EARS::Common::guid128_t* GetInstanceID() const { return &m_InstanceID; }

		private:

			EARS::Common::guid128_t m_InstanceID;

			String m_Name;

			RWS::CEventId m_MsgStart;

			EARS::Common::guid128_t m_PlayerSpawnGuid;

			uint32_t m_MenuOrder = 0;
		};

	} // Modules
} // EARS