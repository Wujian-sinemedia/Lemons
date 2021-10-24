#pragma once

#include <lemons_core/lemons_core.h>
#include <lemons_mobile_utils/lemons_mobile_utils.h>
#include <juce_gui_extra/juce_gui_extra.h>

namespace lemons::gui
{

/** Scales a component relatively to the bounds of its parent.
    The ratios are provided in the form of a rectangle, ie {x, y, w, h}.
    @param component The component to scale. This component must have a parent assigned.
    @param boundsRatio The ratio to the parent component's bounds to which the child will be scaled. If you pass {0.f, 0.f, 1.f, 1.f}, the scaled component will take up the entire area of its parent. If you pass {0.5f, 0.5f, 0.5f, 0.5f}, the scaled component will be half the area of the parent and will sit in the lower right corner of the parent.
 */
void scale (juce::Component& component, const juce::Rectangle<float>& boundsRatio);


/** Variadic function that allows adding any number of child components to a parent with a single function call.
 */
void addAndMakeVisible (juce::Component* parent, juce::Component& child);


/** Variadic function that allows adding any number of child components to a parent with a single function call.
 */
template <typename... Args>
static inline void addAndMakeVisible (juce::Component* parent, juce::Component& firstChild, Args&&... rest)
{
	addAndMakeVisible (parent, firstChild);
	addAndMakeVisible (parent, std::forward<Args> (rest)...);
}


}  // namespace lemons::gui
