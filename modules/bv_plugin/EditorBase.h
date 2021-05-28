
#pragma once

namespace bav::gui
{

class EditorBase  :     public juce::AudioProcessorEditor
{
public:
    EditorBase (dsp::ProcessorBase& pbToUse)
        : AudioProcessorEditor (pbToUse), pb (pbToUse)
    {
        const auto size  = pb.getSavedEditorSize();
        const auto width = size.x, height = size.y;
        setResizable (true, true);
        getConstrainer()->setMinimumSize (width / 2, height / 2);
        getConstrainer()->setFixedAspectRatio ((float) width / (float) height);
        setSize (width, height);
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
