#pragma once

#include <lemons_spline/common/Common.h>

namespace lemons::spline
{
struct Spline : SerializableData
{
	void updatePoints (const juce::Rectangle<float>& bounds);

	float getPoint (int index) const;

	void resize (int newNumPoints);

	Knots  knots;
	Points points;

private:
	void serialize (TreeReflector& ref) final;
};

}  // namespace lemons::spline
