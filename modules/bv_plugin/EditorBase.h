
#pragma once

namespace bav::gui
{

class EditorBase  :     public juce::AudioProcessorEditor
{
public:
    EditorBase (dsp::ProcessorBase& pbToUse)
        : AudioProcessorEditor (pbToUse), pb (pbToUse)
    {
        int width, height;
        pb.getSavedEditorSize (width, height);
        
        setResizable (true, true);
        
        if (width > 0 && height > 0)
        {
            getConstrainer()->setMinimumSize (width / 2, height / 2);
            getConstrainer()->setFixedAspectRatio ((float) width / (float) height);
            setSize (width, height);
        }
        else
        {
            setSize (450, 300);
        }
    }
    
    virtual ~EditorBase() override = default;
    
private:
    void resized() override
    {
        pb.saveEditorSize (getWidth(), getHeight());
        resizeTriggered();
    }
    
    virtual void resizeTriggered() { }
    
    dsp::ProcessorBase& pb;
};

}  // namespace
