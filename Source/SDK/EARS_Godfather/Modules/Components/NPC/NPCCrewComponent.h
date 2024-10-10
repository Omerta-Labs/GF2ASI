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

		/**
		 * NPC Component to store Crew specific information.
		 */
		class NPCCrewComponent : public EARS::Framework::Component
		{
		public:

			/**
			 * Add a new Specialty to the component.
			 * If this Crew memeber is hired, it will update the HUD too.
			 */
			void AddNewCrewSpecialty(const uint32_t Specialty);

			/**
			 * Initialise this Crew Member as a Medic for the associated Family.
			 */
			void InitMedic();

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
