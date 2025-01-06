#pragma once

namespace RWS
{ 
	class CResourceHandler
	{
	public:

		virtual ~CResourceHandler() { /* implemented by game code */ }

	private:

		void* m_HandledTypeList = nullptr; // RWS::CResourceTypeInfo
	};
}
