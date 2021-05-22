
#pragma once

namespace bav::gui
{

class EditorBase  :     private GUIInitializer,
                        public juce::AudioProcessorEditor
{
public:
    EditorBase(ProcessorBase& b)
    : GUIInitializer (*getTopLevelComponent()),
    AudioProcessorEditor (b),
    processorBase (b)
    {
        setInterceptsMouseClicks (false, true);
        
#if JUCE_MAC
        darkMode = juce::Desktop::isOSXDarkModeActive();
#endif
        
        const auto size = processorBase.getSavedEditorSize();
        const auto width = size.x, height = size.y;
        setResizable (true, true);
        getConstrainer()->setMinimumSize (width / 2, height / 2);
        getConstrainer()->setFixedAspectRatio ((float) width / (float) height);
        setSize (width, height);
    }
    
    virtual ~EditorBase() override { }
    
    
    void resized() override final
    {
        processorBase.saveEditorSize (getWidth(), getHeight());
        resizeTriggered (getLocalBounds());
    }
    
protected:
    virtual inline void resizeTriggered (juce::Rectangle<int> area) { juce::ignoreUnused (area); }
    
    ProcessorBase& processorBase;
    
    bool darkMode = true;
};

}  // namespace
