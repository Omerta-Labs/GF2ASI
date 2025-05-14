#pragma once

// SDK
#include "SDK/EARS_StateMachine/StateMachine.h"

// C++
#include <stdint.h>

namespace EARS
{
	namespace Framework
	{
		class SMBuilderState
		{
		public:

			void AddChild(const char* ChildName, bool bEvalParentTrans);

			void AddTransition(const char* DestStateName, uint32_t MsgID);

			void AddEnterMessage(uint32_t MsgID);

			void AddExitMessage(uint32_t MsgID);

			void AddUpdateMessage(uint32_t MsgID);
		};

		class SMBuilder
		{
		public:

			SMBuilderState* AddState(const char* StateName, int StateEnum);

			void CompileAndRegister(uint32_t ClassID, EARS::StateMachineSys::StateMachine* (__cdecl* FFn)(unsigned int, EARS::StateMachineSys::StateMachineParams*), const char* SMName);

			void Destroy();

			static SMBuilder MakeBuilder(const char* TableName, void* Allocator);

		private:

			SMBuilder() { /* nothing */ }

			char m_Padding[0x24];
		};
	}
}
