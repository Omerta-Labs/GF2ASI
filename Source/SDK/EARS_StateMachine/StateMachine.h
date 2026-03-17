#pragma once

// C++
#include <stdint.h>

namespace EARS
{
	namespace StateMachineSys
	{
		// forward declare
		struct StateMachineTree;

		enum StateMachineTypeID
		{
			c_SMIntDataID = 0x3AFD3141,
			c_SMFloatDataID = 0x3E55DDF1,
		};

		struct TransitionList
		{
		public:

			TransitionList();

			bool IsActive() const { return m_bIsActive; }
			bool IsMaster() const { return m_bIsMaster; }

			void Add(uint32_t stateTableID, uint32_t transID, uint32_t evalLevel);
			
			bool Remove(uint32_t StateTableID, uint32_t TransID, uint32_t EvalLevel);

		private:

			uint32_t m_Count = 0;
			uint32_t m_StateTable[32];
			uint32_t m_Transitions[32];
			uint32_t m_EvalTable[32];
			bool m_bUsed[32];
			bool m_bIsMaster = false;
			bool m_bIsActive = false;
		};

		// Transition entry layout in State::m_Transitions (null-terminated void* array):
		//   +0x0: uint16 m_TargetStateIdx   — new state index on transition
		//   +0x2: uint16  m_Flags            — bit 0: use CheckTransition; clear: use CurFlags mask
		//   +0x4: uint32 m_TransIDOrFlags   — TransID (Flags&1) or required CurFlags mask (~Flags&1)
		//   +0x8: void*  m_TransData        — optional data passed to CheckTransition (Flags&1 only)

		struct Transition
		{
			enum StateMachineTransID : uint32_t
			{
				TRANSID_NONE = 0x0,
				TRANSID_NOCHILDREN = 0x1,
				TRANSID_TIMEOUT = 0x2,
				TRANSID_NEXTFRAME = 0x3,
				TRANSID_DEFAULT = 0x4,
				TRANSID_LAST = 0x5,
			};

			struct TransitionData
			{
				StateMachineTypeID m_TypeID;
				
				struct DataUnion
				{
					uint32_t m_IntegerVal = 0;
					float m_FloatVal = 0.0f;;
					void* m_PointerVal = nullptr;
				} m_Data;
			};

			uint16_t m_TargetStateIdx = 0;
			uint16_t  m_Flags = 0;
			uint32_t m_TransIDOrFlags;
			TransitionData* m_TransData = nullptr;
		};

		struct StateMachineParams
		{
			TransitionList* m_Transitions = nullptr;
			uint32_t m_ClassID = 0;
		};

		struct State
		{
		public:

			enum StateMessageID : uint32_t
			{
				MESSAGE_NONE = 0,
				MESSAGE_ADD_CHILDREN = 1,
				MESSAGE_TERMINATE = 2,
				MESSAGE_LAST
			};

			const struct StateMessageData
			{
				uint32_t m_TypeID = 0;

				union DataUnion
				{
					uint32_t m_IntVal;
					float m_FloatVal;
					void* m_PointerVal;
				} m_Data;
			};

			struct StateMessage
			{
				uint32_t m_MessageID = 0;
				StateMessageData* m_StateMessageData = nullptr;
			};

			Transition** m_Transitions = nullptr;
			const char* m_Name = nullptr;
			StateMessage** m_EnterMessages = nullptr;
			StateMessage** m_UpdateMessages = nullptr;
			StateMessage** m_ExitMessages = nullptr;
		};

		struct AddChildrenMessageData
		{
		public:

			uint16_t m_Flags = 0;
			uint16_t m_NumStateMachines = 0;
			uint32_t* m_StateMachineIDs = nullptr;
		};

		struct StateTable
		{
		public:

			bool IsReferenced() const { return m_RefCount != 0; }
			void IncRefCount() { m_RefCount++; }
			void DecRefCount() { m_RefCount--; }
			void ClearRefCount() { m_RefCount = 0; }
			State** GetStates() const { return m_States; }
			const char* GetName() const { return m_Name; }

		private:

			State** m_States = nullptr;
			uint32_t m_ID = 0;
			uint32_t m_SMClassID = 0;
			const char* m_Name = nullptr;
			uint16_t m_Flags = 0;
			uint16_t m_RefCount = 0;
		};

		// Flat snapshot node — 0x10 bytes per StateMachine in a serialised tree walk.
		// ReadInitDataFromSnapShot / WriteInitDataToSnapshot advance through an array
		// of these, returning a pointer to the next node (pSnap + 1).
		struct StateMachineSnapshot
		{
			uint32_t m_StateIdx   = 0;    // 0x00 — current state index
			uint32_t m_TableID    = 0;    // 0x04 — state table ID
			uint32_t m_EvalFlags  = 0;    // 0x08 — eval flags
			uint16_t m_NumChildren = 0;   // 0x0C — child count
			uint16_t m_Pad        = 0;    // 0x0E — cleared on write
		};
		static_assert(sizeof(StateMachineSnapshot) == 0x10, "StateMachineSnapshot must be 0x10 bytes");

