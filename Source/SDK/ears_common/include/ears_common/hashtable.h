#pragma once

// SDK
#include "ears_common/commontypes.h"

namespace EARS
{
	// forward declares
	namespace Framework
	{
		class FastPool;
	}

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

namespace RWS
{
	/**
	 * TODO: Find a better home for this. It won't be in EARS Common if namespace is RWS? 
	 * Intrusive hash table whose entire state is its bin array: sizeof() is
	 * exactly TBinCount pointers, with no count, capacity or allocator stored.
	 * Entries carry their own next-pointer through the TGetValue policy, so an
	 * insert allocates nothing. That is the "compact" half of the name.
	 *
	 * It never rehashes itself. When a bin's chain reaches TMaxChainLength, Add
	 * still inserts but returns NEED_TO_GROW, and the owner is expected to
	 * allocate the next size up, assign this table into it and free this one.
	 * operator=(const TNextLevel&) is that migration. TNextLevel is therefore the
	 * *smaller* table of the ladder, which is why a declaration nests inwards and
	 * bottoms out at int:
	 *
	 *   typedef IntrusiveHashTableFastCompact<uint32_t, CLinkedMsg,   4, 3, int>     TSmall;
	 *   typedef IntrusiveHashTableFastCompact<uint32_t, CLinkedMsg,  32, 3, TSmall>  TMedium;
	 *   typedef IntrusiveHashTableFastCompact<uint32_t, CLinkedMsg, 256, 5, TMedium> TLarge;
	 *
	 * TBinCount must be a power of two - GetBin masks rather than divides.
	 */
	template <typename TKey, typename TValue, int TBinCount, int TMaxChainLength, class TNextLevel,
		class TCompare = EARS::Common::CompareFunc<TKey>,
		class THash = EARS::Common::HashFunc<TKey>,
		class TGetKey = EARS::Common::GetKeyFunc<TValue, TKey>,
		class TGetValue = EARS::Common::HashNext<TValue>>
		class IntrusiveHashTableFastCompact
	{
	public:

		typedef TValue* OBJPTR;

		enum { NUMBINS = TBinCount };

		enum AddResult
		{
			ADD_OK = 0,
			NEED_TO_GROW = 1,
		};

		enum RemoveResult
		{
			REM_OK = 0,
			REM_NOT_FOUND = 1,
		};

		IntrusiveHashTableFastCompact()
		{
			for (uint32_t BinIdx = 0; BinIdx < (uint32_t)NUMBINS; BinIdx++)
			{
				m_BinArr[BinIdx] = nullptr;
			}
		}

		~IntrusiveHashTableFastCompact() {}

		/**
		 * Rehashes every entry of the smaller table into this one. This is how a
		 * promotion migrates: the owner constructs the larger table, assigns the
		 * smaller one into it, then frees the smaller one's memory.
		 *
		 * InSmaller is left pointing at entries it no longer owns - the migration
		 * does not clear it, because the caller is about to release it.
		 */
		IntrusiveHashTableFastCompact& operator=(const TNextLevel& InSmaller)
		{
			for (int BinIdx = 0; BinIdx < InSmaller.GetNumBins(); BinIdx++)
			{
				TValue* pObj = InSmaller.GetBinHead(BinIdx);
				while (pObj)
				{
					// Read the link before relinking, or the walk loses its place.
					TValue* pNext = InSmaller.GetHashNext(*pObj);

					// Prepends, where CreateEntry appends. A promotion therefore
					// reverses the relative order of entries that stay in one bin.
					const uint32_t Bin = GetBin(pObj);
					SetHashNext(*pObj, m_BinArr[Bin]);
					m_BinArr[Bin] = pObj;

					pObj = pNext;
				}
			}

			return *this;
		}

		// Every size of table draws from its own pool, so the block size is fixed at
		// pool setup and the requested size is ignored.
		void* operator new(size_t InSize)
		{
			(void)InSize;
			return m_FreeList->Alloc();
		}

		void operator delete(void* InMemory)
		{
			if (InMemory)
			{
				m_FreeList->Free(InMemory);
			}
		}

		// UNVERIFIED: not instantiated out of line in the debug build, so the body is
		// taken from the constructor, which empties the bins the same way. Nothing in
		// the game calls it. Entries are only unlinked, never destroyed.
		void Clear()
		{
			for (uint32_t BinIdx = 0; BinIdx < (uint32_t)NUMBINS; BinIdx++)
			{
				m_BinArr[BinIdx] = nullptr;
			}
		}

		// InBinIndex must be below NUMBINS; the original asserts it rather than
		// clamping, so an out-of-range bin reads past the array in a release build.
		TValue* GetBinHead(uint32_t InBinIndex) const { return m_BinArr[InBinIndex]; }

		TValue* GetHashNext(TValue& InObj) const { return TGetValue::GetHashNext(InObj); }

