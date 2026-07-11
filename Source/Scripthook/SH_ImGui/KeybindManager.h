#pragma once

#include "Utils/Singleton.h"

// C++
#include <functional>
#include <string>
#include <vector>

namespace SH
{
	/**
	 * A single bindable shortcut: a named action plus a keyboard trigger.
	 *
	 * Actions are registered by their owners (see ImGuiManager::RegisterShortcutActions)
	 * so the behaviour lives next to the state it mutates. KeybindManager only knows the
	 * id, the key and how to invoke the callback - which is also the seam a future Lua
	 * host would register through (RegisterAction / TriggerAction), so nothing here needs
	 * to change to add scripting later.
	 */
	struct ShortcutAction
	{
		// Stable identifier, also the key written to the [Keybinds] ini section.
		// Convention: "category.name", e.g. "player.godmode".
		std::string Id;

		// Human readable label shown in the rebinding UI.
		std::string DisplayName;

		// Grouping/context label shown in the UI, e.g. "Player", "World", "Camera".
		std::string Category;

		// Virtual-key seeded into the binding when the ini has no entry for Id.
		int DefaultVK = 0;

		// Invoked once per key press when the bound key goes down.
		std::function<void()> Callback;

		// Optional: reports whether a toggle action is currently active, so the UI
		// can show its state. Leave empty for one-shot actions.
		std::function<bool()> IsActive;

		// Runtime binding, seeded from DefaultVK then overridden by LoadBindings.
		// 0 means unbound.
		int BoundVK = 0;

		// Runtime: whether the bound key was down last poll. Drives our own edge
		// detection so we don't rely on GetAsyncKeyState's press latch, which would
		// otherwise leak a press made while input was suppressed (e.g. while typing).
		bool bWasDown = false;
	};

	/**
	 * Owns the shortcut action registry and their key bindings, loads/saves them from
	 * the [Keybinds] ini section, and dispatches key presses each tick.
	 *
	 * All access happens on the SIM thread (registration during Open, dispatch in
	 * ImGuiManager::OnTick, rebinding from the mod menu draw in the same tick), so no
	 * additional locking is required.
	 */
	class KeybindManager : public SH::Singleton<KeybindManager>
	{
	public:

		// Add an action to the registry. Its binding starts at Action.DefaultVK until
		// LoadBindings overrides it from the ini. Ignores empty or duplicate ids.
		void RegisterAction(ShortcutAction Action);

		// Read a virtual-key for every registered action from [Keybinds] in the given
		// ini file, falling back to each action's DefaultVK when the key is absent.
		// Caches the path so later saves target the same file.
		void LoadBindings(const std::wstring& InConfigFilePath);

		// Write every action's current binding back to [Keybinds] as hex.
		void Save() const;

		// Edge-triggered dispatch: fire the callback of any action whose key went down
		// since the last call. Suppressed while a rebind capture is in progress or ImGui
		// is consuming text input.
		void PollAndDispatch();

		// Immediately run an action by id (used by the UI and future scripting hosts).
		void TriggerAction(const std::string& InId) const;

		// Current binding for an action id, or 0 if unbound / unknown.
		int GetBinding(const std::string& InId) const;

		// Set (or clear, with 0) the binding for an action id and persist it.
		void SetBinding(const std::string& InId, int InVirtualKey);

		const std::vector<ShortcutAction>& GetActions() const { return Actions; }

		// Convert a virtual-key code to its localized display name (e.g. 0x74 -> "F5",
		// 0x21 -> "Page Up"). Returns an empty string for 0 (unbound) and falls back to
		// the raw "0xNN" code for keys the OS can't name.
		static std::string GetKeyDisplayName(int InVirtualKey);

		// Rebind capture, driven by the mod menu. While an id is captured, dispatch is
		// suppressed and the next key press is assigned to it (Escape cancels).
		void BeginRebind(const std::string& InId) { RebindTargetId = InId; }
		void CancelRebind() { RebindTargetId.clear(); }
		bool IsRebinding() const { return !RebindTargetId.empty(); }
		const std::string& GetRebindTargetId() const { return RebindTargetId; }

	private:

		ShortcutAction* FindAction(const std::string& InId);
		const ShortcutAction* FindAction(const std::string& InId) const;

		// Scan the keyboard for the first held key and assign it to the rebind target.
		void ProcessRebindCapture();

		// Whether the game window is the foreground window, so shortcuts don't fire
		// while the user is in another application.
		static bool IsGameWindowFocused();

		std::vector<ShortcutAction> Actions;

		// Cached ini path so Save/SetBinding can persist without re-resolving it.
		std::wstring ConfigFilePath;

		// Non-empty while the UI is waiting to capture a key for this action id.
		std::string RebindTargetId;
	};
}
