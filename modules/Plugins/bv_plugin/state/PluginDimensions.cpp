
namespace bav::plugin
{
void Dimensions::serialize (TreeReflector& ref)
{
    ref.add ("Width", width);
    ref.add ("Height", height);
}

Dimensions& Dimensions::operator= (const juce::Point< int >& newSize) noexcept
{
    width  = newSize.x;
    height = newSize.y;

    return *this;
}

juce::Point< int > Dimensions::get() const
{
    return {width, height};
}

}  // namespace bav::plugin
