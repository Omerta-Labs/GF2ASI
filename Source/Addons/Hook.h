#pragma once

#define NOMINMAX
#define INLINE_CONSTEXPR        constexpr __forceinline

#include <type_traits>
#include <memory>
#include <utility>

#include <stdbool.h>
#include <Windows.h>
#include <stdlib.h> 
#include <psapi.h>

namespace MemUtils
{
	template <typename Ret, typename C, typename... Args>
	Ret CallClassMethod(unsigned long address, C _this, Args... args) {
		return (reinterpret_cast<Ret(__thiscall*)(C, Args...)>(address))(_this, args...);
	}

	template <typename Ret, typename... Args>
	Ret CallCdeclMethod(unsigned long address, Args... args) {
		return (reinterpret_cast<Ret(__cdecl*)(Args...)>(address))(args...);
	}

	template <typename Ret, typename... Args>
	Ret CallStdMethod(unsigned long address, Args... args) {
		return (reinterpret_cast<Ret(_stdcall*)(Args...)>(address))(args...);
	}
}

const std::uint32_t JUMP_OPCODE = 0xE9;
const std::uint32_t NOP_OPCODE = 0x90;

#pragma pack(push, 1)
struct SHook
{
	unsigned char jumpOpCode;
	unsigned int jumpLocation;
	unsigned char possibleNops[47] = { 0 }; // maximum size for nops

	SHook()
	{
		jumpOpCode = 0xE9;
		jumpLocation = 0;
	}
};
#pragma pack(pop)

template<typename T>
void HookInstall(DWORD installAddress, T addressToJumpTo, int iJmpCodeSize = 5)
{
	DWORD dwAddressToJumpTo;
	_asm
	{
		mov		eax, addressToJumpTo
		mov		dwAddressToJumpTo, eax
	}

	const DWORD x86FixedJumpSize = 5;
	SHook theHook;

	theHook.jumpLocation = (DWORD)dwAddressToJumpTo - (DWORD)installAddress - (DWORD)x86FixedJumpSize;
	memset(theHook.possibleNops, 0x90, iJmpCodeSize - x86FixedJumpSize);

	DWORD dwProtect[2];
	VirtualProtect((void*)installAddress, 5, PAGE_EXECUTE_READWRITE, &dwProtect[0]);
	memcpy((void*)installAddress, &theHook, iJmpCodeSize);
	VirtualProtect((void*)installAddress, 5, dwProtect[0], &dwProtect[1]);
}

namespace hook {
    template <typename TType, bool is_pointer = std::is_pointer<TType>::value, bool is_array = std::is_array<TType>::value>
    class Type {};

    /*
        Hook template for value types
    */
    template <typename TType>
    class Type<TType, false, false> {
    protected:
        TType* lpValue;
    public:
        constexpr Type(int address) : lpValue(reinterpret_cast<TType*>(address)) {};

        inline TType& get() const { return *lpValue; }
        inline void set(TType value) { *lpValue = value; }

        inline TType* ptr() const { return lpValue; }

        /*
            Operators
        */

        inline TType* operator->() const { return lpValue; };
        inline TType* operator&() const { return lpValue; };
        inline TType& operator*() const { return *lpValue; };
        inline TType* operator[](int index) const { return &lpValue[index]; }
        inline TType& operator=(TType value) { return (*lpValue = value); }

        inline operator TType& () const { return *lpValue; }

        /*
            Comparison operators
        */

        inline bool operator==(const TType& rhs) const { return *lpValue == rhs; }
        inline bool operator!=(const TType& rhs) const { return *lpValue != rhs; }

        /*
            Value-type operators
        */

        inline bool operator<(const TType& rhs) const { return *lpValue < rhs; }
        inline bool operator>(const TType& rhs) const { return *lpValue > rhs; }
        inline bool operator<=(const TType& rhs) const { return *lpValue <= rhs; }
        inline bool operator>=(const TType& rhs) const { return *lpValue >= rhs; }

        inline TType operator+() const { return +(*lpValue); }
        inline TType operator-() const { return -(*lpValue); }
        inline TType operator~() const { return ~(*lpValue); }

        inline TType operator+(const TType& rhs) const { return *lpValue + rhs; }
        inline TType operator-(const TType& rhs) const { return *lpValue - rhs; }
        inline TType operator*(const TType& rhs) const { return *lpValue * rhs; }
        inline TType operator/(const TType& rhs) const { return *lpValue / rhs; }
        inline TType operator%(const TType& rhs) const { return *lpValue % rhs; }
        inline TType operator&(const TType& rhs) const { return *lpValue & rhs; }
        inline TType operator|(const TType& rhs) const { return *lpValue | rhs; }
        inline TType operator^(const TType& rhs) const { return *lpValue ^ rhs; }
        inline TType operator<<(const TType& rhs) const { return *lpValue << rhs; }
        inline TType operator>>(const TType& rhs) const { return *lpValue >> rhs; }

