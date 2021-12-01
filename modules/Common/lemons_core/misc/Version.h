#pragma once

namespace lemons
{
/**
    A class that represents a version of something, with major, minor, and patch numbers.
 */
struct Version
{
	/** Creates a default Version object with the value 0.0.1. */
	Version() = default;

	/** Creates a Version object with a specified value. */
	Version (int maj, int min, int p);

	/** Returns true if this version is the same as the other one. */
	bool operator== (const Version& other) const noexcept;

	/** Returns true if this version is not the same as the other one. */
	bool operator!= (const Version& other) const noexcept;

	/** Returns true if this version is newer than the other one. */
	bool operator> (const Version& other) const noexcept;

	/** Returns true if this version is older than the other one. */
	bool operator< (const Version& other) const noexcept;

	/** Returns true if this version has the same major release number as the other one. */
	[[nodiscard]] bool hasSameMajorVersion (const Version& other) const noexcept;

	/** Returns a String representation of this Version.
	    @param separator A character that will be placed between the major, minor, and patch values. Defaults to ".".
	 */
	[[nodiscard]] String toString (const String& separator = ".") const noexcept;

	/** Increments the major value and sets minor and patch to 0. */
	void bumpMajor() noexcept;

	/** Increments the minor value and sets patch to 0. */
	void bumpMinor() noexcept;

	/** Increments the patch value. */
	void bumpPatch() noexcept;

	/** Returns a new Version object with bumpMajor() applied.
	    @see bumpMajor()
	 */
	[[nodiscard]] Version withMajorBump() const;

	/** Returns a new Version object with bumpMinor() applied.
	    @see bumpMinor()
	 */
	[[nodiscard]] Version withMinorBump() const;

	/** Returns a new Version object with bumpPatch() applied.
	    @see bumpPatch()
	 */
	[[nodiscard]] Version withPatchBump() const;

	/** Returns a Version object representing the version of Juce this module was compiled with. */
	[[nodiscard]] static Version juceVersion();
    
    /** Returns a Version object representing the version of Lemons this module was compiled with. */
    [[nodiscard]] static Version lemonsVersion();

private:
	int major { 0 }, minor { 0 }, patch { 1 };
};

}  // namespace lemons
