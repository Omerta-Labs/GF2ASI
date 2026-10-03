#pragma once

// SDK
#include "SDK/EARS_Common/Bitflags.h"
#include "SDK/EARS_Common/SafePtr.h"
#include "SDK/EARS_Framework/Core/Component/Component.h"

// CPP
#include <stdint.h>

namespace EARS
{
	namespace Modules
	{
		struct Vec3
		{
			float X = 0.0f;
			float Y = 0.0f;
			float Z = 0.0f;
		};

		class NPC;

		enum class CrewSettings : int32_t
		{
			CrewSettings_REF = -1,
			CREW_FLAG_CREW = 0x1,
			CREW_FLAG_UNCOMMANDABLE = 0x2,
			CREW_FLAG_SPECIALIST_DEMOLITIONS = 0x4,
			CREW_FLAG_SPECIALIST_ARSON = 0x8,
			CREW_FLAG_SPECIALIST_SAFE_CRACKER = 0x10,
			CREW_FLAG_SPECIALIST_ENGINEER = 0x20,
			CREW_FLAG_SPECIALIST_MEDIC = 0x40,
			CREW_FLAG_SPECIALIST_BRUISER = 0x80,
			CREW_FLAG_CAN_RECRUIT = 0x100,
		};

		/**
		 * NPC Component to store Crew specific information.
		 */
		class NPCCrewComponent : public EARS::Framework::Component
		{
		public:

			/**
			 * Whether or not the NPC which owns this component can
			 * be recruited by the Player
			 */
			bool CanRecruit() const;

			/**
			 * Add a new Specialty to the component.
			 * If this Crew memeber is hired, it will update the HUD too.
			 */
			void AddNewCrewSpecialty(const uint32_t Specialty);

			/**
			 * Initialise this Crew Member as a Medic for the associated Family.
			 */
			void InitMedic();

			/**
			 * Has this crew member to be considered as hired
			 */
			bool IsHiredCrew() const;

		private:

			char m_IDamageListener_Padding0[0xC];
			SafePtr<void*> m_CrewLeaderSentient; // EARS::Modules::Sentient
			void* m_CrewLeaderComponent = nullptr; // EARS::Modules::CrewLeaderComponent
			EARS::Modules::NPC* m_NPC = nullptr;
			void* m_Follower = nullptr; // EARS::Modules::NPCFollowComponent
			SafePtr<void*> m_Target; // EARS::Modules::Sentient
			float m_OutOfRangeTimer = 0.0f;
			float m_HealDurationFraction = 0.0f;
			float m_CommandTimeDelay = 0.0f;
			Flags16 m_CrewSettings;
			Flags16 m_CrewFlags;
			Vec3 m_CrewMovePos;
			void* m_VFXHandle = nullptr; // EARS::Alchemy::VFXHandle
		};
	} // Modules
} // EARS
