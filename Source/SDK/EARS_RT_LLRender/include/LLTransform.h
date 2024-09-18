#pragma once

struct LLTransform
{
	float m_World[4][4];
	float m_Local[4][4];
	LLTransform* m_Parent = nullptr;
	LLTransform* m_FirstChild = nullptr;
	LLTransform* m_NextSibling = nullptr;
	LLTransform* m_PrevSibling = nullptr;
};
