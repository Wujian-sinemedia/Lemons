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

namespace lemons::plugin
{

/** Base class for plugin editors' GUI content components.
    @tparam StateType The type of the plugin's state object. This type must inherit from State.
 */
template <typename StateType, LEMONS_MUST_INHERIT_FROM (StateType, State)>
class GUI : public juce::Component
{
public:
	explicit GUI (StateType& s)
	    : state (s)
	{
	}

protected:
	StateType& state;
};


/** Base class for a plugin editor that simply holds a specified GUI component.
    @tparam ContentComponentType The type of your plugin's main component. This type must inherit from GUI<StateType>.
    @tparam StateType The type of the plugin's state object. This type must inherit from State.
 */
template <typename ContentComponentType, typename StateType,
          LEMONS_MUST_INHERIT_FROM (ContentComponentType, GUI<StateType>),
          LEMONS_MUST_INHERIT_FROM (StateType, State)>
class Editor final : public juce::AudioProcessorEditor
{
public:
	/** Creates a plugin editor. */
	explicit Editor (ProcessorBase& p, StateType& state)
	    : AudioProcessorEditor (p)
	    , content (state)
	    , stateBase (p.getState())
	{
		const auto& initialSize = stateBase.editorSize;

		jassert (initialSize.isValid());

		setResizable (true, true);

		const auto width  = initialSize.getWidth();
		const auto height = initialSize.getHeight();

		if (auto* c = getConstrainer())
		{
			c->setMinimumSize (width / 2, height / 2);
			c->setMaximumSize (width * 2, height * 2);
			c->setFixedAspectRatio (initialSize.getAspectRatio());
		}

		addAndMakeVisible (content);

		setSize (width, height);
	}

private:
	void resized() final
	{
		const auto bounds = getLocalBounds();
		stateBase.editorSize.set (bounds.getWidth(), bounds.getHeight());
		content.setBounds (bounds);
	}

	void paint (juce::Graphics& g) final
	{
		g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
	}

	ContentComponentType content;

	State& stateBase;

	juce::TooltipWindow tooltipWindow { this, 700 };
};

}  // namespace lemons::plugin
