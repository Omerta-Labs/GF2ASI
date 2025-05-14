#pragma once

// GF2
#include <SDK/EARS_Common/Bitflags.h>
#include <SDK/EARS_Common/SafePtr.h>

namespace EARS
{
	// forward declares
	namespace Framework
	{
		class Entity;
	}

	namespace Havok
	{
		class CharacterProxy
		{
		public:

			// declare public facing enumerators
			enum class CollisionState : uint32_t
			{
				CS_ENABLED = 0x0,
				CS_TERRAIN_DISABLED = 0x1,
				CS_TRIGGERS_ONLY = 0x2,
				CS_ALL_DISABLED = 0x3,
			};

			void EnableGravity(bool bValue);

			bool IsGravityEnabled() const;

			void SetCollisionState(const CollisionState NewCollisionState);

			// accessors
			EARS::Framework::Entity* GetSupportingEntity() const { return m_SupportingEntity.GetPtr(); }
			int32_t GetSupportingSurfaceMaterialFlags() const { return m_SupportingEAMaterialFlags; }
			int32_t GetSupportingSurfaceMaterialType() const { return m_SupportingEAMaterialType; }

		private:

			enum class CharacterProxyFlag : uint32_t
			{
				FLAG_DISABLE_GRAVITY = 0x1,
				FLAG_DISABLE_LEDGE_DETECTION = 0x2,
				FLAG_LISTENER_DISABLED = 0x4,
				FLAG_DISABLE_COLLISIONDISABLED_GROUNDCHECK = 0x8,
				FLAG_CONTINUOUSLY_RECOMPUTE_UP = 0x10,
				FLAG_CHECK_SUPPORT_EVERY_FRAME = 0x20,
				FLAG_USE_RAYCAST_CHARACTER = 0x40,
				FLAG_SHOULD_USE_RAYCAST_CHARACTER = 0x80,
				FLAG_RAYCAST_CHARACTER_FIRST_UPDATE = 0x100,
				FLAG_TOUCH_TRIGGERS_WITH_RAYCAST_CHAR = 0x200,
				FLAG_SHOULD_FILTER_EDGE_COLLISIONS = 0x400,
				FLAG_PUSH_DYNAMIC_RIGIDBODIES = 0x800,
			};

			void* m_CharacterProxy_VTABLE = nullptr;
			char m_Padding_0[0xC8];
			int32_t m_SupportingEAMaterialType = 0;
			int32_t m_SupportingEAMaterialFlags = 0;
			uint32_t m_DefaultCollisionFilterInfo = 0;
			SafePtr<EARS::Framework::Entity> m_SupportingEntity;
			uint32_t m_RaycastCharacterTouchLayerMask = 0;
			Flags32 m_Flags;
			CollisionState m_CurrentCollisionState;
		};
	} // Havok
} // EARS
