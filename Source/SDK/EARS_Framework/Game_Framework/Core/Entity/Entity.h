#pragma once

// SDK
#include "SDK/EARS_Common/RwMaths.h"
#include "SDK/EARS_Framework/Game_Framework/Core/Base/Base.h"

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
			virtual bool HasCollisionBinding() = 0;
			virtual void Unknown_1() = 0;
			virtual void Unknown_2() = 0;
			virtual void Unknown_3() = 0;
			virtual void Unknown_4() = 0;
			virtual void Unknown_5() = 0;
			virtual void Unknown_6() = 0;
			virtual void Unknown_7() = 0;
			virtual void Unknown_8() = 0;
			virtual void Rotate(const RwV3d& InOffset) = 0;
			virtual void Translate(const RwV3d& InOffset) = 0;


		private:

			// 0x90 -> LLTransform

			char m_EntityPadding_0[0x2C];
			uint16_t m_LinkPriority = 0;		// 0x7C
			char m_EntityPadding_1[0xE2];
			void* m_MetaModelInstance = nullptr; // 0x130 (EARS::Framework::MetaModelInstance)
			char m_EntityPadding_2[0x34];
			void* m_InterpolationMtx = nullptr; // 0x168 (EARS::Framework::Modules::imatrix44_t)
			void* m_AudOcclusionCB = nullptr; // 0x16C
			// Then entity finishes with one byte? TODO
		};
	} // Modules
} // EARS
