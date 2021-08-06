
#pragma once

namespace bav::plugin
{
class EditorBase : public juce::AudioProcessorEditor
{
public:
    EditorBase (ProcessorBase& pbToUse, juce::Point< int > initialSize = {450, 300});

private:
    void initializeSize (int width, int height);
    void paint (juce::Graphics& g) final;

    void         resized() final;
    virtual void resizeTriggered() { }

    ProcessorBase& pb;
};


template < typename ContentComponentType,
           BV_MUST_INHERIT_FROM (ContentComponentType, GUI) >
class PluginEditor : public EditorBase
{
public:
    template < typename StateType, BV_MUST_INHERIT_FROM (StateType, StateBase) >
    PluginEditor (ProcessorBase& processorToUse, StateType& state, StateToggler& toggler, UndoManager& undo)
        : EditorBase (processorToUse, state.dimensions.get()),
          content (state, toggler, undo)
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

}  // namespace bav::plugin
