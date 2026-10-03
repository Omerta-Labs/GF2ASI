#pragma once

#ifdef DOXYGEN
#define RWS_MAKENEWCLASS(classname)
#else
#define RWS_MAKENEWCLASS(classname) \
static RWS::CAttributeHandler* MakeNew(const RWS::CAttributePacket& InAttr, uint32_t InComponentSize) \
{ \
	void* Mem = classname::operator new(sizeof(classname), InComponentSize); \
	return new (Mem) classname(InAttr); \
}
#endif

//RWS::CAttributeHandler *ptr = static_cast<RWS::CAttributeHandler*>(::new (Mem)  classname(InAttr)); \
	//return ptr; \