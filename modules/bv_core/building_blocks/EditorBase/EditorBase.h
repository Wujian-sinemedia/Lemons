
#pragma once

namespace bav::gui
{

class EditorBase  :     private GUIInitializer,
                        public juce::AudioProcessorEditor
{
public:
    EditorBase(dsp::ProcessorBase& b);
    virtual ~EditorBase() override;
    
    
protected:
    dsp::ProcessorBase& processorBase;
    
    bool darkMode = true;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditorBase)
};

}  // namespace
