#include "RwMaths.h"

// C++
#include <cmath>

RwV3d::RwV3d()
	: m_X(0.0f)
	, m_Y(0.0f)
	, m_Z(0.0f)
{

}

RwV3d::RwV3d(const RwV3d& Other)
	: m_X(Other.m_X)
	, m_Y(Other.m_Y)
	, m_Z(Other.m_Z)
{

}

bool RwV3d::Equal(const RwV3d& Other, const float Threshold) const
{
	return fabs((this->m_X - Other.m_X)) <= Threshold 
		&& fabs((this->m_Y - Other.m_Y)) <= Threshold 
		&& fabs((this->m_Z - Other.m_Z)) <= Threshold;
}

RwV3d::RwV3d(const float InValue)
	: m_X(InValue)
	, m_Y(InValue)
	, m_Z(InValue)
{

}

RwV3d::RwV3d(const float InX, const float InY, const float InZ)
	: m_X(InX)
	, m_Y(InY)
	, m_Z(InZ)
{

}

RwV3d RwV3d::operator+(const RwV3d& InA) const
{
	RwV3d Out;
	RwV3dAdd(Out, *this, InA);
	return Out;
}

RwV3d RwV3d::operator-(const RwV3d& InA) const
{
	RwV3d Out;
	RwV3dSub(Out, *this, InA);
	return Out;
}

RwV3d RwV3d::operator*(const RwV3d& InA) const
{
	RwV3d Out;
	RwV3dScale(Out, *this, InA);
	return Out;
}

RwV3d RwV3d::operator*(const float InA) const
{
	RwV3d Out;
	RwV3dScale(Out, *this, RwV3d(InA));
	return Out;
}

bool RwV3d::operator!=(const RwV3d& InA) const
{
	return m_X != InA.m_X || m_Y != InA.m_Y || m_Z != InA.m_Z;
}

bool RwV3d::operator==(const RwV3d& InA) const
{
	return m_X == InA.m_X && m_Y == InA.m_Y && m_Z == InA.m_Z;
}

void RwV3dAdd(RwV3d& Out, const RwV3d& InA, const RwV3d& InB)
{
	Out.m_X = InA.m_X + InB.m_X;
	Out.m_Y = InA.m_Y + InB.m_Y;
	Out.m_Z = InA.m_Z + InB.m_Z;
}

void RwV3dAssign(RwV3d& Out, const RwV3d& InA)
{
	Out.m_X = InA.m_X;
	Out.m_Y = InA.m_Y;
	Out.m_Z = InA.m_Z;
}

void RwV3dSub(RwV3d& Out, const RwV3d& InA, const RwV3d& InB)
{
	Out.m_X = InA.m_X - InB.m_X;
	Out.m_Y = InA.m_Y - InB.m_Y;
	Out.m_Z = InA.m_Z - InB.m_Z;
}

void RwV3dScale(RwV3d& Out, const RwV3d& InA, const RwV3d& InB)
{
	Out.m_X = InA.m_X * InB.m_X;
	Out.m_Y = InA.m_Y * InB.m_Y;
	Out.m_Z = InA.m_Z * InB.m_Z;
}

void RwV3dAddScale(RwV3d& Out, const RwV3d& InA, const RwV3d& InB, const float InScale)
{
	Out.m_X = InA.m_X + (InB.m_X * InScale);
	Out.m_Y = InA.m_Y + (InB.m_Y * InScale);
	Out.m_Z = InA.m_Z + (InB.m_Z * InScale);
}
