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

#include <juce_graphics/juce_graphics.h>

namespace lemons::tests
{

using juce::Image;

void fillImageWithRandomPixels (Image& image, juce::Random rng);

bool imagesAreEqual (const Image& image1, const Image& image2);


template <typename Type>
[[nodiscard]] inline Type toVarAndBack (const Type& orig)
{
	using Converter = juce::VariantConverter<Type>;

	return Converter::fromVar (Converter::toVar (orig));
}


}  // namespace lemons::tests
