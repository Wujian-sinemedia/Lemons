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

/** A simple struct that represents the dimensions of a GUI.
 */
struct Dimensions final
{
	/** Creates an invalid Dimensions object. */
	constexpr explicit Dimensions() = default;

	/** Creates a valid Dimensions object with a specified width and height. */
	constexpr explicit Dimensions (int widthToUse, int heightToUse);

	/** Returns true if the passed Dimensions object is equal to this one. */
	[[nodiscard]] constexpr bool operator== (const Dimensions& other) const noexcept;

	/** Returns true if the width and height are both greater than 0. */
	[[nodiscard]] constexpr bool isValid() const noexcept;

	/** Returns the aspect ratio of the represented dimensions, calculated as width / height.
	    If isValid() return false, then this function will return 0.
	 */
	[[nodiscard]] constexpr float getAspectRatio() const noexcept;

	/** Returns true if the passed Dimensions object has the same aspect ratio as this one. */
	[[nodiscard]] constexpr bool hasSameAspectRatioAs (const Dimensions& other) const noexcept;

	/** Returns a string representation of these dimensions, eg '400x600'. */
	[[nodiscard]] String toString() const noexcept;

	/** Returns a Dimensions object from a string representation. The string should be formatted like '400x600'. */
	[[nodiscard]] static Dimensions fromString (const String& string);

	/** Returns a Dimensions object representing the size 1060 x 640. */
	[[nodiscard]] static constexpr Dimensions getDefault();

	int width { 0 };
	int height { 0 };
};

}  // namespace lemons
