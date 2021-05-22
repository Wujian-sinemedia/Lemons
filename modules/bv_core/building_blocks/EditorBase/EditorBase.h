
#pragma once

namespace bav::gui
{

class EditorBase  :     private GUIInitializer,
                        public juce::AudioProcessorEditor
{
public:
    EditorBase(ProcessorBase& b);
    virtual ~EditorBase() override;
    
    void resized() override final;
    
protected:
    virtual void resizeTriggered (juce::Rectangle<int> area);
    
    ProcessorBase& processorBase;
    
    bool darkMode = true;
};

}  // namespace
