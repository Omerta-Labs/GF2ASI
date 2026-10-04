#pragma once

// C++
#include <cstdint>
#include <string>

namespace Mod
{
	/**
	 * Read and write access to one ini file, over the Win32 private profile API.
	 *
	 * That API rather than a parser of our own because it is what the config
	 * files were authored against: it ignores the `//` comment lines they use,
	 * is indifferent to key order, and rewrites a single key without disturbing
	 * the rest of the file.
	 *
	 * It has no float accessor and it caches the files it has touched, so
	 * GetFloat, SetFloat and DropApiCache fill those two gaps.
	 */
	class ConfigFile
	{
	public:

		explicit ConfigFile(std::wstring InPath)
			: Path(std::move(InPath))
		{
		}

		const std::wstring& GetPath() const { return Path; }

		// Whether the file is on disk. A missing file is not an error - every
		// getter hands back its default and the setters create it - but it is
		// worth a log line, and worth skipping a block of reads for.
		bool Exists() const;

		bool GetBool(const wchar_t* InSection, const wchar_t* InKey, bool InDefault) const;
		int GetInt(const wchar_t* InSection, const wchar_t* InKey, int InDefault) const;
		uint32_t GetUInt(const wchar_t* InSection, const wchar_t* InKey, uint32_t InDefault) const;

		// Keeps InDefault on a missing, malformed or non-finite value.
		float GetFloat(const wchar_t* InSection, const wchar_t* InKey, float InDefault) const;

		bool SetBool(const wchar_t* InSection, const wchar_t* InKey, bool InValue);
		bool SetFloat(const wchar_t* InSection, const wchar_t* InKey, float InValue);

		/**
		 * Copy one whole section in from another file, keeping only real
		 * key=value entries - the profile API hands comment lines back
		 * verbatim but cannot parse them again on the way in.
		 *
		 * @return false if the source has no such section, or nothing in it
		 *         survived the filter.
		 */
		bool CopySectionFrom(const ConfigFile& InSource, const wchar_t* InSection);

		// Removes the section and every key in it.
		bool DeleteSection(const wchar_t* InSection);

		/**
		 * Drop the profile API's cached copy of this file, so reads that follow
		 * a change made behind its back - a file copy, say - see the new
		 * contents instead of a stale miss.
		 */
		void DropApiCache() const;

	private:

		std::wstring Path;
	};

	// Resolve "<game folder>/scripts/<InFileName>".
	std::wstring ResolveScriptsFilePath(const wchar_t* InFileName);

	namespace Config
	{
		/**
		 * The mod's two config files, resolved on the first call and usable
		 * from any point in start-up.
		 *
		 * Settings::Init seeds them from a pre-split gf2asi.ini when one is
		 * present, so tuning should be read after Init has run. Every fix does:
		 * Init_ModSystems runs ahead of Init_AttachHooks, and a fix reads its
		 * tuning when it installs.
		 */
		ConfigFile& SettingsFile();
		ConfigFile& KeybindsFile();
	}
}
