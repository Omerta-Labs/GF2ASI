#include "Addons/ConfigFile.h"

// C++
#include <cmath>
#include <cstdio>
#include <cwchar>
#include <filesystem>
#include <vector>
#include <windows.h>

namespace
{
	// Everything that is not a key binding
	constexpr wchar_t SETTINGS_FILE_NAME[] = L"gf2asi_settings.ini";

	// Key bindings only, shared with KeybindManager
	constexpr wchar_t KEYBINDS_FILE_NAME[] = L"gf2asi_keybinds.ini";
}

std::wstring Mod::ResolveScriptsFilePath(const wchar_t* const InFileName)
{
	// The module file name rather than std::filesystem's current path, which
	// gives bad results here - a doubled up scripts folder, for one.
	wchar_t RawExeBuffer[MAX_PATH] = {};
	GetModuleFileNameW(nullptr, RawExeBuffer, ARRAYSIZE(RawExeBuffer));

	static const std::filesystem::path SCRIPTS_FOLDER_NAME = "scripts";

	const std::filesystem::path ExecutablePath = RawExeBuffer;
	return (ExecutablePath.parent_path() / SCRIPTS_FOLDER_NAME / InFileName).wstring();
}

Mod::ConfigFile& Mod::Config::SettingsFile()
{
	static ConfigFile File(ResolveScriptsFilePath(SETTINGS_FILE_NAME));
	return File;
}

Mod::ConfigFile& Mod::Config::KeybindsFile()
{
	static ConfigFile File(ResolveScriptsFilePath(KEYBINDS_FILE_NAME));
	return File;
}

bool Mod::ConfigFile::Exists() const
{
	return std::filesystem::exists(Path);
}

bool Mod::ConfigFile::GetBool(const wchar_t* const InSection, const wchar_t* const InKey, const bool InDefault) const
{
	return GetPrivateProfileIntW(InSection, InKey, InDefault, Path.data()) != 0;
}

int Mod::ConfigFile::GetInt(const wchar_t* const InSection, const wchar_t* const InKey, const int InDefault) const
{
	return GetPrivateProfileIntW(InSection, InKey, InDefault, Path.data());
}

uint32_t Mod::ConfigFile::GetUInt(const wchar_t* const InSection, const wchar_t* const InKey, const uint32_t InDefault) const
{
	// GetPrivateProfileIntW is signed and clamps at 0 rather than wrapping, so
	// a negative entry reads back as 0 and the caller's range check catches it.
	return static_cast<uint32_t>(GetPrivateProfileIntW(InSection, InKey, static_cast<int>(InDefault), Path.data()));
}

float Mod::ConfigFile::GetFloat(const wchar_t* const InSection, const wchar_t* const InKey, const float InDefault) const
{
	wchar_t ValueBuffer[64] = {};
	GetPrivateProfileStringW(InSection, InKey, L"", ValueBuffer, ARRAYSIZE(ValueBuffer), Path.data());

	wchar_t* ParseEnd = nullptr;
	const float ParsedValue = std::wcstof(ValueBuffer, &ParseEnd);
	if (ParseEnd == ValueBuffer || !std::isfinite(ParsedValue))
	{
		return InDefault;
	}

	return ParsedValue;
}

bool Mod::ConfigFile::SetBool(const wchar_t* const InSection, const wchar_t* const InKey, const bool InValue)
{
	return WritePrivateProfileStringW(InSection, InKey, InValue ? L"1" : L"0", Path.data()) != FALSE;
}

bool Mod::ConfigFile::SetFloat(const wchar_t* const InSection, const wchar_t* const InKey, const float InValue)
{
	// %g keeps the output compact ("12", "0.08") while still round-tripping
	// through wcstof on the next load.
	wchar_t ValueBuffer[64] = {};
	swprintf_s(ValueBuffer, L"%.6g", InValue);

	return WritePrivateProfileStringW(InSection, InKey, ValueBuffer, Path.data()) != FALSE;
}

bool Mod::ConfigFile::CopySectionFrom(const ConfigFile& InSource, const wchar_t* const InSection)
{
	// GetPrivateProfileSectionW hands a section back as a double null
	// terminated run of "key=value" entries, which is the same shape
	// WritePrivateProfileSectionW takes - so the pair moves a section without
	// this code needing to know which keys are in it.
	//
	// It also truncates silently rather than reporting the size it wanted, so
	// start at the 32767 character cap the API tops out at anyway.
	std::vector<wchar_t> SectionBuffer(32767);
	if (GetPrivateProfileSectionW(InSection, SectionBuffer.data(), static_cast<DWORD>(SectionBuffer.size()), InSource.GetPath().data()) == 0)
	{
		return false;
	}

	std::vector<wchar_t> Entries;
	for (const wchar_t* Entry = SectionBuffer.data(); *Entry != L'\0'; Entry += std::wcslen(Entry) + 1)
	{
		if (std::wcschr(Entry, L'=') == nullptr)
		{
			continue;
		}

		Entries.insert(Entries.end(), Entry, Entry + std::wcslen(Entry) + 1);
	}

	if (Entries.empty())
	{
		return false;
	}

	// Second terminator, closing the double null terminated list
	Entries.push_back(L'\0');

	return WritePrivateProfileSectionW(InSection, Entries.data(), Path.data()) != FALSE;
}

bool Mod::ConfigFile::DeleteSection(const wchar_t* const InSection)
{
	// A null key name deletes the whole section.
	return WritePrivateProfileStringW(InSection, nullptr, nullptr, Path.data()) != FALSE;
}

void Mod::ConfigFile::DropApiCache() const
{
	// A null section, key and value flushes the cache.
	WritePrivateProfileStringW(nullptr, nullptr, nullptr, Path.data());
}
