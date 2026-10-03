#pragma once

// SDK
#include <modules/player/playerdebug.h>

namespace SH
{
	class PlayerDebugOptions_Modded : public EARS::Modules::PlayerDebugOptions
	{
	public:

		PlayerDebugOptions_Modded();
		virtual ~PlayerDebugOptions_Modded();

		void SetAnimViewMode(bool bIsActive);
		bool IsInAnimViewMode() const;

		static PlayerDebugOptions_Modded* GetInstance();

	private:

		bool bAnimViewMode = false;
	};
}
