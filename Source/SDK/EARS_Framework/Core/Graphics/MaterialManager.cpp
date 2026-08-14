#include "MaterialManager.h"

// SDK
#include "MaterialHash.h"

/* static */
Material* MaterialManager::FindMaterial(const uint32_t InHash)
{
	return MatHash_FindMaterial(InHash);
}

/* static */
void MaterialManager::ForEachMaterial(std::function<void(Material&)> Functor)
{
	// TODO: Convert to MatHash functions
	auto MatIt = MaterialHashTable::GetInstance()->GetIterator();
	while (!MatIt.IsFinshed())
	{
		Functor(*MatIt.GetObject()->GetMaterial());

		MatIt++;
	}
}
