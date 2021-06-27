
#pragma once

namespace bav::gui
{
class EditorBase : public juce::AudioProcessorEditor
{
public:
    EditorBase (dsp::ProcessorBase& pbToUse, juce::Point<int> initialSize = {450, 300});

private:
    void initializeSize (int width, int height);
    void paint (juce::Graphics& g) final;
    
    void resized() final;
    virtual void resizeTriggered() { }

    dsp::ProcessorBase& pb;
};


template<typename ContentComponentType>
class PluginEditor : public EditorBase
{
public:
    template<typename... Args>
    PluginEditor (dsp::ProcessorBase& processorToUse, juce::Point<int> initialSize, Args&&... args)
    : EditorBase (processorToUse, initialSize)
    {
        content.reset (new ContentComponentType (std::forward<Args>(args)...));
        addAndMakeVisible (content.get());
    }
    
private:
    void resizeTriggered() final
    {
        content->setBounds (getLocalBounds());
    }
    
    std::unique_ptr<ContentComponentType> content;
};

}  // namespace bav::gui
