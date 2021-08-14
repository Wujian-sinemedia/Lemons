#pragma once

namespace bav
{
struct Version : SerializableData
{
    Version() = default;
    Version (int maj, int min, int p);

    bool operator== (const Version& other) const noexcept;
    bool operator!= (const Version& other) const noexcept;

    bool operator> (const Version& other) const noexcept;
    bool operator< (const Version& other) const noexcept;

    String getAsString() const noexcept;

    void bumpMajor() noexcept;
    void bumpMinor() noexcept;
    void bumpPatch() noexcept;

    Version withMajorBump() const;
    Version withMinorBump() const;
    Version withPatchBump() const;

private:
    void serialize (TreeReflector& ref) final;

    int major {0}, minor {0}, patch {0};
};

}  // namespace bav
