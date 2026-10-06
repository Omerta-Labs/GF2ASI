//=============================================================================
// Calling into the game image, and reading its globals.
//
// Reconstructed SDK classes are thin shells over a specific godfather2.exe:
// their members forward to the original code at fixed addresses, and their
// globals are read through hook::Type.
//
// MSVC x86 only. The CallEax* thunks are inline __asm because no calling
// convention passes arguments in EAX, which the game's __usercall and
// __userpurge functions require.
//=============================================================================

#pragma once

#define NOMINMAX
#define INLINE_CONSTEXPR        constexpr __forceinline

#include <type_traits>
#include <memory>
#include <utility>

#include <stdbool.h>
#include <Windows.h>
#include <stdlib.h>
#include <string.h>
#include <psapi.h>

namespace MemUtils
{
	// Overwrites a value inside the executable image, lifting the page
	// protection for the duration. For the fixes that have to change a single
	// instruction operand rather than detour a whole function - usually because
	// the function takes arguments in registers no MSVC calling convention can
	// express.
	template <typename T>
	bool WriteMemory(uintptr_t address, const T& value)
	{
		void* const Target = reinterpret_cast<void*>(address);

		DWORD OldProtect = 0;
		if (!VirtualProtect(Target, sizeof(T), PAGE_EXECUTE_READWRITE, &OldProtect))
		{
			return false;
		}

		memcpy(Target, &value, sizeof(T));

		DWORD UnusedProtect = 0;
		VirtualProtect(Target, sizeof(T), OldProtect, &UnusedProtect);
		FlushInstructionCache(GetCurrentProcess(), Target, sizeof(T));

		return true;
	}

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

	// Invokes a function that receives its single pointer argument in EAX and takes no
	// stack arguments (IDA "__usercall f@<eax>(arg@<eax>)"; callee returns via bare retn).
	// No standard MSVC calling convention passes the first argument in EAX, hence the thunk.
	inline void CallEaxVoidMethod(unsigned long address, void* eaxArg) {
		__asm
		{
			mov eax, eaxArg
			mov edx, address
			call edx
		}
	}

	// Invokes a function that receives `this` in ESI and two further arguments on the
	// stack, which it pops itself (IDA "__usercall f(this@<esi>, a, b)"; callee ends
	// `retn 8`). ESI is callee-saved, so the thunk restores it around the call.
	inline void CallEsiVoidMethod(unsigned long address, void* esiArg, void* stackArg0, unsigned long stackArg1) {
		__asm
		{
			push esi
			push stackArg1
			push stackArg0
			mov esi, esiArg
			mov edx, address
			call edx
			pop esi
		}
	}

	// Invokes a function that receives its first two arguments in EAX and ECX and three
	// more on the stack, which it pops itself (IDA
	// "__userpurge f@<eax>(a@<eax>, b@<ecx>, c, d, e)"; callee ends `retn 0Ch`).
	// MSVC produces this convention for a member function it has specialised to its call
	// sites: `this` is an ordinary stack argument rather than being in ECX, so __thiscall
	// is as wrong here as __cdecl and __stdcall are.
	//
	// stackArg0 is the argument nearest the return address, so it is pushed last. Nothing
	// cleans up after the call because the callee already did. The pushes do not disturb
	// the remaining parameter reads: MSVC always gives a function containing __asm a frame
	// pointer, so each named parameter resolves EBP-relative rather than off ESP.
	inline void CallEaxEcxVoidMethod(unsigned long address, void* eaxArg, const void* ecxArg,
	                                 void* stackArg0, void* stackArg1, unsigned long stackArg2) {
		__asm
		{
			push stackArg2
			push stackArg1
			push stackArg0
			mov eax, eaxArg
			mov ecx, ecxArg
			mov edx, address
			call edx
		}
	}
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
