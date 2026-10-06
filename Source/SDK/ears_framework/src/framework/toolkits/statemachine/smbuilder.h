#pragma once

// SDK
#include "ears_common/managedarray.h"
#include "ears_statemachine/statemachine.h"

// C++
#include <stdint.h>

// forward declares
namespace EA::Allocator
{
	class IAllocator;
}

namespace EARS
{
	namespace Framework
	{
		/**
		 * One state under construction inside an SMBuilder.
		 *
		 * Instances are owned by the SMBuilder that produced them (see AddState); they are
		 * allocated out of the builder's allocator and destroyed by ~SMBuilder.
		 *
		 * Only the vptr and the overall size are modelled here.  The engine's layout is six
		 * ManagedArrays (transitions, children, enter/update/exit messages, child IDs) followed
		 * by the state name at +0x84; none of it is referenced from reconstructed code.
		 */
		class SMBuilderState
		{
		public:

			//~ Begin virtual interface
			/**
			 * Slot 0.  Engine-side destructor; the engine owns every live SMBuilderState, so a
			 * destroy through a base pointer always dispatches into the engine's own vtable and
			 * this body is never the one that runs.  It exists so the slot is declared.
			 */
			virtual ~SMBuilderState();

			/** Slot 1.  Bytes this state will occupy once compiled. */
			virtual uint32_t GetSize() const;

			/** Slot 2.  Emit the compiled state into InBuffer and report how much was written. */
			virtual EARS::StateMachineSys::State* Compile(void* InBuffer, uint32_t& OutBytesWritten) const;
			//~ End virtual interface

			void AddChild(const char* ChildName, bool bEvalParentTrans);

			void AddTransition(const char* DestStateName, uint32_t MsgID);

			/** Transition carrying an arbitrary payload; TypeID is an EARS::StateMachineSys::StateMachineTypeID. */
			void AddTransition(const char* DestStateName, uint32_t MsgID, const void* InData, uint32_t TypeID, uint32_t DataSize, uint32_t Alignment);

			/** Transition carrying a uint32 payload (TransitionData::m_IntegerVal). */
			void AddIntTransition(const char* DestStateName, uint32_t MsgID, uint32_t Data, uint32_t Alignment);

			/** Transition carrying a float payload (TransitionData::m_FloatVal). */
			void AddFloatTransition(const char* DestStateName, uint32_t MsgID, float Data, uint32_t Alignment);

			/** Shorthand for an enter message that plays AnimID through AnimateStateMachine. */
			void AddEnterAnim(int AnimID, bool bBlend, bool bForceAnim, bool bIgnoreGameMovementBlend, float FrameRateScale);

			void AddEnterMessage(uint32_t MsgID);

			void AddExitMessage(uint32_t MsgID);

			void AddUpdateMessage(uint32_t MsgID);

			// Payload-carrying forms of the three above.  TypeID is an
			// EARS::StateMachineSys::StateMachineTypeID; the Int/Float wrappers below pass
			// c_SMIntDataID / c_SMFloatDataID for you.
			void AddEnterMessage(uint32_t MsgID, uint32_t TypeID, uint32_t DataSize, const void* InData, uint32_t Alignment);
			void AddUpdateMessage(uint32_t MsgID, uint32_t TypeID, uint32_t DataSize, const void* InData, uint32_t Alignment);
			void AddExitMessage(uint32_t MsgID, uint32_t TypeID, uint32_t DataSize, const void* InData, uint32_t Alignment);

			void AddIntEnterMessage(uint32_t MsgID, uint32_t Data, uint32_t Alignment);
			void AddIntUpdateMessage(uint32_t MsgID, uint32_t Data, uint32_t Alignment);
			void AddIntExitMessage(uint32_t MsgID, uint32_t Data, uint32_t Alignment);

			void AddFloatEnterMessage(uint32_t MsgID, float Data, uint32_t Alignment);
			void AddFloatUpdateMessage(uint32_t MsgID, float Data, uint32_t Alignment);
			void AddFloatExitMessage(uint32_t MsgID, float Data, uint32_t Alignment);

			// TODO: AddScript{Transition,Enter,Update,Exit}Message also exist, but nothing in the
			// PC build calls them and their four candidate bodies are indistinguishable by shape,
			// so which is which has not been established.

		private:

			char m_Padding[0x84];
		};

		static_assert(sizeof(SMBuilderState) == 0x88);

		/**
		 * Builds a state table at runtime and registers it with the StateMachineManager.
		 *
		 * Every EARS state machine owns a static build function that constructs one of these on the
		 * stack, describes its states, then calls CompileAndRegister.  The builder's destructor
		 * releases the states and the allocator, so it must be allowed to run.
		 */
		class SMBuilder
		{
		public:

			SMBuilder() = delete;
			SMBuilder(const char* InTableName, EA::Allocator::IAllocator* InAllocator);
			~SMBuilder();

			SMBuilderState* AddState(const char* StateName, int StateEnum);

			/** Bytes the whole compiled table will occupy. */
			uint32_t GetSize() const;

			/** Compile the table into a caller-supplied buffer of GetSize() bytes. */
			EARS::StateMachineSys::State** Compile(void* InBuffer);

			void CompileAndRegister(uint32_t ClassID, EARS::StateMachineSys::StateMachine* (__cdecl* FFn)(unsigned int, EARS::StateMachineSys::StateMachineParams*), const char* SMName);

		private:

			EA::Allocator::IAllocator* m_Allocator = nullptr;
			ManagedArray<EARS::Framework::SMBuilderState*> m_States;
			const char* m_Name = nullptr;
			uint32_t m_StateMachineClassID = 0;
			uint32_t m_StateTableID = 0;
		};

		static_assert(sizeof(SMBuilder) == 0x24);
	}
}
