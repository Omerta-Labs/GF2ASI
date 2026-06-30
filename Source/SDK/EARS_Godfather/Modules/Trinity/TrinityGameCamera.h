#pragma once

#include "SDK/EARS_Trinity/include/TrinityCamera.h"

namespace EARS::Modules
{
	class Allocatable
	{
	public:

		virtual ~Allocatable() = 0;
	};

	class ListNode : public Allocatable
	{
		ListNode* m_Prev;
		ListNode* m_Next;
	};

	class TrinityGameCamera : public EARS::Modules::ListNode, public EA::Trinity::TrinityCamera
	{
	public:

		virtual ~TrinityGameCamera() = 0;

		//~ Begin EA::Trinity::TrinityCamera interface
		virtual void BeginUpdate() override;
		//~ End EA::Trinity::TrinityCamera interface

	private:

		uint32_t m_PlayerID = 0;
	};

	static_assert(sizeof(TrinityGameCamera) == 0xC0);
}
