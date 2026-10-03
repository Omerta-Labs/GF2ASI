#pragma once

// C++
#include <stdint.h>

namespace EARS
{
	namespace Modules
	{
		/**
		 * Flat bit array sized once at construction.
		 *
		 * ControllerManager keeps one per controller to hold "which events fired this
		 * frame", sized to the maxEvents the manager was constructed with (0x80 on PC).
		 *
		 * NB: this lives here because ControllerManager is its only consumer in the code
		 * reconstructed so far; the shipping source almost certainly declared it in a
		 * shared EARS::Modules header. Move it if a second user turns up.
		 */
		class Bitmask
		{
		public:

			uint32_t GetSize() const { return m_Size; }
			uint16_t GetNumBytes() const { return m_numBytes; }

			bool TestBit(uint32_t Index) const
			{
				const uint8_t Bit = static_cast<uint8_t>(1u << (Index & 7));
				return (m_Bits[Index >> 3] & Bit) != 0;
			}

			void SetBit(uint32_t Index) { m_Bits[Index >> 3] |= static_cast<uint8_t>(1u << (Index & 7)); }
			void ClearBit(uint32_t Index) { m_Bits[Index >> 3] &= static_cast<uint8_t>(~(1u << (Index & 7))); }

			/* Zero every byte. (game: 0x004CDF80) */
			void ClearAll();

		private:

			uint8_t* m_Bits = nullptr;	// 0x00
			uint32_t m_Size = 0;		// 0x04 - capacity in bits
			uint16_t m_numBytes = 0;	// 0x08
		};

		static_assert(sizeof(EARS::Modules::Bitmask) == 0xC, "EARS::Modules::Bitmask must equal 0xC");
	} // Modules
} // EARS
