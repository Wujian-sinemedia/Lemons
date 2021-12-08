namespace lemons
{

constexpr Dimensions::Dimensions (int widthToUse, int heightToUse)
    : width (widthToUse)
    , height (heightToUse)
{
}

constexpr bool Dimensions::isValid() const noexcept
{
	return width > 0 && height > 0;
}

constexpr float Dimensions::getAspectRatio() const noexcept
{
	if (! isValid())
		return 0.f;

	return static_cast<float> (width) / static_cast<float> (height);
}

String Dimensions::toString() const noexcept
{
	return String (width) + "x" + String (height);
}

Dimensions Dimensions::fromString (const String& string)
{
	const auto arr = juce::StringArray::fromTokens (string, "x", "");

	jassert (arr.size() == 2);

	return { arr[0].getIntValue(), arr[1].getIntValue() };
}

constexpr Dimensions Dimensions::getDefault()
{
	return { 1060, 640 };
}

}  // namespace lemons