		void SetHashNext(TValue& InObj, TValue* InNext) const { TGetValue::SetHashNext(InObj, InNext); }

		/**
		 * Inserts InObj at the tail of its bin's chain and returns NEED_TO_GROW if
		 * that chain already held TMaxChainLength entries or more. The entry goes in
		 * either way - NEED_TO_GROW is advice to the owner, not a failure.
		 *
		 * InObj must not already be in the table. The original asserts
		 * pObj && !Lookup(pObj); without that, the chain is corrupted.
		 */
		AddResult Add(TValue* const InObj) { return CreateEntry(InObj); }

		/**
		 * Unlinks the entry holding InKey, returning REM_NOT_FOUND if there is none.
		 *
		 * MAYBE_BUG: the removed entry keeps its own hash-next pointing at its old
		 * successor. Re-adding that same object without zeroing it first splices the
		 * rest of the old chain in behind it, so the bin then reaches entries twice.
		 * The game gets away with it because CLinkedMsg entries come from a pool and
		 * are zeroed on allocation, never removed and re-added in place.
		 */
		RemoveResult Remove(const TKey& InKey)
		{
			RemoveResult Result = REM_NOT_FOUND;

			const uint32_t Bin = GetBin(InKey);
			TValue* pObj = m_BinArr[Bin];
			TValue* pPrev = nullptr;

			while (pObj)
			{
				const TKey EntryKey = TGetKey::GetKey(*pObj);
				if (TCompare::Equal(EntryKey, InKey))
				{
					if (pPrev)
					{
						SetHashNext(*pPrev, GetHashNext(*pObj));
					}
					else
					{
						m_BinArr[Bin] = GetHashNext(*pObj);
					}

					Result = REM_OK;
					break;
				}

				pPrev = pObj;
				pObj = GetHashNext(*pObj);
			}

			return Result;
		}

		// UNVERIFIED: declared in the original but never instantiated; keyed removal
		// above is what the game calls.
		RemoveResult Remove(TValue* const InObj)
		{
			const TKey Key = TGetKey::GetKey(*InObj);
			return Remove(Key);
		}

		// Returns nullptr when InKey is not present. The result is the caller's own
		// entry, not a copy.
		TValue* Lookup(const TKey& InKey) const { return FindEntry(InKey); }

		// Looks up by the key InObj carries, so it answers "is this object, or another
		// one sharing its key, in the table".
		TValue* Lookup(TValue* const InObj) const
		{
			const TKey Key = TGetKey::GetKey(*InObj);
			return FindEntry(Key);
		}

		// UNVERIFIED: never instantiated. The signature is the original's.
		bool Get(const TKey& InKey, TValue*& OutObj) const
		{
			OutObj = FindEntry(InKey);
			return (OutObj != nullptr);
		}

		// UNVERIFIED: never instantiated. The signature is the original's.
		TValue* operator[](const TKey& InKey) const { return FindEntry(InKey); }

		bool Contains(const TKey& InKey) const { return (FindEntry(InKey) != nullptr); }

		int GetNumBins() const { return NUMBINS; }

		// UNVERIFIED: never instantiated, and the name is all the evidence there is
		// for what the three out-parameters mean. Only their count, order and type
		// are recovered; treat the meanings chosen here as a guess.
		void GetStatistics(uint32_t& OutNumUsedBins, uint32_t& OutNumEntries, uint32_t& OutLongestChain) const
		{
			OutNumUsedBins = 0;
			OutNumEntries = 0;
			OutLongestChain = 0;

			for (uint32_t BinIdx = 0; BinIdx < (uint32_t)NUMBINS; BinIdx++)
			{
				uint32_t ChainLength = 0;
				for (TValue* pObj = m_BinArr[BinIdx]; pObj; pObj = GetHashNext(*pObj))
				{
					ChainLength++;
				}

				if (ChainLength)
				{
					OutNumUsedBins++;
					OutNumEntries += ChainLength;

					if (ChainLength > OutLongestChain)
					{
						OutLongestChain = ChainLength;
					}
				}
			}
		}

		/**
		 * Walks every entry of every bin, in bin order. Entries within one bin come
		 * out in chain order, which is insertion order until a promotion reverses it.
		 *
		 * TODO: the original also declares three more constructors, operator-- (pre
		 * and post), a post-increment operator++ and operator=, none of which were
		 * instantiated. operator-- cannot be recovered by inspection: the bin chains
		 * are singly linked, so whatever the original did to step backwards is not
		 * visible from the forward path.
		 */
		class Iterator
		{
		public:

			Iterator()
				: m_HashTable(nullptr)
				, m_NextBin(0)
				, m_Entry(nullptr)
			{
			}

			// UNVERIFIED: one of the original's four constructors took the table, but
			// only the default was instantiated, so this forwarding is inferred.
			Iterator(const IntrusiveHashTableFastCompact& InTable)
				: m_HashTable(nullptr)
				, m_NextBin(0)
				, m_Entry(nullptr)
			{
				Reset(InTable);
			}

