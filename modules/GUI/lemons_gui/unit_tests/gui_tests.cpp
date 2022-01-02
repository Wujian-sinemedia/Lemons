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

namespace lemons::tests
{

void fillImageWithRandomPixels (Image& image, juce::Random rng)
{
	for (int x = 0; x < image.getWidth(); ++x)
		for (int y = 0; y < image.getHeight(); ++y)
			image.setPixelAt (x, y, { rng.nextFloat(), rng.nextFloat(), rng.nextFloat(), rng.nextFloat() });
}

bool imagesAreEqual (const Image& image1, const Image& image2)
{
	const auto width  = image1.getWidth();
	const auto height = image1.getHeight();

	if (width != image2.getWidth() || height != image2.getHeight())
		return false;

	for (int x = 0; x < width; ++x)
		for (int y = 0; y < height; ++y)
			if (image1.getPixelAt (x, y) != image2.getPixelAt (x, y))
				return false;

	return true;
}

}  // namespace lemons::tests
