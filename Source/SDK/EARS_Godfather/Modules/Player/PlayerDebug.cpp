#include "PlayerDebug.h"

namespace PrivateDebug
{
	static EARS::Modules::PlayerDebugOptions* StaticDebugOptions = nullptr;
}

namespace EARS
{
	namespace Modules
	{

		PlayerDebugOptions::PlayerDebugOptions()
		{
			PrivateDebug::StaticDebugOptions = this;
		}

		PlayerDebugOptions::~PlayerDebugOptions()
		{
			PrivateDebug::StaticDebugOptions = nullptr;
		}

		void PlayerDebugOptions::SetIsInDebugFly(const bool bInDebugFly)
		{
			if (bInDebugFly)
			{
				m_Flags.Set(1);
			}
			else
			{
				m_Flags.Clear(1);
			}
		}

		bool PlayerDebugOptions::IsInDebugFly() const
		{
			return m_Flags.Test(1);
		}

		PlayerDebugOptions* PlayerDebugOptions::GetInstance()
		{
			return PrivateDebug::StaticDebugOptions;
		}
	}
}