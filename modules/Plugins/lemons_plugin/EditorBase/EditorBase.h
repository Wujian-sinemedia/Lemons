
#pragma once

namespace lemons::plugin
{
/** Base class for plugin editors.
 */
class EditorBase : public juce::AudioProcessorEditor
{
public:
	/** Creates a plugin editor with an initial size. */
	EditorBase (ProcessorBase& pbToUse, const juce::Point<int>& initialSize = defaultWindowSize());

private:
	void paint (juce::Graphics& g) final;

	void resized() final;

	/** Called when the editor is resized. */
	virtual void resizeTriggered() { }

	ProcessorBase& pb;
};


/** Base class for a plugin editor that instantiates the owned content for you.
    @tparam ContentComponentType The type of your plugin's main component. This must inherit from GUIBase, but should preferably inherit from GUI<StateType>. \n
    This type's constructor signature must be ContentComponentType (PluginState< StateType >&).
    @tparam StateType The type of your plugin's main state object.
 */
template <typename ContentComponentType, typename StateType>//,
          //LEMONS_MUST_INHERIT_FROM (ContentComponentType, GUIBase), LEMONS_MUST_INHERIT_FROM (StateType, StateBase)>
class PluginEditor : public EditorBase
{
public:
	/** Creates a plugin editor. */
	PluginEditor (ProcessorBase& processorToUse, StateType& state)
	    : EditorBase (processorToUse, state.dimensions)
	    , content (state)
	{
		addAndMakeVisible (content);
	}

private:
	void resizeTriggered() final
	{
		content.setBounds (getLocalBounds());
	}

	ContentComponentType content;
};

}  // namespace lemons::plugin
