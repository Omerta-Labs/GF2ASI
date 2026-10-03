#pragma once

// SDK
#include "SDK/ears_common/include/ears_common/array.h"
#include "SDK/ears_common/include/ears_common/guid.h"
#include "SDK/ears_common/include/ears_common/singleton.h"
#include "SDK/ears_framework/src/game_framework/framework/core/eventhandler/ceventhandler.h"

namespace EARS
{
	namespace Modules
	{
		// forward declares
		class DemoPackage;

		class DemoPackageManager : public RWS::CEventHandler, public Singleton<DemoPackageManager>
		{
		public:

			void SetPendingPackageByIndex(const uint32_t InIndex);

			void ClearPendingPackage();

			/** resolve a package using a given index */
			const DemoPackage* GetPackage(const uint32_t Index) const { return m_Packages[Index]; }

			/** get the number of packages currently registered */
			uint32_t GetNumPackages() const { return m_Packages.Size(); }

			static DemoPackageManager* GetInstance();

		private:

			void SetPendingPackage(const DemoPackage* InNewPackage);

			EARS::Common::guid128_t m_PendingPackageGuid;
			EARS::Common::guid128_t m_ActivePackageGuid;
			Array<EARS::Modules::DemoPackage*> m_Packages;
		};

	} // Modules
} // EARS
