
#pragma once

namespace lemons::plugin
{
/** Base class for plugin editors.
 */
class EditorBase : public juce::AudioProcessorEditor
{
public:
    /** Creates a plugin editor with an initial size. */
    EditorBase (ProcessorBase& pbToUse, juce::Point< int > initialSize = {450, 300});

private:
    void initializeSize (int width, int height);
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
template < typename ContentComponentType, typename StateType,
           BV_MUST_INHERIT_FROM (ContentComponentType, GUIBase), BV_MUST_INHERIT_FROM (StateType, StateBase) >
class PluginEditor : public EditorBase
{
public:
    /** Creates a plugin editor. */
    PluginEditor (ProcessorBase& processorToUse, PluginState< StateType >& state)
        : EditorBase (processorToUse, state.state.dimensions), content (state)
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
