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

bool Version::operator== (const Version& other) const noexcept
{
	return major == other.major && minor == other.minor && patch == other.patch;
}

bool Version::operator!= (const Version& other) const noexcept
{
	return ! (*this == other);
}

bool Version::operator> (const Version& other) const noexcept
{
	if (major < other.major) return false;

	if (minor < other.minor) return false;

	return patch > other.patch;
}

bool Version::operator< (const Version& other) const noexcept
{
	if (major > other.major) return false;

	if (minor > other.minor) return false;

	return patch < other.patch;
}

bool Version::isNewerThan (const Version& other) const noexcept
{
	return *this > other;
}

bool Version::isOlderThan (const Version& other) const noexcept
{
	return *this < other;
}

bool Version::hasSameMajorVersion (const Version& other) const noexcept
{
	return major == other.major;
}

String Version::toString (const String& separator) const noexcept
{
	return String (major) + separator + String (minor) + separator + String (patch);
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

Version Version::withMajorBump() const
{
	return { major + 1,
		     0,
		     0 };
}

Version Version::withMinorBump() const
{
	return { major,
		     minor + 1,
		     0 };
}

Version Version::withPatchBump() const
{
	return { major,
		     minor,
		     patch + 1 };
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
