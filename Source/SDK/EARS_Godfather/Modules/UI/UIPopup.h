#pragma once

#include "SDK/EARS_Common/Bitflags.h"
#include "SDK/EARS_Common/String.h"

namespace EARS::Apt
{
	struct UIPopupInfo
	{
	public:

		UIPopupInfo();
		~UIPopupInfo();

		enum class PopupStyle
		{
			STANDARD,
			STANDARD_NO_BACK_BUTTON,
			YES_NO,
			FULL_SCREEN,
			STANDARD2,
			CONTINUE2,
			FULL_SCREEN_NO_BACK_BUTTON,
			CONTINUE_LARGE,
			STANDARD_LARGE,
			YES_NO_LARGE,
			STANDARD_LARGE_NO_BACK_BUTTON
		};

		enum class PopupFlags : int32_t
		{
			USE_PAUSE_EVENT = 0x1,
			IGNORE_INPUT = 0x2,
			IGNORE_BACK_BUTTON = 0x4,
		};

		using PopupCallbackFunc = void(__cdecl*)(int, EARS::Apt::UIPopupInfo*, bool);

		/** assign popup style. Call before Show(). */
		void SetStyle(PopupStyle InStyle) { m_Style = InStyle; }

		/** assign specific back button flag. Call before Show(). */
		void SetIgnoreBackButton(bool bIgnoreBackButton) { m_Flags.Set((int)PopupFlags::IGNORE_BACK_BUTTON, bIgnoreBackButton); }

		/** assign a string to the popup's title. Call before Show(). */
		void SetTitle(const char* InTitle);

		/** assign a string to the popup's description. Call before Show(). */
		void SetDescription(const char* InDescription);

		/** assign a callback function to trigger when the Popup's buttons has been pressed. */
		void SetCallback(PopupCallbackFunc InFunction) { m_CallbackFunction = InFunction; }

		/** include an option into the popup. Call before Show(). Max is 8. */
		void AddOption(const char* InText, bool bSelected, int32_t InUserValue);

		/** Grab the user value associated with the option index. */
		int32_t GetUserValue(const uint32_t InOptionIndex);

		/** Call when Popup is ready to be shown. */
		void Show();

	private:

		static const uint32_t MAX_OPTIONS = 8;

		Flags32 m_Flags;
		uint32_t m_NumOptions = 0;
		int32_t m_SelectedIdx = -1;
		PopupStyle m_Style = PopupStyle::STANDARD;
		void* m_UserData = nullptr;
		PopupCallbackFunc m_CallbackFunction;
		uint32_t m_Unk18 = 0;
		uint32_t m_BackButtonHashId = 0;
		uint32_t m_AcceptButtonHashId = 0;
		uint8_t m_Unk24[8];
		String m_DescriptionStr;
		String m_TitleStr;
		String m_OptionStrArray[MAX_OPTIONS];
		int32_t m_UserValueArray[MAX_OPTIONS];
		uint8_t m_UnkC8[4];
	};

	static_assert(sizeof(UIPopupInfo) == 0xF0);
}