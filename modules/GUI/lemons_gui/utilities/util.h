#pragma once

namespace lemons::gui::util
{

using juce::Component;

static void addAndMakeVisible (Component& parent, Component& child)
{
    parent.addAndMakeVisible (child);
}

template<typename... Args>
static void addAndMakeVisible (Component& parent, Component& firstChild, Args&&... rest)
{
    addAndMakeVisible (parent, firstChild);
    addAndMakeVisible (parent, std::forward<Args>(rest)...);
}

}