			// UNVERIFIED: never instantiated.
			const IntrusiveHashTableFastCompact* GetHashTable() const { return m_HashTable; }

			bool IsFinished() const { return (m_Entry == nullptr); }

			// UNVERIFIED: never instantiated.
			TKey GetKey() const { return TGetKey::GetKey(*m_Entry); }

			// The original asserts m_Entry, so this is only valid while !IsFinished().
			TValue* GetObjectPtr() const { return m_Entry; }

			// UNVERIFIED: never instantiated.
			TValue* operator->() const { return m_Entry; }

			// UNVERIFIED: never instantiated.
			TValue& operator*() const { return *m_Entry; }

			// UNVERIFIED: never instantiated.
			bool operator==(const Iterator& InOther) const { return (m_Entry == InOther.m_Entry); }

			// UNVERIFIED: never instantiated.
			bool operator!=(const Iterator& InOther) const { return (m_Entry != InOther.m_Entry); }

			Iterator& operator++()
			{
				m_Entry = TGetValue::GetHashNext(*m_Entry);
				WalkToValidEntry();
				return *this;
			}

			void Reset(const IntrusiveHashTableFastCompact& InTable)
			{
				m_HashTable = &InTable;
				Reset();
			}

			void Reset()
			{
				m_NextBin = 0;
				m_Entry = nullptr;
				WalkToValidEntry();
			}

		protected:

			// Advances past empty bins until m_Entry holds something or the bins run
			// out. m_NextBin is left one past the bin m_Entry came from, so stepping
			// off the end of a chain resumes in the right place.
			void WalkToValidEntry()
			{
				if (m_HashTable)
				{
					while (!m_Entry && m_NextBin < (uint32_t)NUMBINS)
					{
						m_Entry = m_HashTable->m_BinArr[m_NextBin];
						m_NextBin++;
					}
				}
			}

			const IntrusiveHashTableFastCompact* m_HashTable;
			uint32_t m_NextBin;
			TValue* m_Entry;
		};

	private:

		AddResult CreateEntry(TValue* const InObj) { return CreateEntry(InObj, GetBin(InObj)); }

		AddResult CreateEntry(TValue* const InObj, uint32_t InBin)
		{
			AddResult Result = ADD_OK;

			int ChainLength = 0;
			TValue* pLast = nullptr;
			for (TValue* pObj = m_BinArr[InBin]; pObj; pObj = GetHashNext(*pObj))
			{
				ChainLength++;
				pLast = pObj;
			}

			if (pLast)
			{
				SetHashNext(*pLast, InObj);
			}
			else
			{
				m_BinArr[InBin] = InObj;
			}

			// The length measured is the chain as it was before InObj joined it.
			if (ChainLength >= TMaxChainLength)
			{
				Result = NEED_TO_GROW;
			}

			return Result;
		}

		// Returns the entry holding InKey, or nullptr. Lookup, Get, operator[] and
		// Contains are all this function.
		TValue* FindEntry(const TKey& InKey) const
		{
			const uint32_t Bin = GetBin(InKey);

			TValue* pObj = m_BinArr[Bin];
			for (; pObj; pObj = GetHashNext(*pObj))
			{
				const TKey EntryKey = TGetKey::GetKey(*pObj);
				if (TCompare::Equal(EntryKey, InKey))
				{
					break;
				}
			}

			return pObj;
		}

		uint32_t GetBin(TValue* const InObj) const
		{
			const TKey Key = TGetKey::GetKey(*InObj);
			return GetBin(Key);
		}

		uint32_t GetBin(const TKey& InKey) const
		{
			// NUMBINS is a power of two, so the wrap is a mask and not a divide.
			return (THash::Hash(InKey) & (NUMBINS - 1));
		}

		// Declared and never defined: a table owns the links inside its entries, so
		// copying one is a link error rather than two tables sharing chains.
		IntrusiveHashTableFastCompact(const IntrusiveHashTableFastCompact& InOther);
		IntrusiveHashTableFastCompact& operator=(const IntrusiveHashTableFastCompact& InOther);

		OBJPTR m_BinArr[NUMBINS];

	public:

		// One pool per instantiation, installed during start-up. The original asserts
		// it is non-null on every allocation rather than falling back to the heap.
		static EARS::Framework::FastPool* m_FreeList;
	};

	template <typename TKey, typename TValue, int TBinCount, int TMaxChainLength, class TNextLevel,
		class TCompare, class THash, class TGetKey, class TGetValue>
	EARS::Framework::FastPool* IntrusiveHashTableFastCompact<TKey, TValue, TBinCount, TMaxChainLength,
		TNextLevel, TCompare, THash, TGetKey, TGetValue>::m_FreeList = nullptr;

}
