
namespace bav
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

String Version::getAsString() const noexcept
{
    return String (major) + "." + String (minor) + "." + String (patch);
}

void Version::serialize (TreeReflector& ref)
{
    ref.add ("Major", major);
    ref.add ("Minor", minor);
    ref.add ("Patch", patch);
}

}  // namespace bav
