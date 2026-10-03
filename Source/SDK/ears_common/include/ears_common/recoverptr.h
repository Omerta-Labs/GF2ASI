#pragma once

#include <cstdint>

/**
 * Turns a chunk-relative offset back into a usable pointer.
 *
 * Streamed data stores pointers as offsets from the start of the chunk that
 * contains them, so every pointer field has to be fixed up once the chunk has
 * been placed in memory. A null offset stays null.
 *
 * Global namespace and this exact signature are the original's, from mangled
 * names such as ??$RecoverPtr@D@@YAXAAPADPBX@Z for T = char.
 */
template <typename TType>
void RecoverPtr(TType*& PtrToFixUp, const void* PtrBase)
{
	if (PtrToFixUp != nullptr)
	{
		const uintptr_t Offset = reinterpret_cast<uintptr_t>(PtrToFixUp);
		const uintptr_t Base = reinterpret_cast<uintptr_t>(PtrBase);
		PtrToFixUp = reinterpret_cast<TType*>(Offset + Base);
	}
}
