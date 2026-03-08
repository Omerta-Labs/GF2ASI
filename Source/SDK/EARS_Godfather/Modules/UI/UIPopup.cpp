#include "UIPopup.h"

// Addons
#include "Addons/Hook.h"

// C++
#include <assert.h>

namespace EARS::Apt
{
	UIPopupInfo::UIPopupInfo()
		: m_AcceptButtonHashId(0xDD340FEC)
		, m_BackButtonHashId(0xB45419AB)
	{
	}

	UIPopupInfo::~UIPopupInfo()
	{
		// TODO: implement destructor, by default blows up game
		MemUtils::CallClassMethod<void, UIPopupInfo*>(0x09823B0, this);
	}

	void UIPopupInfo::SetTitle(const char* InTitle)
	{
		m_TitleStr.assign(InTitle);
	}

	void UIPopupInfo::SetDescription(const char* InDescription)
	{
		m_DescriptionStr.assign(InDescription);
	}

	void UIPopupInfo::AddOption(const char* InText, bool bSelected, int32_t InUserValue)
	{
		assert(m_NumOptions < 8);

		m_OptionStrArray[m_NumOptions].assign(InText);
		m_UserValueArray[m_NumOptions] = InUserValue;

		if (bSelected)
		{
			m_SelectedIdx = m_NumOptions;
		}

		m_NumOptions++;
	}

	void UIPopupInfo::Show()
	{
		MemUtils::CallClassMethod<void, UIPopupInfo*>(0x0982E10, this);
	}
}
