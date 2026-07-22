#include "SDK/rwfilesystem/include/rwfilesysmanager.h"

// Scripthook
#include "addons/hook.h"

namespace rw::core::filesys
{
	void Manager::AddSearchLocation(const char* Location, SearchPathFlags Flags)
	{
		MemUtils::CallClassMethod<void>(0x0AC1120, this, Location, Flags);
	}
	
	/* static */
	Manager* Manager::GetInstance()
	{
		return *(Manager**)0x113D18C;
	}

}
