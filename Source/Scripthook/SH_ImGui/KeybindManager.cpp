#include "KeybindManager.h"

// Addons
#include "Addons/tConsole.h"
#include "Addons/imgui/imgui.h"

// C++
#include <cstdio>
#include <cwchar>
#include <windows.h>

namespace
{
	// Action ids are ASCII, so a straight widening is enough to hand them to the
	// Win32 profile API (which has no narrow-string variant we use elsewhere).
	std::wstring WidenAscii(const std::string& InText)
	{
		return std::wstring(InText.begin(), InText.end());
	}

	// Read a virtual-key from the ini. The value is stored as text, so parse it with
	// base 0 to accept both "0x74" and plain decimal. Returns the default on a
	// missing or malformed entry.
	int ReadVirtualKey(const wchar_t* InSection, const std::wstring& InKey, const int InDefaultVK, const wchar_t* InFilePath)
	{
		wchar_t ValueBuffer[32] = {};
		GetPrivateProfileStringW(InSection, InKey.c_str(), L"", ValueBuffer, ARRAYSIZE(ValueBuffer), InFilePath);

		if (ValueBuffer[0] == L'\0')
		{
			return InDefaultVK;
		}

		wchar_t* ParseEnd = nullptr;
		const long ParsedValue = std::wcstol(ValueBuffer, &ParseEnd, 0);
		if (ParseEnd == ValueBuffer || ParsedValue < 0 || ParsedValue > 0xFF)
		{
			return InDefaultVK;
		}

		return static_cast<int>(ParsedValue);
	}

	// Write a virtual-key back as hex to match the .ini's existing style.
	void WriteVirtualKey(const wchar_t* InSection, const std::wstring& InKey, const int InVirtualKey, const wchar_t* InFilePath)
	{
		wchar_t ValueBuffer[16] = {};
		swprintf_s(ValueBuffer, L"0x%02X", InVirtualKey);

		WritePrivateProfileStringW(InSection, InKey.c_str(), ValueBuffer, InFilePath);
	}

	constexpr wchar_t KEYBINDS_SECTION[] = L"Keybinds";
}

namespace SH
{
	void KeybindManager::RegisterAction(ShortcutAction Action)
	{
		if (Action.Id.empty())
		{
			tConsole::fWriteLine("KeybindManager: ignoring action with empty id");
			return;
		}

		if (FindAction(Action.Id) != nullptr)
		{
			tConsole::fPrintf("KeybindManager: ignoring duplicate action id \"%s\"", Action.Id.c_str());
			return;
		}

		// Seed the runtime binding from the default until the ini is loaded.
		Action.BoundVK = Action.DefaultVK;
		Actions.push_back(std::move(Action));
	}

	void KeybindManager::LoadBindings(const std::wstring& InConfigFilePath)
	{
		ConfigFilePath = InConfigFilePath;

		if (ConfigFilePath.empty())
		{
			tConsole::fWriteLine("KeybindManager: no config path, keeping default bindings");
			return;
		}

		for (ShortcutAction& Action : Actions)
		{
			Action.BoundVK = ReadVirtualKey(KEYBINDS_SECTION, WidenAscii(Action.Id), Action.DefaultVK, ConfigFilePath.c_str());
			tConsole::fPrintf("Keybind \"%s\" -> 0x%02X", Action.Id.c_str(), Action.BoundVK);
		}
	}

	void KeybindManager::Save() const
	{
		if (ConfigFilePath.empty())
		{
			tConsole::fWriteLine("KeybindManager::Save called before LoadBindings, ignoring");
			return;
		}

		for (const ShortcutAction& Action : Actions)
		{
			WriteVirtualKey(KEYBINDS_SECTION, WidenAscii(Action.Id), Action.BoundVK, ConfigFilePath.c_str());
		}
	}

	void KeybindManager::PollAndDispatch()
	{
		// Snapshot before capturing: ProcessRebindCapture may clear the rebind state
		// this tick, but we still want this whole tick suppressed so the key that was
		// just assigned (and is still held) doesn't immediately fire its action.
		const bool bWasRebinding = IsRebinding();
		if (bWasRebinding)
		{
			ProcessRebindCapture();
		}

		// Suppress firing - but still track key state below - when the game isn't the
		// foreground window, while rebinding, or while ImGui is taking text input.
		// Tracking state during suppression is what stops a press made while typing
		// from leaking out as a shortcut the moment typing stops.
		const bool bSuppress = bWasRebinding
			|| !IsGameWindowFocused()
			|| ImGui::GetIO().WantTextInput;

		for (ShortcutAction& Action : Actions)
		{
			if (Action.BoundVK == 0)
			{
				Action.bWasDown = false;
				continue;
			}

			// High bit = currently held. Unlike the low-bit "pressed since last call"
			// latch, this is a live read, so we can poll it every tick and derive our
			// own rising edge that stays correct across suppressed frames.
			const bool bIsDown = (GetAsyncKeyState(Action.BoundVK) & 0x8000) != 0;
			const bool bJustPressed = bIsDown && !Action.bWasDown;
			Action.bWasDown = bIsDown;

			if (bJustPressed && !bSuppress && Action.Callback)
			{
				Action.Callback();
			}
		}
	}

