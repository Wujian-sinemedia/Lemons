/*
 ======================================================================================

 ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝

 This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.

 ======================================================================================
 */


#pragma once

#include <juce_core/juce_core.h>

#ifndef LEMONS_VERSION_MAJOR
#  define LEMONS_VERSION_MAJOR 0
#endif

#ifndef LEMONS_VERSION_MINOR
#  define LEMONS_VERSION_MINOR 0
#endif

#ifndef LEMONS_VERSION_PATCH
#  define LEMONS_VERSION_PATCH 1
#endif

#ifndef LEMONS_PROJECT_VERSION_MAJOR
#  define LEMONS_PROJECT_VERSION_MAJOR 0
#endif

#ifndef LEMONS_PROJECT_VERSION_MINOR
#  define LEMONS_PROJECT_VERSION_MINOR 0
#endif

#ifndef LEMONS_PROJECT_VERSION_PATCH
#  define LEMONS_PROJECT_VERSION_PATCH 1
#endif

namespace lemons
{

using juce::String;

/**
    A class that represents a version of something, with major, minor, and patch numbers.
 */
struct Version final
{
	/** Creates a default Version object with the value 0.0.1. */
	constexpr explicit Version() = default;

	/** Creates a Version object with a specified value. */
	constexpr explicit Version (int maj, int min, int p)
	    : major (maj)
	    , minor (min)
	    , patch (p)
	{
	}

	/** Returns true if this version is the same as the other one. */
	[[nodiscard]] constexpr bool operator== (const Version& other) const noexcept
	{
		return major == other.major && minor == other.minor && patch == other.patch;
	}

	/** Returns true if this version is not the same as the other one. */
	[[nodiscard]] constexpr bool operator!= (const Version& other) const noexcept
	{
		return ! (*this == other);
	}

	/** Returns true if this version is newer than the other one. */
	[[nodiscard]] constexpr bool operator> (const Version& other) const noexcept
	{
		if (major < other.major) return false;

		if (minor < other.minor) return false;

		return patch > other.patch;
	}

	/** Returns true if this version is older than the other one. */
	[[nodiscard]] constexpr bool operator< (const Version& other) const noexcept
	{
		if (major > other.major) return false;

		if (minor > other.minor) return false;

		return patch < other.patch;
	}

	/** Returns true if this version has the same major release number as the other one. */
	[[nodiscard]] constexpr bool hasSameMajorVersion (const Version& other) const noexcept
	{
		return major == other.major;
	}

	/** Returns true if this version is newer than the other one. */
	[[nodiscard]] constexpr bool isNewerThan (const Version& other) const noexcept
	{
		return *this > other;
	}

	/** Returns true if this version is older than the other one. */
	[[nodiscard]] constexpr bool isOlderThan (const Version& other) const noexcept
	{
		return *this < other;
	}

	/** Increments the major value and sets minor and patch to 0. */
	void bumpMajor() noexcept;

	/** Increments the minor value and sets patch to 0. */
	void bumpMinor() noexcept;

	/** Increments the patch value. */
	void bumpPatch() noexcept;

	/** Returns the major version number. */
	[[nodiscard]] constexpr int getMajor() const noexcept
	{
		return major;
	}

	/** Returns the minor version number. */
	[[nodiscard]] constexpr int getMinor() const noexcept
	{
		return minor;
	}

	/** Returns the patch version number. */
	[[nodiscard]] constexpr int getPatch() const noexcept
	{
		return patch;
	}

	/** Returns a new Version object with bumpMajor() applied.
	    @see bumpMajor()
	 */
	[[nodiscard]] constexpr Version withMajorBump() const
	{
		return Version { major + 1,
			             0,
			             0 };
	}

	/** Returns a new Version object with bumpMinor() applied.
	    @see bumpMinor()
	 */
	[[nodiscard]] constexpr Version withMinorBump() const
	{
		return Version { major,
			             minor + 1,
			             0 };
	}

	/** Returns a new Version object with bumpPatch() applied.
	    @see bumpPatch()
	 */
	[[nodiscard]] constexpr Version withPatchBump() const
	{
		return Version { major,
			             minor,
			             patch + 1 };
	}

	/** Returns a String representation of this Version, with a '.' placed between the major, minor, and patch integers.
	 */
	[[nodiscard]] String toString() const noexcept;

	/** Returns a Version object from a String representation. The string should be in the format '0.0.1', for example.
	 */
	[[nodiscard]] static Version fromString (const String& string);

	/** Returns a Version object representing the version of Juce this module was compiled with. */
	[[nodiscard]] constexpr static Version juceVersion()
	{
		return Version { JUCE_MAJOR_VERSION, JUCE_MINOR_VERSION, JUCE_BUILDNUMBER };
	}

	/** Returns a Version object representing the version of Lemons this module was compiled with. */
//	[[nodiscard]] constexpr static Version lemonsVersion()
//	{
//		return Version { LEMONS_VERSION_MAJOR, LEMONS_VERSION_MINOR, LEMONS_VERSION_PATCH };
//	}

	/** Returns a Version object representing the version of the current project that this module was compiled as a part of.
	    Note that this will only be correct if you use the Lemons repo's CMake configuration scripts, or manually define the macros LEMONS_PROJECT_VERSION_MAJOR, LEMONS_PROJECT_VERSION_MINOR, and LEMONS_PROJECT_VERSION_PATCH.
	 */
//	[[nodiscard]] constexpr static Version projectVersion()
//	{
//		return Version { LEMONS_PROJECT_VERSION_MAJOR, LEMONS_PROJECT_VERSION_MINOR, LEMONS_PROJECT_VERSION_PATCH };
//	}

private:
	int major { 0 }, minor { 0 }, patch { 1 };
};

}  // namespace lemons
