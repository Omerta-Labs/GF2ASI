#pragma once

#include "SDK/ears_framework/src/framework/core/entity/Entity.h"

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