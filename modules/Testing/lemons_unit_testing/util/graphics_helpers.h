#pragma once

#include <juce_graphics/juce_graphics.h>

namespace lemons::tests
{

using juce::Image;

void fillImageWithRandomPixels (Image& image, juce::Random rng);

bool imagesAreEqual (const Image& image1, const Image& image2);

}
