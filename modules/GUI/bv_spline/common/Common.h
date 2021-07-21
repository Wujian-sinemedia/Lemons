#pragma once

#include <juce_graphics/juce_graphics.h>
#include <bv_serializing/bv_serializing.h>


namespace bav::spline
{
using Point = juce::Point< float >;
}

#include "Knots.h"
#include "Interpolation.h"
