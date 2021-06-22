
#pragma once

namespace bav::gui
{
class EditorBase : public juce::AudioProcessorEditor
{
public:
    EditorBase (dsp::ProcessorBase& pbToUse);

private:
    void paint (juce::Graphics& g) final;
    void resized() final;
    virtual void resizeTriggered() { }

    dsp::ProcessorBase& pb;
    juce::TooltipWindow tooltipWindow {this, 700};
};


template<typename ContentComponentType>
class PluginEditor : public EditorBase
{
public:
    template<typename... Args>
    PluginEditor (dsp::ProcessorBase& processorToUse, Args&&... args)
    : EditorBase (processorToUse)
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