        inline TType operator+=(const TType& rhs) { return (*lpValue += rhs); }
        inline TType operator-=(const TType& rhs) { return (*lpValue -= rhs); }
        inline TType operator*=(const TType& rhs) { return (*lpValue *= rhs); }
        inline TType operator/=(const TType& rhs) { return (*lpValue /= rhs); }
        inline TType operator%=(const TType& rhs) { return (*lpValue %= rhs); }
        inline TType operator&=(const TType& rhs) { return (*lpValue &= rhs); }
        inline TType operator|=(const TType& rhs) { return (*lpValue |= rhs); }
        inline TType operator^=(const TType& rhs) { return (*lpValue ^= rhs); }
        inline TType operator<<=(const TType& rhs) { return (*lpValue <<= rhs); }
        inline TType operator>>=(const TType& rhs) { return (*lpValue >>= rhs); }
    };

    /*
        Hook template for pointer types
    */
    template <typename TType>
    class Type<TType, true, false> {
    protected:
        TType* lpValue;
    public:
        constexpr Type(int address) : lpValue(reinterpret_cast<TType*>(address)) {};

        inline TType& get() const { return *lpValue; }
        inline void set(TType value) { *lpValue = value; }

        inline TType* ptr() const { return lpValue; }

        /*
            Operators
        */

        inline TType& operator->() const { return *lpValue; };
        inline TType* operator&() const { return lpValue; };
        inline TType& operator*() const { return *lpValue; };
        inline TType operator[](int index) const { return lpValue[index]; }
        inline TType& operator=(TType value) { return (*lpValue = value); }

        inline operator TType& () const { return *lpValue; }

        /*
            Comparison operators
        */

        inline bool operator==(const TType& rhs) const { return *lpValue == rhs; }
        inline bool operator!=(const TType& rhs) const { return *lpValue != rhs; }

        inline bool operator==(const std::nullptr_t& rhs) const
        {
            return *lpValue == nullptr;
        }
        inline bool operator!=(const std::nullptr_t& rhs) const
        {
            return *lpValue != nullptr;
        }

        template <typename... TArgs>
        inline auto operator()(TArgs... args) {
            return (*lpValue)(args...);
        }
    };

    /*
        Hook template for array types
    */
    template <typename TArray>
    class Type<TArray, false, true> {
        /*
            we need all this spaghett to resolve the actual array type
            because the fucking template isn't smart enough to do so
        */

        template <typename _T, int N>
        static constexpr _T _type(_T(*ary)[N]);

        template <typename _T, int N>
        static constexpr int _count(_T(*ary)[N]) {
            return N;
        };

        using type = decltype(_type((TArray*)nullptr));

        template <typename TRet, typename ...TArgs>
        using rtype = TRet;
    protected:
        using TValue = rtype<type>;

        TValue* lpValue;
    public:
        constexpr Type(int address) : lpValue(reinterpret_cast<TValue*>(address)) {};

        inline int count() const {
            return _count((TArray*)nullptr);
        }

        inline TValue* ptr() const { return lpValue; }
        inline TValue* ptr(int index) const { return lpValue + index; }

        /*
            Operators
        */

        inline TValue* operator&() const { return lpValue; };
        inline TValue& operator[](int index) const { return lpValue[index]; }

        template <typename TType>
        inline operator TType* () const { return reinterpret_cast<TType*>(lpValue); }
    };

    template <typename TType>
    class TypeProxy {
    protected:
        TType* lpValue;
    public:
        static_assert(!std::is_pointer<TType>::value, "Type proxy cannot be a pointer to a class.");

        constexpr TypeProxy(int address) : lpValue(reinterpret_cast<TType*>(address)) {};

        inline void read(TType& value) { memcpy(&value, lpValue, sizeof(TType)); }
        inline void write(TType& value) { memcpy(lpValue, &value, sizeof(TType)); }

        inline TType* operator->() const { return lpValue; }
        inline TType* operator&() const { return lpValue; }
        inline TType& operator*() const { return *lpValue; }
        inline TType& operator[](int index) const { return &lpValue[index]; }

        inline operator TType* () const { return lpValue; }
        inline operator TType& () const { return *lpValue; }
    };

    template<int offset, typename TValue>
    struct Field {
    public:
        template <class TThis>
        static INLINE_CONSTEXPR TValue get(const TThis* p) {
            return *(TValue*)((BYTE*)p + offset);
        };

        template <class TThis>
        static INLINE_CONSTEXPR void set(const TThis* p, TValue value) {
            *(TValue*)((BYTE*)p + offset) = value;
        };

        template <class TThis>
        static INLINE_CONSTEXPR TValue* ptr(const TThis* p) {
            return (TValue*)((BYTE*)p + offset);
        };
    };
};

template <typename TType>
using _Type = hook::Type<TType>;

template <typename TType>
using _TypeProxy = hook::TypeProxy<TType>;
