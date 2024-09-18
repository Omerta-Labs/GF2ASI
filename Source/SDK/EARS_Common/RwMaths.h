#pragma once

struct RwV3d
{
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