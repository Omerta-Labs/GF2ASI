#include "PlayerDebugFlySM.h"

// SDK
#include "SDK/EARS_Framework/Core/Camera/CameraManager.h"
#include "SDK/EARS_Framework/Core/Input/Input.h"
#include "SDK/EARS_Framework/Toolkits/StateMachine/SMBuilder.h"
#include "SDK/EARS_Godfather/Modules/Player/Player.h"
#include "SDK/EARS_Godfather/Modules/Player/PlayerDebug.h"

// Addons
#include "Addons/Hook.h"

// C++
#include <cmath>

namespace Private
{
	EARS::StateMachineSys::StateMachine* S_PlayerDebugFlySM_FactoryFn(unsigned int id, EARS::StateMachineSys::StateMachineParams* pSMParams)
	{
		return new EARS::Modules::PlayerDebugFlySM(id, pSMParams);
	}
}

namespace EARS
{
	namespace Modules
	{
		EARS::Modules::PlayerDebugFlySM::PlayerDebugFlySM(unsigned int TableID, EARS::StateMachineSys::StateMachineParams* SmParams)
			: EARS::Modules::PlayerSM(TableID, SmParams)
		{
			if (Player* CurPlayer = GetPlayer())
			{
				// first disable collisions with everything on the Player assigned to this SM
				EARS::Havok::CharacterProxy& CharProxy = CurPlayer->GetCharacterProxyChecked();
				CharProxy.SetCollisionState(Havok::CharacterProxy::CollisionState::CS_TERRAIN_DISABLED);

				// TODO: Consider doing the same for car they are in
			}

			// TODO: Consider doing for all their crew
		}

		PlayerDebugFlySM::~PlayerDebugFlySM()
		{
			if (Player* CurPlayer = GetPlayer())
			{
				// first disable collisions with everything on the Player assigned to this SM
				EARS::Havok::CharacterProxy& CharProxy = CurPlayer->GetCharacterProxyChecked();
				CharProxy.SetCollisionState(Havok::CharacterProxy::CollisionState::CS_ENABLED);
			}

			// TODO: Do the same for the rest of the function, copy xbox version
		}

		uint32_t EARS::Modules::PlayerDebugFlySM::GetStateMachineID() const
		{
			return 0x29CC4DD4;
		}

		bool EARS::Modules::PlayerDebugFlySM::HandleStateMessage(uint32_t SimTime, float FrameTime, uint32_t CurFlags, uint32_t MessageID, EARS::StateMachineSys::State::StateMessageData* MsgData)
		{
			switch (MessageID)
			{
			case 0x31: // MESSAGE_PROCESSMOVEMENT
			{
				ProcessMovement(FrameTime);
				break;
			}
			case 0x32: // MESSAGE_PUSHCAMERA
			{
				break;
			}
			case 0x33: // MESSAGE_POPCAMERA
			{
				break;
			}
			case 0x34: // MESSAGE_EXIT_VEHICLE_IF_NEEDED
			{
				break;
			}
			default:
			{
				return EARS::Modules::PlayerSM::HandleStateMessage(SimTime, FrameTime, CurFlags, MessageID, MsgData);
			}
			}

			return true;
		}

		bool EARS::Modules::PlayerDebugFlySM::CheckTransition(uint32_t SimTime, float FrameTime, uint32_t TransID, void* TransData)
		{
			if (TransID == 17) // TRANSID_STOPDEBUGFLY
			{
				// should we enter fly mode
				EARS::Modules::PlayerDebugOptions& DebugOptions = *EARS::Modules::PlayerDebugOptions::GetInstance();
				return (DebugOptions.IsInDebugFly() == false);
			}

			// call parent class
			return EARS::Modules::PlayerSM::CheckTransition(SimTime, FrameTime, TransID, TransData);
		}

