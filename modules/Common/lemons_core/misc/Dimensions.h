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


#pragma once

namespace lemons
{


/** A simple struct that represents the dimensions of a GUI. */
struct Dimensions final
{
	/** Creates an invalid Dimensions object. */
	constexpr explicit Dimensions() = default;

	/** Creates a valid Dimensions object with a specified width and height. */
	constexpr explicit Dimensions (int widthToUse, int heightToUse)
	    : width (widthToUse)
	    , height (heightToUse)
	{
	}

	/** Returns true if the passed Dimensions object is equal to this one. */
	[[nodiscard]] constexpr bool operator== (const Dimensions& other) const noexcept
	{
		return width == other.width && height == other.height;
	}

	/** Returns true if the width and height are both greater than 0. */
	[[nodiscard]] constexpr bool isValid() const noexcept
	{
		return width > 0 && height > 0;
	}

	/** Returns the width of these dimensions. */
	[[nodiscard]] constexpr int getWidth() const noexcept
	{
		return width;
	}

	/** Returns the height of these dimensions. */
	[[nodiscard]] constexpr int getHeight() const noexcept
	{
		return height;
	}

	/** Returns the aspect ratio of the represented dimensions, calculated as width / height.
	    If isValid() return false, then this function will return 0.
	 */
	[[nodiscard]] constexpr double getAspectRatio() const noexcept
	{
		if (! isValid())
			return 0.;

		return static_cast<double> (width) / static_cast<double> (height);
	}

	/** Returns true if the passed Dimensions object has the same aspect ratio as this one. */
	[[nodiscard]] constexpr bool hasSameAspectRatioAs (const Dimensions& other) const noexcept
	{
		return getAspectRatio() == other.getAspectRatio();
	}

	/** Returns a string representation of these dimensions, eg '400x600'. */
	[[nodiscard]] String toString() const noexcept;

	/** Returns a Dimensions object from a string representation. The string should be formatted like '400x600'. */
	[[nodiscard]] static Dimensions fromString (const String& string);

	/** Returns a Dimensions object representing the size 1060 x 640. */
	[[nodiscard]] static constexpr Dimensions getDefault() { return Dimensions { 1060, 640 }; }

private:
	int width { 0 }, height { 0 };
};

}  // namespace lemons
