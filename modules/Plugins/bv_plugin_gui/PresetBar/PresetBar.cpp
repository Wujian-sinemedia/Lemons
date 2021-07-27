
namespace bav::plugin
{
PresetBar::PresetBar (State& stateToUse, const String& productName, const String& presetFileExtension, const String& companyName)
    : state (stateToUse), presetManager (state, companyName, productName, presetFileExtension)
{
    gui::addAndMakeVisible (this, toggleButtons);
}

}  // namespace bav::plugin
