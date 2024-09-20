#pragma once

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

	// copies the values from the other RwV3d
	RwV3d(const RwV3d& Other);

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