	bool KeybindManager::IsGameWindowFocused()
	{
		const ImGuiViewport* MainViewport = ImGui::GetMainViewport();
		const HWND GameWindow = MainViewport ? static_cast<HWND>(MainViewport->PlatformHandleRaw) : nullptr;

		// If we can't identify the window, don't over-suppress and break shortcuts.
		if (GameWindow == nullptr)
		{
			return true;
		}

		// Mirror the Win32 backend's own focus test: the game window, or a child of it
		// (e.g. a temporary dialog), counts as focused.
		const HWND Foreground = GetForegroundWindow();
		return Foreground == GameWindow || IsChild(GameWindow, Foreground);
	}

	void KeybindManager::TriggerAction(const std::string& InId) const
	{
		if (const ShortcutAction* Action = FindAction(InId))
		{
			if (Action->Callback)
			{
				Action->Callback();
			}
		}
	}

	int KeybindManager::GetBinding(const std::string& InId) const
	{
		const ShortcutAction* Action = FindAction(InId);
		return Action ? Action->BoundVK : 0;
	}

	void KeybindManager::SetBinding(const std::string& InId, const int InVirtualKey)
	{
		ShortcutAction* Action = FindAction(InId);
		if (!Action)
		{
			return;
		}

		Action->BoundVK = InVirtualKey;

		// Persist immediately so a crash or hard exit doesn't lose the rebind.
		if (!ConfigFilePath.empty())
		{
			WriteVirtualKey(KEYBINDS_SECTION, WidenAscii(InId), InVirtualKey, ConfigFilePath.c_str());
		}
	}

	void KeybindManager::ProcessRebindCapture()
	{
		// Escape cancels the capture, leaving the existing binding untouched.
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
		{
			CancelRebind();
			return;
		}

		// Assign the first key currently held down. The range starts past the mouse
		// buttons (0x01-0x06) so a click on the Rebind button can't bind itself.
		for (int VirtualKey = 0x08; VirtualKey <= 0xFE; ++VirtualKey)
		{
			if (GetAsyncKeyState(VirtualKey) & 0x8000)
			{
				SetBinding(RebindTargetId, VirtualKey);
				CancelRebind();
				return;
			}
		}
	}

	std::string KeybindManager::GetKeyDisplayName(const int InVirtualKey)
	{
		if (InVirtualKey == 0)
		{
			return std::string();
		}

		// Mouse buttons have no keyboard scan code, so name them ourselves.
		switch (InVirtualKey)
		{
		case VK_LBUTTON:  return "Mouse Left";
		case VK_RBUTTON:  return "Mouse Right";
		case VK_MBUTTON:  return "Mouse Middle";
		case VK_XBUTTON1: return "Mouse X1";
		case VK_XBUTTON2: return "Mouse X2";
		default:          break;
		}

		const UINT ScanCode = MapVirtualKeyW(static_cast<UINT>(InVirtualKey), MAPVK_VK_TO_VSC);

		// Extended keys share scan codes with the numpad; without the extended bit
		// GetKeyNameText reports the numpad name (e.g. "Num 9" instead of "Page Up").
		bool bExtended = false;
		switch (InVirtualKey)
		{
		case VK_PRIOR: case VK_NEXT:
		case VK_END:   case VK_HOME:
		case VK_INSERT: case VK_DELETE:
		case VK_LEFT:  case VK_UP: case VK_RIGHT: case VK_DOWN:
		case VK_NUMLOCK: case VK_DIVIDE:
		case VK_RCONTROL: case VK_RMENU:
			bExtended = true;
			break;
		default:
			break;
		}

		LONG NameParam = (static_cast<LONG>(ScanCode & 0xFF) << 16);
		if (bExtended)
		{
			NameParam |= (1 << 24);
		}

		wchar_t NameBuffer[128] = {};
		if (ScanCode != 0 && GetKeyNameTextW(NameParam, NameBuffer, ARRAYSIZE(NameBuffer)) > 0)
		{
			const int Utf8Length = WideCharToMultiByte(CP_UTF8, 0, NameBuffer, -1, nullptr, 0, nullptr, nullptr);
			if (Utf8Length > 1)
			{
				// Utf8Length includes the null terminator; size the string without it.
				std::string Result(static_cast<size_t>(Utf8Length - 1), '\0');
				WideCharToMultiByte(CP_UTF8, 0, NameBuffer, -1, Result.data(), Utf8Length, nullptr, nullptr);
				return Result;
			}
		}

		// Unmapped key - fall back to the raw code so it is still identifiable.
		char Fallback[16] = {};
		sprintf_s(Fallback, "0x%02X", InVirtualKey);
		return Fallback;
	}

	ShortcutAction* KeybindManager::FindAction(const std::string& InId)
	{
		for (ShortcutAction& Action : Actions)
		{
			if (Action.Id == InId)
			{
				return &Action;
			}
		}

		return nullptr;
	}

	const ShortcutAction* KeybindManager::FindAction(const std::string& InId) const
	{
		for (const ShortcutAction& Action : Actions)
		{
			if (Action.Id == InId)
			{
				return &Action;
			}
		}

		return nullptr;
	}
}
