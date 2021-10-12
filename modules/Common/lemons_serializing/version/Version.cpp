
namespace lemons
{
Version::Version (int maj, int min, int p)
    : major (maj), minor (min), patch (p)
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

[[nodiscard]] String Version::getAsString (const String& separator) const noexcept
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

[[nodiscard]] Version Version::withMajorBump() const
{
    return {major + 1,
            0,
            0};
}

[[nodiscard]] Version Version::withMinorBump() const
{
    return {major,
            minor + 1,
            0};
}

[[nodiscard]] Version Version::withPatchBump() const
{
    return {major,
            minor,
            patch + 1};
}

void Version::serialize (TreeReflector& ref)
{
    ref.add ("Major", major);
    ref.add ("Minor", minor);
    ref.add ("Patch", patch);
}

}  // namespace lemons
