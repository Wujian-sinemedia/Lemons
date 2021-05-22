
#pragma once

namespace bav::gui
{

class EditorBase  :     private GUIInitializer,
                        public juce::AudioProcessorEditor
{
public:
    EditorBase(dsp::ProcessorBase& b);
    virtual ~EditorBase() override;
    
    void resized() override final;
    
protected:
    virtual void resizeTriggered (juce::Rectangle<int> area);
    
    dsp::ProcessorBase& processorBase;
    
    bool darkMode = true;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProcessorBase)
};

}  // namespace
