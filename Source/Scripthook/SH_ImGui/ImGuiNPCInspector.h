#pragma once

// forward declaring
namespace EARS
{
	namespace Modules
	{
		class Sentient;
		class UpgradeComponent;
	}
}

// displays a window to render an NPC element
// This also works for Players
// TODO: More like sentient?
struct ImGuiNPCInspector
{
public:

	void Initialise(EARS::Modules::Sentient* InSentient, const bool bInPlayerObject);

	void DrawWindow();

	void Clear();

	bool IsActive() const { return ActiveObject != nullptr; }

private:

	void DrawTab_GeneralState();

	void DrawTab_Appearance();

	// utility to fetch specific parts of an object depending on type
	EARS::Modules::UpgradeComponent* GetUpgradeComponent() const;

	EARS::Modules::Sentient* ActiveObject = nullptr;

	bool bIsPlayer = false;

};

