#pragma once

// C++
#include <functional>
#include <stdint.h>

// forward declares
struct Material;

class MaterialManager
{
public:

	/** resolve material from hash table */
	static Material* FindMaterial(const uint32_t InHash);

	/** iterate through every single material */
	static void ForEachMaterial(std::function<void(Material&)> Functor);
};
