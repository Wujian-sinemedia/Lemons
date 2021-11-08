#pragma once

#include <juce_graphics/juce_graphics.h>


namespace lemons::spline
{
using Point = juce::Point<float>;

using Points = std::vector<float>;
}  // namespace lemons::spline

#include "Knots.h"
#include "Interpolation.h"
