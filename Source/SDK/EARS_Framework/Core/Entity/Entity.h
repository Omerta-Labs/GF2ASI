#pragma once

// SDK
#include "SDK/EARS_Common/RwMaths.h"
#include "SDK/EARS_Framework/Core/Base/Base.h"
#include "SDK/EARS_RT_LLRender/include/LLTransform.h"

namespace EARS
{
	namespace Framework
	{
		class Entity : public EARS::Framework::Base
		{
		public:

			virtual void SetPosition(const RwV3d& InPosition) = 0;
			virtual void SetRotation(const RwMatrixTag& InMatrix) = 0;
			virtual void SetRotation(const RwV3d& InRotation) = 0;
			virtual void SetMatrix(const RwMatrixTag& InMatrix) = 0;
			virtual void SetMatrixNoScale(const RwMatrixTag& InMatrix) = 0;
			virtual void GetMatrixNoScale(RwMatrixTag& OutMatrix) = 0;
			virtual void GetAABBMinsMaxs() = 0; // TODO
			virtual void GetAABBMinsMaxsRelative() = 0; // TODO
			virtual bool HasCollisionBinding() = 0; // TODO
			virtual void Unknown_1() = 0; // TODO
			virtual void Unknown_2() = 0; // TODO
			virtual void Unknown_3() = 0; // TODO
			virtual void Unknown_4() = 0; // TODO
			virtual void Unknown_5() = 0; // TODO
			virtual void Unknown_6() = 0; // TODO
			virtual void Unknown_7() = 0; // TODO
			virtual void Unknown_8() = 0; // TODO
			virtual void Rotate(const RwV3d& InOffset) = 0;
			virtual void Translate(const RwV3d& InOffset) = 0;

			// Get the world position of this Entity
			// TODO: Is this actually local or world??
			RwV3d GetPosition() const;

			// Get the world matrix of this Entity
			// TODO: Is this actually local or world??
			RwMatrixTag GetMatrix() const;

			// Get the forward direction (facing direction) of this entity
			RwV3d GetForwardVector() const;

			// Get the up direction for this entity
			RwV3d GetUpVector() const;

		private:

			char m_EntityPadding_0[0x2C];
			uint16_t m_LinkPriority = 0;		// 0x7C
			char m_EntityPadding_1[0x12];
			LLTransform m_Transform;
			char m_EntityPadding_2[0x10];
			void* m_MetaModelInstance = nullptr; // 0x130 (EARS::Framework::MetaModelInstance)
			char m_EntityPadding_3[0x34];
			void* m_InterpolationMtx = nullptr; // 0x168 (EARS::Framework::Modules::imatrix44_t)
			void* m_AudOcclusionCB = nullptr; // 0x16C
			// Then entity finishes with one byte? TODO

			// 0x150 alpha value
			// 0x15E flags			

			char m_Entity_Padding_4[0x10];
		};

		static_assert(sizeof(Entity) == 0x180, "EARS::Framework::Entity must equal 0x180");
	} // Modules
} // EARS
