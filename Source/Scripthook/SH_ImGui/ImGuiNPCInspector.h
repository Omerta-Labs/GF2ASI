#pragma once

// SDK
#include "SDK/EARS_Godfather/Modules/FamilyTree/PlayerFamilyTree.h"

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

	void DrawTab_Upgrades();

	// utility to fetch specific parts of an object depending on type
	EARS::Modules::UpgradeComponent* GetUpgradeComponent() const;

	// utility to get the tree slot assigned to this NPC, if any.
	// this is used for crew members of the Players family
	EARS::Modules::PlayerFamilyTree::FamilyTreeSlot GetTreeSlot() const;

	EARS::Modules::Sentient* ActiveObject = nullptr;

	bool bIsPlayer = false;

};

