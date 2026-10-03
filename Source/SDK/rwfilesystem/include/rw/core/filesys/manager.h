#pragma once

namespace rw::core::filesys
{
	class Manager
	{
	public:

		enum class SearchPathFlags
		{
			ADD_TO_TAIL = 0x0,
			ADD_TO_HEAD = 0x1,
		};

		void AddSearchLocation(const char* Location, SearchPathFlags Flags);

		/* static */
		static Manager* GetInstance();

	private:
	};
} // rw::core::filesys