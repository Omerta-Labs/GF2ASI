#include "Entity.h"

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
