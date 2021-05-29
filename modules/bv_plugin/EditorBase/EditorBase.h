
#pragma once

namespace bav::gui
{

class EditorBase  :     public juce::AudioProcessorEditor
{
public:
    EditorBase (dsp::ProcessorBase& pbToUse);
    virtual ~EditorBase() override = default;
    
private:
    void resized() override final;
    
    virtual void resizeTriggered() = 0;
    
    dsp::ProcessorBase& pb;
};

}  // namespace
