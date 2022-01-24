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

/** @ingroup lemons_plugin_editors
	Base class for plugin editors' GUI content components.
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


/** @ingroup lemons_plugin_editors
	Base class for a plugin editor that simply holds a specified GUI component.
	@tparam ContentComponentType The type of your plugin's main component. This type must inherit from GUI<StateType>.
	@tparam StateType The type of the plugin's state object. This type must inherit from State.
 */
template <typename ContentComponentType,
	LEMONS_MUST_INHERIT_FROM (ContentComponentType, juce::Component)>
class Editor final : public juce::AudioProcessorEditor
{
public:

	/** Creates a plugin editor. */
	template <typename StateType, LEMONS_MUST_INHERIT_FROM (StateType, State)>
	explicit Editor (ProcessorBase& p, StateType& state, bool resizable = true, bool useDraggableCorner = true, int msBeforeTooltip = 700, const Dimensions& defaultInitialSize = Dimensions::getDefault())
		: AudioProcessorEditor (p), content (state), stateBase (p.getState()), tooltipWindow (this, msBeforeTooltip), msBeforeTooltip_ (msBeforeTooltip)
	{
		const auto& initialSize = [&]() -> const Dimensions&
		{
			if (stateBase.editorSize.isValid())
				return stateBase.editorSize;

			return defaultInitialSize;
		}();

		jassert (initialSize.isValid());

		if (! resizable) useDraggableCorner = false;

		setResizable (resizable, useDraggableCorner);

		const auto width  = initialSize.getWidth();
		const auto height = initialSize.getHeight();

		if (resizable)
		{
			if (auto* c = getConstrainer())
			{
				c->setMinimumSize (width / 2, height / 2);
				c->setMaximumSize (width * 2, height * 2);
				c->setFixedAspectRatio (initialSize.getAspectRatio());
			}
		}

		addAndMakeVisible (content);

		setSize (width, height);
	}

	template <typename StateType, LEMONS_MUST_INHERIT_FROM (StateType, State)>
	explicit Editor (ProcessorBase& p, StateType& state, const EditorAttributes& attributes)
		: Editor (p, state, attributes.isResizable, attributes.useResizableCorner, attributes.msBeforeTooltip, attributes.initialSize)
	{
	}

	[[nodiscard]] EditorAttributes getAttributes() const
	{
		EditorAttributes attributes;

		attributes.initialSize.set (getWidth(), getHeight());
		attributes.isResizable		  = isResizable();
		attributes.useResizableCorner = resizableCorner.get() != nullptr;
		attributes.msBeforeTooltip	  = msBeforeTooltip_;

		return attributes;
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

	juce::TooltipWindow tooltipWindow;
	const int			msBeforeTooltip_;
};

}  // namespace lemons::plugin
