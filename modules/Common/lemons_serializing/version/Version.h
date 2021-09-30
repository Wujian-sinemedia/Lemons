#pragma once

namespace lemons
{
/**
    A class that represents a version of something, with major, minor, and patch numbers.
 
    @see SerializableData
 */
struct Version : SerializableData
{
    /** Creates a default Version object with the value 0.0.1. */
    Version() = default;

    /** Creates a Version object with a specified value. */
    Version (int maj, int min, int p);

    bool operator== (const Version& other) const noexcept;
    bool operator!= (const Version& other) const noexcept;

    /** Returns true if this version is newer than the other one. */
    bool operator> (const Version& other) const noexcept;

    /** Returns true if this version is older than the other one. */
    bool operator< (const Version& other) const noexcept;

    /** Returns a String representation of this Version.
        @param separator A character that will be placed between the major, minor, and patch values. Defaults to ".".
     */
    String getAsString (const String& separator = ".") const noexcept;

    /** Increments the major value and sets minor and patch to 0. */
    void bumpMajor() noexcept;

    /** Increments the minor value and sets patch to 0. */
    void bumpMinor() noexcept;

    /** Increments the patch value. */
    void bumpPatch() noexcept;

    /** Returns a new Version object with bumpMajor() applied.
        @see bumpMajor()
     */
    Version withMajorBump() const;

    /** Returns a new Version object with bumpMinor() applied.
        @see bumpMinor()
     */
    Version withMinorBump() const;

    /** Returns a new Version object with bumpPatch() applied.
     @see bumpPatch()
     */
    Version withPatchBump() const;

private:
    void serialize (TreeReflector& ref) final;

    int major {0}, minor {0}, patch {1};
};

}  // namespace lemons
