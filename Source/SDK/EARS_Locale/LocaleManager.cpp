#include "LocaleManager.h"

// Hooks
#include "Addons/Hook.h"

const std::unordered_map<std::string, std::string> EARS::Locale::LocaleManager::languageMap = {
	{"en", "English"},
	{"es", "Spanish"},
	{"ru", "Russian"},
	{"cs", "Czech"},
	{"nl", "Dutch"},
	{"pl", "Polish"},
	{"fr", "French"},
	{"de", "German"},
	{"it", "Italian"},
	{"jp", "Japanese"}
};

std::string EARS::Locale::LocaleManager::GetLanguageName(const char* code)
{
	auto it = languageMap.find(code);
	return (it != languageMap.end()) ? it->second.c_str() : "Unknown";
}

int EARS::Locale::LocaleManager::GetNumLanguages()
{
	return MemUtils::CallClassMethod<int, EARS::Locale::LocaleManager*>(0x604350, this);
}

bool EARS::Locale::LocaleManager::GetTextLanguageIsUserSelectable(int index)
{
	return MemUtils::CallClassMethod<bool, EARS::Locale::LocaleManager*, int>(0x602F00, this, index);
}

const char* EARS::Locale::LocaleManager::GetTextLanguageCode(int index)
{
	return MemUtils::CallClassMethod<char*, EARS::Locale::LocaleManager*, int>(0x602E60, this, index);
}

int EARS::Locale::LocaleManager::FindAudioLanguageIndex(const char* code)
{
	return MemUtils::CallClassMethod<int, EARS::Locale::LocaleManager*, const char*>(0x602FF0, this, code);
}

void EARS::Locale::LocaleManager::SetCurrentLanguage(int index)
{
	MemUtils::CallClassMethod<void, EARS::Locale::LocaleManager*, int>(0x604360, this, index);
}

void EARS::Locale::LocaleManager::SetCurrentAudioLanguage(int index)
{
	MemUtils::CallClassMethod<void, EARS::Locale::LocaleManager*, int>(0x604540, this, index);
}

EARS::Locale::LocaleManager* EARS::Locale::LocaleManager::GetInstance()
{
	return *(EARS::Locale::LocaleManager**)0x12234A4;
}