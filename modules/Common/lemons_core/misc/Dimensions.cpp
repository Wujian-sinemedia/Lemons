/*
 ======================================================================================

 ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝

 This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.

 ======================================================================================
 */


namespace lemons
{

constexpr bool Dimensions::operator== (const Dimensions& other) const noexcept
{
	return width == other.width && height == other.height;
}

constexpr bool Dimensions::isValid() const noexcept
{
	return width > 0 && height > 0;
}

constexpr int Dimensions::getWidth() const noexcept
{
	return width;
}

constexpr int Dimensions::getHeight() const noexcept
{
	return height;
}

constexpr double Dimensions::getAspectRatio() const noexcept
{
	if (! isValid())
		return 0.;

	return static_cast<double> (width) / static_cast<double> (height);
}

constexpr bool Dimensions::hasSameAspectRatioAs (const Dimensions& other) const noexcept
{
	return getAspectRatio() == other.getAspectRatio();
}

String Dimensions::toString() const noexcept
{
	return String (width) + "x" + String (height);
}

Dimensions Dimensions::fromString (const String& string)
{
	const auto arr = juce::StringArray::fromTokens (string, "x", "");

	jassert (arr.size() == 2);

	return Dimensions { arr[0].getIntValue(), arr[1].getIntValue() };
}

}  // namespace lemons
