#include "Addons/ImGuiRuntime.h"

#include "Addons/imgui/imgui.h"
#include "Addons/imgui/backends/imgui_impl_dx9.h"
#include "Addons/imgui/backends/imgui_impl_win32.h"
#include "Platform/MemUtils.h"

#include <d3d9.h>

#include <filesystem>
#include <mutex>

// Defined in imgui_impl_win32.cpp at global scope. imgui_impl_win32.h #if 0's
// its own declaration and tells you to copy this line into your .cpp, so it has
// to sit outside the namespace below -- inside it, this declares a different
// symbol that nothing defines.
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Mod::ImGuiRuntime
{
	namespace
	{
		// The game's window and device, read from their fixed addresses rather
		// than passed in: the detours that drive this runtime have no handle to
		// either.
		// int, not uintptr_t: hook::Type's constructor takes int, and a wider
		// type only buys a narrowing warning.
		constexpr int GAME_WINDOW_HANDLE_ADDRESS = 0x112A024;
		constexpr int GAME_D3D9_DEVICE_ADDRESS = 0x1205750;

		constexpr int MAX_PANELS = 16;

		// Serialises all context access between the SIM thread (frame build in
		// Tick), the PRESENTATION thread (Present and device reset) and the
		// window thread (WndProc).
		//
		// Recursive because the Win32 backend re-enters the window proc on the
		// same thread: its handler calls ReleaseCapture(), which synchronously
		// dispatches WM_CAPTURECHANGED back into WndProc while the lock is
		// held.
		std::recursive_mutex g_ContextLock;

		// Deep copy of the last completed frame, with cloned ImDrawLists we
		// own. Present renders this rather than the live context, so it always
		// sees a whole frame however many presents happen per tick.
		ImDrawData g_Snapshot;

		PanelFn g_Panels[MAX_PANELS] = {};
		int g_PanelCount = 0;

		bool g_Open = false;
		bool g_InputOwned = false;

		bool g_HasSuppressedCursorPos = false;
		int g_SuppressedCursorPosX = 0;
		int g_SuppressedCursorPosY = 0;

		ImFont* g_CustomFont = nullptr;
		ImFont* g_DefaultFont = nullptr;

		void SetupStyle()
		{
		// Fork of Clean Dark/Red style from ImThemes
			ImGuiStyle& style = ImGui::GetStyle();

			style.WindowPadding = ImVec2(5.0f, 2.0f);
			style.WindowRounding = 4.0f;
			style.WindowBorderSize = 1.0f;
			style.WindowMinSize = ImVec2(32.0f, 32.0f);
			style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
			style.WindowMenuButtonPosition = ImGuiDir_Left;
			style.ChildRounding = 4.0f;
			style.ChildBorderSize = 1.0f;
			style.PopupRounding = 4.0f;
			style.PopupBorderSize = 1.0f;
			style.FramePadding = ImVec2(9.0f, 7.0f);
			style.FrameRounding = 4.0f;
			style.FrameBorderSize = 1.0f;
			style.ItemSpacing = ImVec2(9.0f, 8.0f);
			style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
			style.CellPadding = ImVec2(4.0f, 2.0f);
			style.IndentSpacing = 0.0f;
			style.ColumnsMinSpacing = 6.0f;
			style.ScrollbarSize = 16.0f;
			style.ScrollbarRounding = 4.0f;
			style.ScrollbarPadding = 4.0f;
			style.GrabMinSize = 10.0f;
			style.GrabRounding = 4.0f;
			style.TabRounding = 4.0f;
			style.TabBorderSize = 2.0f;
			style.TabBarBorderSize = 2.0f;
			style.TabBarOverlineSize = 1.0f;
			style.TabMinWidthBase = 1.0f;
			style.TabMinWidthShrink = 80;
			style.TouchExtraPadding = ImVec2(0.0f, 0.0f);
			//style.TabMinWidthForCloseButton = 3.0f;
			style.ColorButtonPosition = ImGuiDir_Right;
			style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
			style.SelectableTextAlign = ImVec2(0.5f, 0.0f);
			style.ColorMarkerSize = 3.0f;
			style.SelectableTextAlign = ImVec2(0.5f, 0.0f);
			style.SeparatorSize = 1.0f;
			style.SeparatorTextBorderSize = 3.0f;
			style.SeparatorTextAlign = ImVec2(0.0f, 0.5f);

			ImVec4* colors = style.Colors;
			colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			colors[ImGuiCol_TextDisabled] = ImVec4(0.73f, 0.75f, 0.74f, 1.00f);
			colors[ImGuiCol_WindowBg] = ImVec4(0.07f, 0.07f, 0.07f, 0.94f);
			colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
			colors[ImGuiCol_Border] = ImVec4(0.52f, 0.52f, 0.52f, 1.00f);
			colors[ImGuiCol_BorderShadow] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
			colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.54f);
			colors[ImGuiCol_FrameBgHovered] = ImVec4(0.21f, 0.21f, 0.21f, 0.40f);
			colors[ImGuiCol_FrameBgActive] = ImVec4(0.29f, 0.29f, 0.29f, 0.67f);
			colors[ImGuiCol_TitleBg] = ImVec4(0.14f, 0.14f, 0.14f, 0.65f);
			colors[ImGuiCol_TitleBgActive] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
			colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.14f, 0.14f, 0.14f, 0.67f);
			colors[ImGuiCol_MenuBarBg] = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
			colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
			colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
			colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.48f, 0.48f, 0.48f, 1.00f);
			colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 1.00f, 0.03f, 1.00f);
			colors[ImGuiCol_CheckboxSelectedBg] = ImVec4(0.22f, 0.48f, 0.80f, 0.45f);
			colors[ImGuiCol_SliderGrab] = ImVec4(0.75f, 0.00f, 0.00f, 1.00f);
			colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 0.38f, 0.38f, 1.00f);
			colors[ImGuiCol_Button] = ImVec4(0.00f, 0.00f, 0.00f, 0.54f);
			colors[ImGuiCol_ButtonHovered] = ImVec4(0.18f, 0.18f, 0.18f, 0.40f);
			colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.67f);
			colors[ImGuiCol_Header] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
			colors[ImGuiCol_HeaderHovered] = ImVec4(0.33f, 0.33f, 0.33f, 1.00f);
			colors[ImGuiCol_HeaderActive] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
			colors[ImGuiCol_Separator] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			colors[ImGuiCol_SeparatorHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
			colors[ImGuiCol_SeparatorActive] = ImVec4(1.00f, 0.33f, 0.33f, 1.00f);
			colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
			colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 0.49f, 0.49f, 1.00f);
			colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 0.49f, 0.49f, 1.00f);
			colors[ImGuiCol_InputTextCursor] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			colors[ImGuiCol_TabHovered] = ImVec4(0.46f, 0.46f, 0.46f, 1.00f);
			colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
			colors[ImGuiCol_TabSelected] = ImVec4(0.47f, 0.00f, 0.00f, 1.00f);
			colors[ImGuiCol_TabSelectedOverline] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
			colors[ImGuiCol_TabDimmed] = ImVec4(0.15f, 0.07f, 0.07f, 0.97f);
			colors[ImGuiCol_TabDimmedSelected] = ImVec4(0.40f, 0.15f, 0.15f, 1.00f);
			colors[ImGuiCol_TabDimmedSelectedOverline] = ImVec4(0.55f, 0.55f, 0.55f, 0.00f);
			colors[ImGuiCol_DockingPreview] = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
			colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
			colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
			colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
			colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.00f, 0.00f, 1.00f);
			colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.36f, 0.00f, 0.00f, 1.00f);
			colors[ImGuiCol_TableHeaderBg] = ImVec4(0.34f, 0.34f, 0.34f, 1.00f);
			colors[ImGuiCol_TableBorderStrong] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
			colors[ImGuiCol_TableBorderLight] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
			colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
			colors[ImGuiCol_TextLink] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
			colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.64f, 0.88f, 0.44f);
			colors[ImGuiCol_TreeLines] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
			colors[ImGuiCol_DragDropTarget] = ImVec4(0.47f, 0.18f, 0.18f, 0.97f);
			colors[ImGuiCol_DragDropTargetBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
			colors[ImGuiCol_UnsavedMarker] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
			colors[ImGuiCol_NavCursor] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
			colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
			colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
			colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
		}

		// Expects g_ContextLock to be held.
		void ClearSnapshot()
		{
			// ImDrawData::Clear() does not free the lists -- it assumes the
			// context owns them, but ours are clones.
			for (ImDrawList* ClonedList : g_Snapshot.CmdLists)
			{
				IM_DELETE(ClonedList);
			}

			g_Snapshot.Clear();
		}

		// Expects g_ContextLock to be held.
		void CaptureSnapshot()
		{
			ClearSnapshot();

			const ImDrawData* SourceData = ImGui::GetDrawData();
			if (!SourceData || !SourceData->Valid)
			{
				return;
			}

			// Copy the scalar fields (counts, display rect, texture list
			// pointer), then swap the context-owned draw lists for clones we
			// own. The clones stay valid while the context recycles its lists
			// on the next NewFrame.
			g_Snapshot = *SourceData;
			g_Snapshot.CmdLists.resize(0);
			for (const ImDrawList* SourceList : SourceData->CmdLists)
			{
				g_Snapshot.CmdLists.push_back(SourceList->CloneOutput());
			}
		}
	}

	bool AddFont(const char* const Path)
	{
		if (Path == nullptr || !std::filesystem::exists(Path))
		{
			return false;
		}

		ImGuiIO& IO = ImGui::GetIO();
		IO.Fonts->Clear();

		static const ImWchar RangeEverything[] =
		{
			0x0020, 0xFFFF,
			0,
		};

		constexpr float FONT_UPSCALE = 1.0f;
		constexpr float FONT_SIZE = 15.0f;

		g_CustomFont = IO.Fonts->AddFontFromFileTTF(
			Path, FONT_SIZE * FONT_UPSCALE, nullptr, &RangeEverything[0]);
		if (g_CustomFont == nullptr)
		{
			g_CustomFont = IO.Fonts->AddFontDefault();
		}

		g_DefaultFont = IO.Fonts->AddFontDefault();
		return true;
	}

	bool Open()
	{
		// The EndScene and WndProc detours may already be live on other
		// threads.
		std::lock_guard<std::recursive_mutex> ContextLock(g_ContextLock);

		if (g_Open)
		{
			return true;
		}

		const HWND Window = hook::Type<HWND>(GAME_WINDOW_HANDLE_ADDRESS);
		IDirect3DDevice9* const Device =
			hook::Type<IDirect3DDevice9*>(GAME_D3D9_DEVICE_ADDRESS);
		if (Window == nullptr || Device == nullptr)
		{
			return false;
		}

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& IO = ImGui::GetIO();
		IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		IO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

		ImGui_ImplWin32_Init(Window);
		ImGui_ImplDX9_Init(Device);

		AddFont("scripts/Roboto-Medium.ttf");
		SetupStyle();

		g_Open = true;
		return true;
	}

	void Close()
	{
		std::lock_guard<std::recursive_mutex> ContextLock(g_ContextLock);

		if (!g_Open)
		{
			return;
		}

		ClearSnapshot();

		ImGui_ImplWin32_Shutdown();
		ImGui_ImplDX9_Shutdown();

		g_Open = false;
	}

	bool RegisterPanel(const PanelFn Panel)
	{
		if (Panel == nullptr || g_PanelCount >= MAX_PANELS)
		{
			return false;
		}

		g_Panels[g_PanelCount++] = Panel;
		return true;
	}

	void Tick()
	{
		std::lock_guard<std::recursive_mutex> ContextLock(g_ContextLock);

		if (!g_Open)
		{
			return;
		}

		ImGuiIO& IO = ImGui::GetIO();
		IO.MouseDrawCursor = g_InputOwned;

		// The window detour feeds every input message to the backend and the
		// Win32 handler swallows none of them, so ImGui would still hover and
		// click against a cursor the game is busy recentring. Mask its input
		// off while it does not own the mouse.
		if (g_InputOwned)
		{
			IO.ConfigFlags &= ~(ImGuiConfigFlags_NoMouse | ImGuiConfigFlags_NoKeyboard);
		}
		else
		{
			IO.ConfigFlags |= (ImGuiConfigFlags_NoMouse | ImGuiConfigFlags_NoKeyboard);
		}

		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		for (int Index = 0; Index < g_PanelCount; ++Index)
		{
			g_Panels[Index]();
		}

		ImGui::EndFrame();
		ImGui::Render();

		CaptureSnapshot();
	}

	void Present()
	{
		std::lock_guard<std::recursive_mutex> ContextLock(g_ContextLock);

		if (g_Snapshot.Valid)
		{
			ImGui_ImplDX9_RenderDrawData(&g_Snapshot);
		}
	}

	void OnDeviceLost()
	{
		std::lock_guard<std::recursive_mutex> ContextLock(g_ContextLock);

		// The snapshot references textures that are about to be destroyed.
		ClearSnapshot();

		ImGui_ImplDX9_InvalidateDeviceObjects();
	}

	void DropFrame()
	{
		std::lock_guard<std::recursive_mutex> ContextLock(g_ContextLock);

		ClearSnapshot();
	}

	void OnDeviceRestored()
	{
		std::lock_guard<std::recursive_mutex> ContextLock(g_ContextLock);

		ImGui_ImplDX9_CreateDeviceObjects();
	}

	LRESULT WndProc(const HWND Window, const UINT Message,
	                const WPARAM wParam, const LPARAM lParam)
	{
		// The handler appends to the context's shared input queue, which must
		// not overlap the SIM thread's frame build.
		std::lock_guard<std::recursive_mutex> ContextLock(g_ContextLock);

		return ImGui_ImplWin32_WndProcHandler(Window, Message, wParam, lParam);
	}

	void SetInputOwned(const bool bOwned)
	{
		g_InputOwned = bOwned;
	}

	bool IsInputOwned()
	{
		return g_InputOwned;
	}

	void NotifySuppressedCursorPos(const int X, const int Y)
	{
		g_SuppressedCursorPosX = X;
		g_SuppressedCursorPosY = Y;
		g_HasSuppressedCursorPos = true;
	}

	void RestoreCursorPos()
	{
		if (!g_HasSuppressedCursorPos)
		{
			return;
		}

		// Safe to go straight at the API: the detour only swallows the call
		// while input is owned, and this runs after that has been cleared.
		::SetCursorPos(g_SuppressedCursorPosX, g_SuppressedCursorPosY);
		g_HasSuppressedCursorPos = false;
	}
}
