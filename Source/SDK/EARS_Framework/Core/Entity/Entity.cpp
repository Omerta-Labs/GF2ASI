#include "Entity.h"

#include "Addons/Hook.h"

void EARS::Framework::Entity::SetHeadingToPosition(const RwV3d& TargetPosition, const RwV3d* UpDirection)
{
	const RwV3d CurrentPosition = GetPosition();
	const RwV3d Diff = (TargetPosition - CurrentPosition);
	SetHeadingToDirection(Diff, UpDirection);
}

void EARS::Framework::Entity::SetHeadingToDirection(const RwV3d& TargetDir, const RwV3d* UpDirection)
{
	const RwV3d v7 = { 0.0f, 0.0f, 0.0f };

	const RwV3d WorkingUpDir = (UpDirection ? *UpDirection : RwV3d(0.0, 1.0f, 0.0f));

	// looks like we're calculating magniture
	const float Magnitude = (WorkingUpDir.m_X * TargetDir.m_X) + (WorkingUpDir.m_Y * TargetDir.m_Y) + (WorkingUpDir.m_Z * TargetDir.m_Z);

	// only apply under specific circumstances
	RwMatrixTag NewHeading = {};
	RwV3dAddScale(NewHeading.m_At, TargetDir, WorkingUpDir, (-1.0f * Magnitude));
	if (!NewHeading.m_At.Equal(v7, 0.00000011920929f))
	{
		NewHeading.m_Right.m_X = (WorkingUpDir.m_Y * NewHeading.m_At.m_Z) - (WorkingUpDir.m_Z * NewHeading.m_At.m_Y);
		NewHeading.m_Right.m_Y = (WorkingUpDir.m_Z * NewHeading.m_At.m_X) - (WorkingUpDir.m_X * NewHeading.m_At.m_Z);
		NewHeading.m_Right.m_Z = (WorkingUpDir.m_X * NewHeading.m_At.m_Y) - (WorkingUpDir.m_Y * NewHeading.m_At.m_X);
		NewHeading.m_Up.m_X = (NewHeading.m_At.m_Y * NewHeading.m_Right.m_Z) - (NewHeading.m_At.m_Z * NewHeading.m_Right.m_Y);
		NewHeading.m_Up.m_Y = (NewHeading.m_At.m_Z * NewHeading.m_Right.m_X) - (NewHeading.m_At.m_X * NewHeading.m_Right.m_Z);
		NewHeading.m_Up.m_Z = (NewHeading.m_At.m_X * NewHeading.m_Right.m_Y) - (NewHeading.m_At.m_Y * NewHeading.m_Right.m_X);
		MemUtils::CallCdeclMethod<RwMatrixTag*, RwMatrixTag&, RwMatrixTag&>(0x043A2A0, NewHeading, NewHeading); // RwMatrixOrthoNormalize

		// TODO: This should be valling the VTable version but for some reason that doesnt work!!
		MemUtils::CallClassMethod<void, Entity*, const RwMatrixTag*>(0x046F9C0, this, &NewHeading);
	}
}

RwV3d EARS::Framework::Entity::GetPosition() const
{
	return GetMatrix().m_Pos;
}

RwMatrixTag EARS::Framework::Entity::GetMatrix() const
{
	// I assume this is what the actual engine does too.
	// Initially this engine used Renderware, and all its core classes, including maths.
	// Somewhere between The Godfather I and The Godfather II, they built their own renderer and core classes.
	// Now at this point, their codebase has a mixture of Renderware types AND LLRender.
	// Since the structure of LLMatrix (which is actually float[4][4]) is exactly the same as RwMatrixTag,
	// We can just do this hack to avoid any costly conversions between types.

	LLMatrix Local = m_Transform.GetLocal();
	return *(RwMatrixTag*)&Local;
}

RwV3d EARS::Framework::Entity::GetForwardVector() const
{
	return GetMatrix().m_At;
}

RwV3d EARS::Framework::Entity::GetUpVector() const
{
	return GetMatrix().m_Up;
}
