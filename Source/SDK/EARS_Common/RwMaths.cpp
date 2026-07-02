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

void RwMatrixSetIdentity(RwMatrixTag& InMatrix)
{
	InMatrix.m_Right.m_X = 1.0f;
	InMatrix.m_Right.m_Y = 0.0f;
	InMatrix.m_Right.m_Z = 0.0f;

	InMatrix.m_Up.m_X = 0.0f;
	InMatrix.m_Up.m_Y = 1.0f;
	InMatrix.m_Up.m_Z = 0.0f;

	InMatrix.m_At.m_X = 0.0f;
	InMatrix.m_At.m_Y = 0.0f;
	InMatrix.m_At.m_Z = 1.0f;

	InMatrix.pad0 = 0.0f;
	InMatrix.pad1 = 0.0f;
	InMatrix.pad2 = 0.0f;
	InMatrix.pad3 = 1.0f;
}

RwMatrixTag::RwMatrixTag()
{
	RwMatrixSetIdentity(*this);
}

static RwV3d RwV3dNormalize(const RwV3d& InVector)
{
	const float LengthSq = (InVector.m_X * InVector.m_X) + (InVector.m_Y * InVector.m_Y) + (InVector.m_Z * InVector.m_Z);
	const float Length = sqrtf(LengthSq);

	if (Length <= 0.0f)
	{
		return InVector;
	}

	const float InvLength = 1.0f / Length;
	return RwV3d(InVector.m_X * InvLength, InVector.m_Y * InvLength, InVector.m_Z * InvLength);
}

void RwMatrixRotate(RwMatrixTag& InMatrix, const RwV3d& InAxis, float InAngleDegrees, RwOpCombineType InCombineOp)
{
	// Engine constant: degrees -> radians (0x3c8efa35)
	constexpr float DegreesToRadians = 0.017453292f;

	const float Radians = InAngleDegrees * DegreesToRadians;
	const float Sine = sinf(Radians);
	const float OneMinusCosine = 1.0f - cosf(Radians);

	const RwV3d Axis = RwV3dNormalize(InAxis);
	const float X = Axis.m_X;
	const float Y = Axis.m_Y;
	const float Z = Axis.m_Z;

	// Rodrigues' rotation matrix (Renderware row-vector layout)
	RwMatrixTag Rotation;
	Rotation.m_Right = RwV3d(1.0f - ((1.0f - (X * X)) * OneMinusCosine), (X * Y * OneMinusCosine) + (Z * Sine), (Z * X * OneMinusCosine) - (Y * Sine));
	Rotation.m_Up    = RwV3d((X * Y * OneMinusCosine) - (Z * Sine), 1.0f - ((1.0f - (Y * Y)) * OneMinusCosine), (Y * Z * OneMinusCosine) + (X * Sine));
	Rotation.m_At    = RwV3d((Z * X * OneMinusCosine) + (Y * Sine), (Y * Z * OneMinusCosine) - (X * Sine), 1.0f - ((1.0f - (Z * Z)) * OneMinusCosine));
	Rotation.m_Pos   = RwV3d(0.0f, 0.0f, 0.0f);

	switch (InCombineOp)
	{
	case rwCOMBINEPRECONCAT:
		RwMatrixMultiply(InMatrix, Rotation, InMatrix);
		break;

	case rwCOMBINEPOSTCONCAT:
		RwMatrixMultiply(InMatrix, InMatrix, Rotation);
		break;

	case rwCOMBINEREPLACE:
	default:
		InMatrix = Rotation;
		break;
	}
}

void RwMatrixMultiply(RwMatrixTag& Out, const RwMatrixTag& InA, const RwMatrixTag& InB)
{
	// Use a temporary so 'Out' may alias either input.
	RwMatrixTag Result;

	Result.m_Right = (InB.m_Right * InA.m_Right.m_X) + (InB.m_Up * InA.m_Right.m_Y) + (InB.m_At * InA.m_Right.m_Z);
	Result.m_Up    = (InB.m_Right * InA.m_Up.m_X)    + (InB.m_Up * InA.m_Up.m_Y)    + (InB.m_At * InA.m_Up.m_Z);
	Result.m_At    = (InB.m_Right * InA.m_At.m_X)    + (InB.m_Up * InA.m_At.m_Y)    + (InB.m_At * InA.m_At.m_Z);
	Result.m_Pos   = (InB.m_Right * InA.m_Pos.m_X)   + (InB.m_Up * InA.m_Pos.m_Y)   + (InB.m_At * InA.m_Pos.m_Z) + InB.m_Pos;

	Out = Result;
}

void EARS::ConvertRwV3dToVec4flt(float Out[4], const RwV3d& In)
{
	Out[0] = In.m_X;
	Out[1] = In.m_Y;
	Out[2] = In.m_Z;
	Out[3] = 1.0f;
}

void EARS::RwMatrixExtractEulerAngles(RwV3d& Result, const RwMatrixTag& InMatrix)
{
	const RwV3d& Right = InMatrix.m_Right;
	const RwV3d& Up = InMatrix.m_Up;
	const RwV3d& At = InMatrix.m_At;

	// 0x3fc90fdb == half pi
	constexpr float HalfPi = 1.5707964f;

	auto ClampUnit = [](float Value) -> float
	{
		if (Value <= -1.0f) { return -1.0f; }
		if (Value >= 1.0f) { return 1.0f; }
		return Value;
	};

	float Pitch = 0.0f;
	float Yaw = 0.0f;
	float Roll = 0.0f;

	if (At.m_Z != 0.0f)
	{
		Yaw = atan2f(At.m_X, At.m_Z);
		Pitch = -asinf(ClampUnit(At.m_Y));

		if (Up.m_Y != 0.0f)
		{
			Roll = atan2f(Right.m_Y, Up.m_Y);
		}
		else if (Right.m_Y != 0.0f)
		{
			Roll = ((cosf(Pitch) * Right.m_Y) > 0.0f) ? HalfPi : -HalfPi;
		}
	}
	else if (At.m_X != 0.0f)
	{
		Pitch = -asinf(ClampUnit(At.m_Y));
		Yaw = ((At.m_X * cosf(Pitch)) > 0.0f) ? HalfPi : -HalfPi;

		if (Up.m_Y != 0.0f)
		{
			Roll = atan2f(Right.m_Y, Up.m_Y);
		}
		else
		{
			Roll = ((Right.m_Y * cosf(Pitch)) > 0.0f) ? HalfPi : -HalfPi;
		}
	}
	else
	{
		Pitch = (At.m_Y > 0.0f) ? -HalfPi : HalfPi;
		Roll = 0.0f;

		if (Right.m_X != 0.0f)
		{
			Yaw = -atan2f(Right.m_X, Right.m_X);
		}
		else
		{
			Yaw = (Right.m_Z > 0.0f) ? -HalfPi : HalfPi;
		}
	}

	Result.m_X = Pitch;
	Result.m_Y = Yaw;
	Result.m_Z = Roll;
}
