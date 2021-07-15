
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
           std::enable_if_t< std::is_base_of< juce::Component, ContentComponentType >::value >* = nullptr >
class PluginEditor : public EditorBase
{
public:
    template < typename... Args >
    PluginEditor (ProcessorBase& processorToUse, juce::Point< int > initialSize, Args&&... args)
        : EditorBase (processorToUse, initialSize), content (std::forward< Args > (args)...)
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
