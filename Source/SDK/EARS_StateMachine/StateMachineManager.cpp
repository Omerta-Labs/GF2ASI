#include "StateMachineManager.h"

#include "Addons/Hook.h"

#include "SDK/EARS_StateMachine/StateMachine.h"

// C++
#include <assert.h>

namespace EARS
{
	namespace StateMachineSys
	{
		StateTable* StateMachineManager::GetStateTableFromID(uint32_t InTableID) const
		{
			return MemUtils::CallClassMethod<StateTable*, const StateMachineManager*, uint32_t>(0x0621530, this, InTableID);
		}

		StateMachine* StateMachineManager::CreateStateMachineFromTableID(uint32_t StateTableID, StateMachineParams* Params)
		{
			StateTable* SMTable = GetStateTableFromID(StateTableID);
			assert(SMTable);

			return CreateStateMachine(SMTable->GetSMClassID(), StateTableID, Params);
		}

		StateMachine* StateMachineManager::CreateStateMachine(uint32_t InSMTableID, uint32_t StateTableID, StateMachineParams* Params)
		{
			FactoryFunc Factory = GetFactoryFromID(InSMTableID);
			assert(Factory);

			StateMachine* NewSM = Factory(StateTableID, Params);
			assert(NewSM);

			NewSM->SetAllocNumber(m_AllocationNumber++);
			return NewSM;
		}

		//
		// GetFactoryFromID — naked __thiscall thunk for the game's __userpurge function.
		//
		// Original PC code at 0x621610 is __userpurge(this@<eax>).  This thunk is
		// identical to the original except 'this' is sourced from ECX (__thiscall)
		// instead of EAX, so it can be called normally from C++.
		//
		// Internally calls sub_621D50 (std::map::find) at its fixed game address.
		// sub_621D50 calling convention:  eax = output buffer ptr, esi = tree header, stack = &key.
		//
		static const uint32_t kAddr_MapFind = 0x621D50;

		__declspec(naked) StateMachineManager::FactoryFunc StateMachineManager::GetFactoryFromID(uint32_t /*ClassID*/) const
		{
			__asm
			{
				push    ebp
				mov     ebp, esp
				and esp, 0FFFFFFF8h
				sub     esp, 0Ch
				push    ebx
				push    esi
				push    edi

				// Original: mov edi, eax  —  changed to ecx for __thiscall
				mov     edi, ecx

				mov     esi, [edi + 0Ch]          // esi = this->m_FactoryMap (tree header)
				lea     eax, [ebp + 8]            // eax = &ClassID (first stack arg)
				push    eax                     // push &key
				lea     eax, [esp + 1Ch - 8]        // eax = &output buffer (var_8)
				call    dword ptr[kAddr_MapFind]

				mov     esi, [esp + 18h - 8]        // esi = found node
				test    esi, esi
				mov     eax, [edi + 0Ch]          // eax = tree header
				mov     edi, [eax + 1Ch]          // edi = sentinel / size
				jz      short _not_found
				cmp     esi, eax
				jz      short _check

				_not_found :
					pop     edi
					pop     esi
					xor eax, eax                // return nullptr
					pop     ebx
					mov     esp, ebp
					pop     ebp
					ret     4

				_check:
					mov     ebx, [esp + 18h - 4]        // ebx = iterator node
					cmp     ebx, edi
					jz      short _not_found
					test    esi, esi
					jz      short _not_found
					cmp     ebx, [esi + 1Ch]
					jz      short _not_found

					mov     eax, [ebx + 10h]          // eax = factory function pointer
					pop     edi
					pop     esi
					pop     ebx
					mov     esp, ebp
					pop     ebp
					ret     4
			}
		}

		StateMachineManager* StateMachineManager::GetInstance()
		{
			return *(StateMachineManager**)0x12234AC;
		}
	} // StateMachineSys
} // EARS