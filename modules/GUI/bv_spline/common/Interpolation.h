#pragma once

namespace bav::spline::interpolation
{
juce::Point< float > lin (const juce::Point< float >& a, const juce::Point< float >& b, float x) noexcept;

juce::Point< float > nurbs (const std::vector< juce::Point< float > >& knots, float t, const int* k) noexcept;

float hermitCubic (const Knots& data, float x) noexcept;

float hermitCubic2 (const Knots& data, float x, float tolerance, int i1) noexcept;

}  // namespace bav::spline::interpolation
