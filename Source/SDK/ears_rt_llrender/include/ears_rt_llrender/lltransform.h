#pragma once

// NB: Because its terrible to pass about float[4][4],
// i've instead created a new type (which likely didn't exist)
// This is now LLMatrix, so it is easier to convert between LLMatrix and RwMatrixTag

struct LLMatrix
{
	float m_Matrix[4][4];
};

struct LLTransform
{
public:

	LLMatrix GetLocal() const { return m_Local; }

private:

	LLMatrix m_World;
	LLMatrix m_Local;
	LLTransform* m_Parent = nullptr;
	LLTransform* m_FirstChild = nullptr;
	LLTransform* m_NextSibling = nullptr;
	LLTransform* m_PrevSibling = nullptr;
};
