#pragma once

#include "SDK/EARS_Framework/Core/Entity/Entity.h"

namespace EARS::Modules
{
	// forward declares
	class BuildingStore;

	class Building : public EARS::Framework::Entity
	{
	public:

		BuildingStore* GetStorageUnit() const { return m_Store; }

	private:

		EARS::Modules::BuildingStore* m_Store = nullptr;
	};
}