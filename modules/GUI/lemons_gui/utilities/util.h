/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 * ======================================================================================
 */

#pragma once

namespace lemons::gui::util
{

using juce::Component;

static inline void addAndMakeVisible (Component& parent, Component& child)
{
	parent.addAndMakeVisible (child);
}

template <typename... Args>
static inline void addAndMakeVisible (Component& parent, Component& firstChild, Args&&... rest)
{
	addAndMakeVisible (parent, firstChild);
	addAndMakeVisible (parent, std::forward<Args> (rest)...);
}

}  // namespace lemons::gui::util

namespace lemons::gui::components
{

using juce::Component;

template <typename ComponentType, LEMONS_MUST_INHERIT_FROM (ComponentType, Component)>
struct ComponentArray
{
	void addAllTo (Component& parent)
	{
		for (auto* comp : components)
			parent.addAndMakeVisible (comp);
	}

	template <typename... Args>
	void createAndAddTo (Component& parent, Args&&... args)
	{
		auto* newComp = components.add (new ComponentType (std::forward<Args> (args)...));
		parent.addAndMakeVisible (newComp);
	}

	juce::OwnedArray<ComponentType> components;
};

}  // namespace lemons::gui::components
