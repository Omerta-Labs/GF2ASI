#pragma once

#include <cstdint>

/**
 * Turns a chunk-relative offset back into a usable pointer.
 *
 * Streamed data stores pointers as offsets from the start of the chunk that
 * contains them, so every pointer field has to be fixed up once the chunk has
 * been placed in memory. A null offset stays null.
 *
 * Signature taken from the original's mangled names, which give
 * `void RecoverPtr<T>(T*&, const void*)` in the global namespace -- for
 * instance ??$RecoverPtr@D@@YAXAAPADPBX@Z for T = char. The project had a copy
 * of this in an anonymous namespace inside simmanager.cpp, which left the mod
 * loader unable to reach it once it moved out of the SDK.
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