		void PlayerDebugFlySM::ProcessMovement(float FrameTime)
		{
			// get dependencies
			using namespace EARS::Framework;
			EARS::Framework::InputDeviceManager* InputMgr = EARS::Framework::InputDeviceManager::GetInstance();

			Player* CurPlayer = GetPlayer();
			const uint32_t ControllerID = CurPlayer->GetControllerID();

			constexpr float v17 = 1.0f;

			if (InputMgr->CheckButtonsANY(ControllerID, ButtonMask::CTRL_BUTTON_RIGHT_BOTTOM, ButtonStatus::BUTTON_STATUS_DOWN))
			{
				// apply something
				int z = 0;
			}

			if (InputMgr->CheckButtonsANY(ControllerID, ButtonMask::CTRL_BUTTON_RIGHT_TOP, ButtonStatus::BUTTON_STATUS_DOWN))
			{
				// apply something
				int z = 0;
			}

			if (InputMgr->CheckButtonsANY(ControllerID, ButtonMask::CTRL_BUTTON_LEFT_BOTTOM, ButtonStatus::BUTTON_STATUS_DOWN))
			{
				// apply something
				int z = 0;
			}

			// TODO: Check vehicle
			RwV3d WorkingLocation = CurPlayer->GetPosition();

			constexpr float MULTIPLIER_Y = 0.2f;
			if (InputMgr->CheckButtonsANY(ControllerID, ButtonMask::CTRL_BUTTON_RIGHT_DPAD_UP, ButtonStatus::BUTTON_STATUS_DOWN))
			{
				// make character go up
				WorkingLocation.m_Y = WorkingLocation.m_Y + (v17 * MULTIPLIER_Y);
			}

			if (InputMgr->CheckButtonsANY(ControllerID, ButtonMask::CTRL_BUTTON_RIGHT_DPAD_DOWN, ButtonStatus::BUTTON_STATUS_DOWN))
			{
				// make character go down
				WorkingLocation.m_Y = WorkingLocation.m_Y - (v17 * MULTIPLIER_Y);
			}

			const Controller_Info* CurController = InputMgr->GetControllerInfo(ControllerID);
			float LeftStickX = CurController->m_LeftStick.m_X;
			float LeftStickY = CurController->m_LeftStick.m_Y;

			// if below the stick threshold then we do not move the character
			constexpr float STICK_THRESEHOLD = 0.25f;
			if (fabs(LeftStickX) <= STICK_THRESEHOLD)
			{
				LeftStickX = 0.0f;
			}

			if (fabs(LeftStickY) <= STICK_THRESEHOLD)
			{
				LeftStickY = 0.0f;
			}

			RwV3d CameraRot;
			const CameraManager* CameraMgr = CameraManager::GetInstance();
			CameraMgr->GetActiveCamRot(CameraRot, CurPlayer->GetPlayerID());

			// NB: Uses XMScalarSinCos in XBOX
			const float SinY = std::sin(CameraRot.m_Y);
			const float CosY = std::cos(CameraRot.m_Y);

			WorkingLocation.m_X = WorkingLocation.m_X - (((CosY * LeftStickX) * v17) * 0.5f);
			WorkingLocation.m_Z = WorkingLocation.m_Z + (((SinY * LeftStickX) * v17) * 0.5f);
			WorkingLocation.m_X = WorkingLocation.m_X - (((SinY * LeftStickY) * v17) * 0.5f);
			WorkingLocation.m_Z = WorkingLocation.m_Z - (((CosY * LeftStickY) * v17) * 0.5f);

			// TODO: GetVehicle
			CurPlayer->SetPosition(WorkingLocation);
			
			// then figure out heading
			const RwV3d NewHeading = RwV3d(SinY, 0.0f, CosY) + WorkingLocation;
			CurPlayer->SetHeadingToPosition(NewHeading, nullptr);

			// TODO: Grabbed Sentient
			// TODO: CrewLeader Fly Update
		}

		/* static */
		void EARS::Modules::PlayerDebugFlySM::BuildStateMachine()
		{
			// TODO: Check whether the state machine has already been built!
			void* thread_new_allocator = MemUtils::CallCdeclMethod<void*>(0x09C8F80);
			EARS::Framework::SMBuilder Builder = EARS::Framework::SMBuilder::MakeBuilder("playerDebugFlyStateTable", thread_new_allocator);

			// IDLE STATE
			EARS::Framework::SMBuilderState* IdleState = Builder.AddState("idle", -1);
			IdleState->AddEnterMessage(0x32);
			IdleState->AddEnterMessage(0x34);
			IdleState->AddUpdateMessage(0x31);
			IdleState->AddTransition("exit", 0x11);

			// EXIT STATE
			EARS::Framework::SMBuilderState* ExitState = Builder.AddState("exit", -1);
			ExitState->AddEnterMessage(0x33);
			ExitState->AddEnterMessage(0x2);

			Builder.CompileAndRegister(0x29CC4DD4, Private::S_PlayerDebugFlySM_FactoryFn, "PlayerDebugFlySM");

			Builder.Destroy();
		}
	}
}
