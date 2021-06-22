
namespace bav::gui
{
EditorBase::EditorBase (dsp::ProcessorBase& pbToUse)
    : AudioProcessorEditor (pbToUse), pb (pbToUse)
{
    setResizable (true, true);

    const auto size   = pb.getSavedEditorSize();
    const auto width  = size.x;
    const auto height = size.y;

    if (width > 0 && height > 0)
    {
        getConstrainer()->setMinimumSize (width / 2, height / 2);
        getConstrainer()->setMaximumSize (width * 2, height * 2);
        getConstrainer()->setFixedAspectRatio ((float) width / (float) height);
        setSize (width, height);
    }
    else
    {
        jassertfalse;
    }
}

void EditorBase::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void EditorBase::resized()
{
    pb.saveEditorSize (getWidth(), getHeight());
    resizeTriggered();
}

}  // namespace bav::gui
