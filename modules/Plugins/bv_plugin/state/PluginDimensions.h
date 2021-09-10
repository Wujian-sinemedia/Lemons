#pragma once

namespace bav::plugin
{
struct Dimensions : SerializableData
{
    using SerializableData::SerializableData;

    Dimensions& operator= (const juce::Point< int >& newSize) noexcept;

    void serialize (TreeReflector& ref) final;

    juce::Point< int > get() const;

    int w() const;
    int h() const;

private:
    int width {0}, height {0};
};

}  // namespace bav::plugin