		class StateMachine
		{
		public:

			// Linked-list view over a StateMachine's direct children.
			// Iterates via m_Next sibling pointers; end sentinel is nullptr.
			class ChildList
			{
			public:
				struct const_iterator
				{
					StateMachine* m_pCurrent;

					explicit const_iterator(StateMachine* p) : m_pCurrent(p) {}

					const_iterator& operator++()
					{
						m_pCurrent = m_pCurrent->m_Next;
						return *this;
					}

					StateMachine* operator*() const { return m_pCurrent; }

					bool operator!=(const const_iterator& other) const
					{
						return m_pCurrent != other.m_pCurrent;
					}
				};

				// iterator is identical to const_iterator for this list
				using iterator = const_iterator;

				explicit ChildList(const StateMachine* pParent) : m_pParent(pParent) {}

				iterator begin() const { return iterator(m_pParent->m_ChildHead); }
				iterator end()   const { return iterator(nullptr); }

			private:
				const StateMachine* m_pParent;
			};

			StateMachine() = delete;
			StateMachine(unsigned int TableID, EARS::StateMachineSys::StateMachineParams* SmParams);

			virtual uint32_t GetStateMachineID() const = 0;

			virtual ~StateMachine();

			virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, State::StateMessageData* MsgData);
			virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, Transition::TransitionData* TransData);
			virtual void InitialiseChild(StateMachine* ChildMachine);
			virtual StateMachine* Update(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t& TerminateLevel, uint32_t EvalLevel);
			virtual void AddChild(uint32_t SimTime, float FrameTime, EARS::StateMachineSys::StateMachine* Child);
			virtual void RemoveChild(StateMachine* ChildMachine);
			virtual void SpawnChild(uint32_t SimTime, float FrameTime, uint32_t TableID);
			virtual const StateMachineSnapshot* ReadInitDataFromSnapShot(const StateMachineSnapshot* pSnap);
			virtual StateMachineSnapshot* WriteInitDataToSnapShot(StateMachineSnapshot* pSnap);

			// source: c:\packages\ears_statemachine\dev\src\statemachine.cpp
			unsigned int PrintStateMachine(char* buf, unsigned int bufSize, unsigned int level) const;

			// getters
			StateMachine*      GetParent()          const { return m_Parent; }
			StateMachine*      GetChildHead()       const { return m_ChildHead; }
			StateMachine*      GetNextSibling()     const { return m_Next; }
			StateMachineTree*  GetTree()            const { return m_Tree; }
			uint32_t           GetTableID()         const { return m_StateTableID; }
			float              GetTimeInCurState()  const { return m_TimeInCurrentState; }
			const char*        GetCurrentStateName() const;
			const char*        GetTableName()        const;
			ChildList          GetChildList()        const { return ChildList(this); }

			// evaluators
			bool IsBusy() const { return m_EvalFlags & 1; }

			// operator overloads
			void* operator new(size_t size);
			void operator delete(void* p);

		protected:

			/* assign external transitions into state machine */
			void SetExternalTransitions(TransitionList* ExternalTransitions);

		private:

			/** original function: 0x0624820 */
			void InitializeState(uint32_t simTime, float frameTime);

			/** original function but inline on PC */
			// Returns true when this SM has at least one child (m_ChildHead != nullptr).
			bool HasChildren() const { return m_ChildHead != nullptr; }

			/** original function but inline on PC */
			void SetEvaluateTransitions(bool bCheck);

			// source: sub_624D20 — PC address 0x624D20
			// Iteratively removes and destroys all direct children.
			// Recurses depth-first: each child's subtree is cleared before it is removed from this.
			void TerminateChildren();

			/** original function but inline on PC */
			void Destroy();

			TransitionList* m_ExternalTransitions = nullptr;
			uint32_t m_StateTableID = 0;
			uint32_t m_CurStateIdx = 0;
			EARS::StateMachineSys::State** m_States = nullptr;
			uint32_t m_EvalFlags = 0;
			EARS::StateMachineSys::StateMachineTree* m_Tree = nullptr;
			EARS::StateMachineSys::StateMachine* m_Parent = nullptr;
			EARS::StateMachineSys::StateMachine* m_ChildHead = nullptr;
			EARS::StateMachineSys::StateMachine* m_ChildTail = nullptr;
			EARS::StateMachineSys::StateMachine* m_Next = nullptr;
			uint32_t m_AllocNumber = 0;
			float m_TimeInCurrentState = 0.0f;
			uint32_t m_Padding = 0;
		};

		struct StateMachineTree
		{
		public:

			// source: ?Remove@StateMachineTree@StateMachineSys@EARS@@QAAXPAVStateMachine@23@@Z
			// PC address: 0x624F30
			void Remove(StateMachine* pSM);

			// getters
			EARS::StateMachineSys::StateMachine* GetRoot() const { return m_Root; }

		private:

			uint32_t m_EvalFlags = 0;
			EARS::StateMachineSys::StateMachine* m_Root = nullptr;
		};
	}
}