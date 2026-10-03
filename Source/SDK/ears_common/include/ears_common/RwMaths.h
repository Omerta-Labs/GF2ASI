#pragma once

// C++
#include <cstdint>

/**
 * Customised version of Renderware's RwV3d
 * This is built more to make use of new C++ tech
 * Renderware is a little dated now.
 */
struct RwV3d
{
public:

	// default constructor. sets all to 0.0f
	RwV3d();
	
	// sets all to specified 'InValue'
	RwV3d(const float InValue);

	// applies values to each component
	RwV3d(const float InX, const float InY, const float InZ);

	// copies the values from the other RwV3d
	RwV3d(const RwV3d& Other);

	// checks whether this vector3 equals another, using a specific threshold
	bool Equal(const RwV3d& Other, const float Threshold) const;

	// operator overloads

	// addition
	RwV3d operator+(const RwV3d& InA) const;

	// subtract
	RwV3d operator-(const RwV3d& InA) const;

	// multiply
	RwV3d operator*(const RwV3d& InA) const;
	RwV3d operator*(const float InA) const;

	// equality checks
	bool operator==(const RwV3d& InA) const;
	bool operator!=(const RwV3d& InA) const;

	float m_X = 0.0f;
	float m_Y = 0.0f;
	float m_Z = 0.0f;
};

struct RwMatrixTag
{
	RwMatrixTag();

	RwV3d m_Right;
	float pad0;
	RwV3d m_Up;
	float pad1;
	RwV3d m_At;
	float pad2;
	RwV3d m_Pos;
	float pad3;
};

// functions
// (from engine, feel free to use operator overloads for simplicity)
void RwV3dAdd(RwV3d& Out, const RwV3d& InA, const RwV3d& InB);
void RwV3dAssign(RwV3d& Out, const RwV3d& InA);
void RwV3dSub(RwV3d& Out, const RwV3d& InA, const RwV3d& InB);
void RwV3dScale(RwV3d& Out, const RwV3d& InA, const RwV3d& InB);
void RwV3dAddScale(RwV3d& Out, const RwV3d& InA, const RwV3d& InB, const float InScale);

void RwMatrixSetIdentity(RwMatrixTag& InMatrix);

// Combine modes for RwMatrixRotate (matches Renderware's RwOpCombineType)
enum RwOpCombineType : uint32_t
{
	rwCOMBINEREPLACE = 0,		// InMatrix becomes the rotation
	rwCOMBINEPRECONCAT = 1,		// InMatrix = rotation * InMatrix
	rwCOMBINEPOSTCONCAT = 2,	// InMatrix = InMatrix * rotation
};

// Builds a rotation of 'InAngleDegrees' about 'InAxis' (Rodrigues) and combines it into 'InMatrix'.
void RwMatrixRotate(RwMatrixTag& InMatrix, const RwV3d& InAxis, float InAngleDegrees, RwOpCombineType InCombineOp);

// Out = InA * InB using Renderware's row-vector convention (Out.row[i] = sum_k InA[i][k] * InB.row[k]).
// 'Out' may safely alias 'InA' or 'InB'.
void RwMatrixMultiply(RwMatrixTag& Out, const RwMatrixTag& InA, const RwMatrixTag& InB);

namespace EARS
{
	// Extracts Euler angles (radians) from the rotation part of 'InMatrix'.
	// Result is stored as { X = pitch, Y = yaw, Z = roll }.
	void RwMatrixExtractEulerAngles(RwV3d& Result, const RwMatrixTag& InMatrix);

	// Copies an RwV3d into a 4-float vector, setting the W component to 1.0f.
	void ConvertRwV3dToVec4flt(float Out[4], const RwV3d& In);
}
