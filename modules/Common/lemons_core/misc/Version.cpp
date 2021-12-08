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
constexpr Version::Version (int maj, int min, int p)
    : major (maj)
    , minor (min)
    , patch (p)
{
}

constexpr bool Version::operator== (const Version& other) const noexcept
{
	return major == other.major && minor == other.minor && patch == other.patch;
}

constexpr bool Version::operator!= (const Version& other) const noexcept
{
	return ! (*this == other);
}

constexpr bool Version::operator> (const Version& other) const noexcept
{
	if (major < other.major) return false;

	if (minor < other.minor) return false;

	return patch > other.patch;
}

constexpr bool Version::operator< (const Version& other) const noexcept
{
	if (major > other.major) return false;

	if (minor > other.minor) return false;

	return patch < other.patch;
}

constexpr bool Version::isNewerThan (const Version& other) const noexcept
{
	return *this > other;
}

constexpr bool Version::isOlderThan (const Version& other) const noexcept
{
	return *this < other;
}

constexpr bool Version::hasSameMajorVersion (const Version& other) const noexcept
{
	return major == other.major;
}

void Version::bumpMajor() noexcept
{
	++major;
	minor = 0;
	patch = 0;
}

void Version::bumpMinor() noexcept
{
	++minor;
	patch = 0;
}

void Version::bumpPatch() noexcept
{
	++patch;
}

constexpr Version Version::withMajorBump() const
{
	return { major + 1,
		     0,
		     0 };
}

constexpr Version Version::withMinorBump() const
{
	return { major,
		     minor + 1,
		     0 };
}

constexpr Version Version::withPatchBump() const
{
	return { major,
		     minor,
		     patch + 1 };
}

String Version::toString() const noexcept
{
	return String (major) + "." + String (minor) + "." + String (patch);
}

Version Version::fromString (const String& string)
{
	const auto arr = juce::StringArray::fromTokens (string, ".", "");

	jassert (arr.size() == 3);

	return { arr[0].getIntValue(), arr[1].getIntValue(), arr[2].getIntValue() };
}

constexpr Version Version::juceVersion()
{
	return { JUCE_MAJOR_VERSION, JUCE_MINOR_VERSION, JUCE_BUILDNUMBER };
}

constexpr Version Version::lemonsVersion()
{
	return { LEMONS_VERSION_MAJOR, LEMONS_VERSION_MINOR, LEMONS_VERSION_PATCH };
}

constexpr static Version projectVersion()
{
	return { LEMONS_PROJECT_VERSION_MAJOR, LEMONS_PROJECT_VERSION_MINOR, LEMONS_PROJECT_VERSION_PATCH };
}

}  // namespace lemons
