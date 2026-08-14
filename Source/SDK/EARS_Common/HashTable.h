#pragma once

// SDK
#include "SDK/EARS_Common/CommonTypes.h"

namespace EARS
{
	namespace Common
	{
		template<typename TKey, typename TValue, class TCompare = CompareFunc<TKey>, class THash = HashFunc<TKey>, class TGetKey = GetKeyFunc<TValue, TKey>, class TGetValue = HashNext<TValue>>
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

		template<typename TKey, typename TValue, int TBinCount, class TCompare = CompareFunc<TKey>, class THash = HashFunc<TKey>, class TGetKey = GetKeyFunc<TValue, TKey>, class TGetValue = HashNext<TValue>>
		struct IntrusiveHashTableFast
		{
		public:

			using THashTableType = IntrusiveHashTableFast<TKey, TValue, TBinCount, TCompare, THash, TGetKey, TGetValue>;

			IntrusiveHashTableFast()
			{
				m_NumEntries = 0;
				memset(this, 0, sizeof(TValue) * TBinCount);
			}

			uint32_t GetBin(const TKey& Key) const
			{
				return THash::Hash(Key);
			}

			uint32_t GetBin(const TValue& Value) const
			{
				const TKey& ValueKey = TGetKey::GetKey(&Value);
				return GetBin(ValueKey);
			}

			TValue* FindEntry(const TKey& Key) const
			{
				int index = GetBin(Key) & 0xFFF;
				for (auto i = m_BinArray[index]; i; i = TGetValue::GetHashNext(*i))
				{
					const TKey& ValueKey = TGetKey::GetKey(*i);
					if (TCompare::Equal(ValueKey, Key))
					{
						return i;
					}
				}

				return nullptr;
			}

			void Clear()
			{
				m_NumEntries = 0;
				memset(this, 0, sizeof(TValue) * TBinCount);
			}

			uint32_t Size() const { m_NumEntries; }

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
						while (!m_Entry && m_NextBin < TBinCount)
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

			TValue* m_BinArray[TBinCount];
			uint32_t m_NumEntries = 0;
		};

		template<typename TKey, typename TValue, size_t N, class TCompare = CompareFunc<TKey>, class THash = HashFunc<TKey>>
		struct HashTableByValue
		{
		public:

			using THashTableByValueType = HashTableByValue<TKey, TValue, N, TCompare, THash>;

			struct Entry
			{
			public:

			private:

				TKey m_Key = 0;
				TValue m_Obj = nullptr;
				Entry* m_Next = nullptr;
			
				friend HashTableByValue;
			};

			struct EntryBlock
			{
			public:

			private:

				EntryBlock* m_Next = nullptr;
				Entry m_EntryArray[N];
			};

			uint32_t GetBin(const TKey& Key) const
			{
				return THash::Hash(Key) % m_NumBins;
			}

			bool Get(const TKey& Key, TValue* OutFoundValue) const
			{
				if (Entry* FoundEntry = FindEntry(Key))
				{
					*OutFoundValue = FoundEntry->m_Obj;
					return true;
				}

				return false;
			}

			Entry* FindEntry(const TKey& KeyToFind) const
			{
				for (Entry* i = m_BinArr[GetBin(KeyToFind)]; i; i = i->m_Next)
				{
					if (TCompare::Equal(KeyToFind, i->m_Key))
					{
						return i;
					}
				}

				return nullptr;
			}

			TValue* Lookup(const TKey& KeyToFind) const
			{
				if (Entry* FoundEntry = FindEntry(KeyToFind))
				{
					return &FoundEntry->m_Obj;
				}

				return nullptr;
			}

			uint32_t Size() const { return m_NumEntries; }

			uint32_t GetNumBins() const { return m_NumBins; }

			struct Iterator
			{
			public:

				Iterator(THashTableByValueType* InHashTable)
					: m_HashTable(InHashTable)
					, m_NextBin(0)
					, m_Entry(nullptr)
				{
					Reset();
				}

				void WalkToValidEntry()
				{
					if (const THashTableByValueType* CurHashTable = m_HashTable)
					{
						while (!m_Entry && m_NextBin < CurHashTable->GetNumBins())
						{
							m_Entry = CurHashTable->m_BinArr[m_NextBin++];
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

				TKey GetKey() const { return m_Entry->m_Key; }

				TValue* GetObject() const { return &m_Entry->m_Obj; }

				// operator overloads
				Iterator& operator++(int a1)
				{
					m_Entry = m_Entry->m_Next;
					WalkToValidEntry();
					return *this;
				}

			private:

				THashTableByValueType* m_HashTable = nullptr;
				uint32_t m_NextBin = 0;
				Entry* m_Entry = nullptr;
			};

			Iterator CreateIterator()
			{
				return Iterator(this);
			}

		private:

			Entry** m_BinArr = nullptr;
			uint32_t m_NumBins = 0;
			Entry* m_FreeList = nullptr;
			uint32_t m_NumEntries = 0;
			EntryBlock* m_BlockList = nullptr;
			bool m_bGrowable = false;
		};
	}
}

#define DEFINE_MEMBER_IntrusiveHashTable(Key, Value, GetKeyFunc, GetValueFunc, MemberName) EARS::Common::IntrusiveHashTable<Key, Value, EARS::Common::CompareFunc<Key>, EARS::Common::HashFunc<Key>, GetKeyFunc, GetValueFunc> MemberName;