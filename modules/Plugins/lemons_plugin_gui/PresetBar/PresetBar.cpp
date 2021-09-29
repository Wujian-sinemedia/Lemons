
namespace lemons::plugin
{
PresetBar::PresetBar (StateBase&    stateToUse,
                      StateToggler& toggler,
                      const String& productName,
                      const String& presetFileExtension,
                      const String& companyName)
    : presetLibrary (stateToUse, companyName, productName, presetFileExtension),
      toggleButtons (toggler)
{
    gui::addAndMakeVisible (this, toggleButtons);
}

}  // namespace lemons::plugin
