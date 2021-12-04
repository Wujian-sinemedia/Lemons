#pragma once

namespace lemons
{

/** A simple struct that represents the dimensions of a GUI.
 */
struct Dimensions final
{
	/** Creates an invalid Dimensions object. */
	constexpr Dimensions() = default;

	/** Creates a valid Dimensions object with a specified width and height. */
	constexpr Dimensions (int widthToUse, int heightToUse);

	/** Returns true if the width and height are both greater than 0. */
	[[nodiscard]] bool isValid() const noexcept;

	/** Returns the aspect ratio of the represented dimensions, calculated as width / height.
	    If isValid() return false, then this function will return 0.
	 */
	[[nodiscard]] float getAspectRatio() const noexcept;

	/** Returns a Dimensions object representing the size 1060 x 640. */
	static constexpr Dimensions getDefault();

	int width { 0 };
	int height { 0 };
};

}  // namespace lemons
