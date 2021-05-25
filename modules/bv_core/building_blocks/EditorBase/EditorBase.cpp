
namespace bav::gui
{

EditorBase::EditorBase(dsp::ProcessorBase& b)
: GUIInitializer (getTopLevelComponent()),
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

EditorBase::~EditorBase() { }


}  //namespace
