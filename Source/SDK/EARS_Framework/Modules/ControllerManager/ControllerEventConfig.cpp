#include "ControllerEventConfig.h"

// SDK
#include "SDK/EARS_Common/CommonTypes.h"

// C++
#include <string.h>

namespace EARS
{
	namespace Modules
	{
		namespace
		{
			void CopyBounded(char* pDest, uint32_t DestSize, const char* pSource)
			{
				memset(pDest, 0, DestSize);

				if (pSource == nullptr)
				{
					return;
				}

				uint32_t Index = 0;
				while (Index < DestSize - 1 && pSource[Index] != '\0')
				{
					pDest[Index] = pSource[Index];
					++Index;
				}
			}
		}

		void ControllerEventConfig::SetNameAndHash(const char* InName)
		{
			CopyBounded(m_ConfigName, cMaxConfigNameLength, InName);
			m_ConfigNameHash = EARS::Common::HashString_SDBM(m_ConfigName);
		}

		void ControllerEventConfig::SetConfigResourceLocation(const char* InLocation)
		{
			CopyBounded(m_ConfigFileLocation, cMaxConfigLocationLength, InLocation);
		}

		void ControllerEventConfig::InitAsResourceImage()
		{
			// Mirrors what ControllerDataManager::LoadResource does to a freshly bound
			// blob: the descriptor array is the tail of this same allocation, so it always
			// starts one header past the base. m_EventConfigSize is left alone - it is
			// authored data, and the loader reads it back before doing this.
			m_EventConfigVersion = cCurrentParserVersion;
			m_ReferenceCount = 0;
			m_pEventDesc = reinterpret_cast<CtrlEventDesc*>(reinterpret_cast<uint8_t*>(this) + sizeof(ControllerEventConfig));
		}
	} // Modules
} // EARS
