#pragma once

// SDK
#include "SDK/EARS_Common/CommonTypes.h"

namespace EARS
{
	namespace Common
	{
		template<typename TKey, typename TValue, class TCompare = CompareFunc<TKey>, class THash = HashFunc<TKey>, class TGetKey = GetKeyFunc<TValue>, class TGetValue = HashNext<TValue>>
		struct IntrusiveHashTable
		{
		public:

			using THashTableType = IntrusiveHashTable<TKey, TValue, TCompare, THash, TGetKey, TGetValue>;

			uint32_t GetBin(const TKey& Key) const
			{
				return THash::Hash(Key) % m_NumBins;
			}

			uint32_t GetBin(const TValue& Value) const
			{
				const TKey& ValueKey = TGetKey::GetKey(&Value);
				return GetBin(ValueKey);
			}

			TValue* FindEntry(const TKey& Key) const
			{
				for (auto i = m_BinArray[GetBin(Key)]; i; i = TGetValue::GetHashNext(*i))
				{
					const TKey& ValueKey = TGetKey::GetKey(i);
					if (TCompare::Equal(ValueKey, Key))
					{
						return i;
					}
				}

				TValue* CurVal = m_BinArray[GetBin(Key)];
				while (CurVal)
				{
					const TKey& ValueKey = TGetKey::GetKey(CurVal);
					if (TCompare::Equal(ValueKey, Key))
					{
						return CurVal;
					}

					CurVal = TGetValue::GetHashNext(*CurVal);
				}

				return nullptr;
			}

			void Clear()
			{
				m_NumEntries = 0;
				memset(m_BinArray, 0, 4 * m_NumBins);
			}

			uint32_t GetNumBins() const { return m_NumBins; }

			struct Iterator
			{
			public:

				Iterator(THashTableType* InHashTable)
					: m_HashTable(InHashTable)
					, m_NextBin(0)
					, m_Entry(nullptr)
				{
					Reset();
				}

				void WalkToValidEntry()
				{
					if (const THashTableType* CurHashTable = m_HashTable)
					{
						while (!m_Entry && m_NextBin < CurHashTable->GetNumBins())
						{
							m_Entry = CurHashTable->m_BinArray[m_NextBin++];
						}
					}
				}

				void Reset()
				{
					m_NextBin = 0;
					m_Entry = nullptr;

					WalkToValidEntry();
				}

				bool IsFinshed() const { return m_Entry == nullptr; }

				TValue* GetObject() const { return m_Entry; }

				// operator overloads
				TValue* operator*()
				{
					return GetObject();
				}

				Iterator& operator++(int a1)
				{
					m_Entry = TGetValue::GetHashNext(*m_Entry);
					WalkToValidEntry();
					return *this;
				}

			private:

				THashTableType* m_HashTable = nullptr;
				uint32_t m_NextBin = 0;
				TValue* m_Entry = nullptr;	
			};

			Iterator CreateIterator()
			{
				return Iterator(this);
			}

		private:

			TValue** m_BinArray = nullptr;
			uint32_t m_NumBins = 0;
			uint32_t m_NumEntries = 0;
			bool m_bGrowable = false;
		};
	}
}

#define DEFINE_MEMBER_IntrusiveHashTable(Key, Value, GetKeyFunc, GetValueFunc, MemberName) EARS::Common::IntrusiveHashTable<Key, Value, EARS::Common::CompareFunc<Key>, EARS::Common::HashFunc<Key>, GetKeyFunc, GetValueFunc> MemberName;