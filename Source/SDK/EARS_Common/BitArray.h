#pragma once

#include <cstdint>

// Fixed-capacity bitset, reconstructed from the Xbox 360 PDB (global scope, not namespaced
// in the original binary). Confirmed against two instantiations used by the engine:
//   BitArray<4096,unsigned int> -> sizeof=0x20C, align=0x4
//   BitArray<32,unsigned int>   -> sizeof=0x10
// RWS::IDArray uses BitArray<4096,unsigned int> as its sole base class (no vtable, no
// additional members), so this layout must stay bit-exact.
template<uint32_t NumBits, typename WordType = uint32_t>
class BitArray
{
public:

	BitArray()
	{
		m_ShiftBits = GetMinBitsToContain(kBitsPerWord - 1);

		// All-bits-set sentinel for a word — FindNextClearBit uses this to skip full words.
		m_FilledValue = 0;
		for (uint32_t i = 0; i < kBitsPerWord; ++i)
		{
			m_FilledValue |= (WordType(1) << i);
		}

		m_Bits.m_Size = 0;
		for (uint32_t i = 0; i < m_Bits.MaxSize(); ++i)
		{
			m_Bits.Add(WordType(0));
		}
	}

	~BitArray() = default;

	bool IsSet(uint32_t BitIndex) const
	{
		return (*GetWord(BitIndex) & GetBit(BitIndex)) != 0;
	}

	void Set(uint32_t BitIndex)
	{
		*GetWord(BitIndex) |= GetBit(BitIndex);
	}

	void Clear(uint32_t BitIndex)
	{
		*GetWord(BitIndex) &= ~GetBit(BitIndex);
	}

	// Searches forward from CurrentIndex (wrapping around) for the first clear bit.
	// Returns true and writes the found index to OutIndex; false if every bit is set.
	bool FindNextClearBit(uint32_t CurrentIndex, uint32_t& OutIndex)
	{
		bool bFound = false;

		const uint32_t StartWordIdx = CurrentIndex >> m_ShiftBits;
		uint32_t WordIdx = StartWordIdx;

		do
		{
			const WordType Word = m_Bits[WordIdx];
			if (Word != m_FilledValue)
			{
				WordType Bits = Word;
				uint32_t BitPos = 0;
				while (Bits & 1)
				{
					++BitPos;
					Bits >>= 1;
				}

				const uint32_t ResultBit = (WordIdx << m_ShiftBits) + BitPos;
				if (ResultBit < NumBits)
				{
					OutIndex = ResultBit;
					bFound = true;
				}

				break;
			}

			WordIdx = (WordIdx + 1 < m_Bits.MaxSize()) ? (WordIdx + 1) : 0;

		} while (WordIdx != StartWordIdx);

		return bFound;
	}

	static uint32_t GetMinBitsToContain(uint32_t Num)
	{
		uint32_t Count = 0;
		while (Num > 0)
		{
			++Count;
			Num >>= 1;
		}

		return Count;
	}

protected:

	WordType* GetWord(uint32_t BitIndex) { return &m_Bits[BitIndex >> m_ShiftBits]; }
	const WordType* GetWord(uint32_t BitIndex) const { return &m_Bits[BitIndex >> m_ShiftBits]; }

	WordType GetBit(uint32_t BitIndex) const { return WordType(1) << (BitIndex & (kBitsPerWord - 1)); }

private:

	static constexpr uint32_t kBitsPerWord = sizeof(WordType) * 8;
	static constexpr uint32_t kNumWords = (NumBits + kBitsPerWord - 1) / kBitsPerWord;

	// PDB nested type name: BitArray<4096,unsigned int>::StaticBitArray
	struct StaticBitArray
	{
		WordType& operator[](uint32_t Idx) { return m_Array[Idx]; }
		const WordType& operator[](uint32_t Idx) const { return m_Array[Idx]; }

		uint32_t Size() const { return m_Size; }
		uint32_t MaxSize() const { return kNumWords; }

		bool Add(const WordType& Elem)
		{
			if (m_Size >= kNumWords)
			{
				return false;
			}

			m_Array[m_Size] = Elem;
			++m_Size;
			return true;
		}

		WordType m_Array[kNumWords];
		uint32_t m_Size = 0;
	};

	WordType m_ShiftBits;
	WordType m_FilledValue;
	StaticBitArray m_Bits;
};

static_assert(sizeof(BitArray<4096, uint32_t>) == 0x20C, "BitArray<4096,uint> layout must match the confirmed PDB sizeof");
static_assert(sizeof(BitArray<32, uint32_t>) == 0x10, "BitArray<32,uint> layout must match the confirmed PDB sizeof");
