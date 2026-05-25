#pragma once

namespace EARS::Common
{
	template<typename TType>
	struct SingleInternalLinkedListLightweight
	{
	public:

		void Remove(TType* NodeToRemove)
		{
			if (m_Head == NodeToRemove)
			{
				m_Head = NodeToRemove->GetNext();
			}
			else
			{
				TType* CurrentNode = m_Head;
				while (CurrentNode)
				{
					if (CurrentNode == NodeToRemove)
					{
						TType* NewNext = NodeToRemove->GetNext();
						CurrentNode->SetNext(NewNext);
					}
				}
			}
		}

		bool IsEmpty() const { return (m_Head == nullptr); }

		TType* GetFront() const { return m_Head; }

	private:

		TType* m_Head = nullptr;
	};
}