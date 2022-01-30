/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 *  ======================================================================================
 */

namespace lemons
{

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

String Version::toString() const noexcept
{
	return String (major) + "." + String (minor) + "." + String (patch);
}

Version Version::fromString (const String& string)
{
	const auto arr = juce::StringArray::fromTokens (string, ".", "");

	jassert (arr.size() == 3);

	return Version { arr[0].getIntValue(), arr[1].getIntValue(), arr[2].getIntValue() };
}

}  // namespace lemons
