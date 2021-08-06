
namespace bav::plugin
{
PresetBar::PresetBar (StateBase&    stateToUse,
                      StateToggler& toggler,
                      const String& productName,
                      const String& presetFileExtension,
                      const String& companyName)
    : presetManager (stateToUse, companyName, productName, presetFileExtension), toggleButtons (toggler)
{
    gui::addAndMakeVisible (this, toggleButtons);
}

}  // namespace bav::plugin
