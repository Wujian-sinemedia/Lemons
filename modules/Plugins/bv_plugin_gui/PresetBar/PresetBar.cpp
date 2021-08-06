
namespace bav::plugin
{
PresetBar::PresetBar (StatePackage& stateToUse, const String& productName, const String& presetFileExtension, const String& companyName)
    : presetManager (stateToUse.get(), companyName, productName, presetFileExtension), toggleButtons (stateToUse.getToggler())
{
    gui::addAndMakeVisible (this, toggleButtons);
}

}  // namespace bav::plugin
