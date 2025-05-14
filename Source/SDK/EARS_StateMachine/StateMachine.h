#pragma once

// C++
#include <stdint.h>

namespace EARS
{
	namespace StateMachineSys
	{
		// forward declare
		struct StateMachineTree;

		struct TransitionList
		{
		public:

			uint32_t m_Count = 0;
			uint32_t m_StateTable[32];
			uint32_t m_Transitions[32];
			uint32_t m_EvalTable[32];
			bool m_bUsed[32];
			bool m_bIsMaster = false;
			bool m_bIsActive = false;
		};

		struct StateMachineParams
		{
			TransitionList* m_Transitions = nullptr;
			uint32_t m_ClassID = 0;
		};

		struct State
		{
		public:

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

		private:

			void** m_Transitions = nullptr;
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

		private:

			State** m_States = nullptr;
			uint32_t m_ID = 0;
			uint32_t m_SMClassID = 0;
			const char* m_Name = nullptr;
			uint16_t m_Flags = 0;
			uint16_t m_RefCount = 0;
		};

		class StateMachine
		{
		public:

			StateMachine() = delete;
			StateMachine(unsigned int TableID, EARS::StateMachineSys::StateMachineParams* SmParams);

			virtual uint32_t GetStateMachineID() const = 0;

			virtual ~StateMachine();

			virtual bool HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, State::StateMessageData* MsgData);
			virtual bool CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, void* TransData);
			virtual void InitialiseChild(StateMachine* ChildMachine);
			virtual void Update(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t& TerminateLevel, uint32_t EvalLevel);
			virtual void AddChild(uint32_t SimTime, float FrameTime, EARS::StateMachineSys::StateMachine* Child);
			virtual void RemoveChild(StateMachine* ChildMachine);
			virtual void SpawnChild(uint32_t SimTime, float FrameTime, uint32_t TableID);
			virtual void ReadInitDataFromSnapShot(void* SnapshotData);
			virtual void WriteInitDataToSnapShot(void* SnapshotData);

			// getters
			StateMachine* GetParent() const { return m_Parent; }

			// operator overloads
			void* operator new(size_t size);
			void operator delete(void* p);

		protected:

			/* assign external transitions into state machine */
			void SetExternalTransitions(TransitionList* ExternalTransitions);

		private:

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

			// getters
			EARS::StateMachineSys::StateMachine* GetRoot() const { return m_Root; }

		private:

			uint32_t m_EvalFlags = 0;
			EARS::StateMachineSys::StateMachine* m_Root = nullptr;
		};
	}
}