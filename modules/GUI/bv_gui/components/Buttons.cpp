
namespace bav::gui
{

TextButton::TextButton (const String& buttonText, std::function< void() > whenClicked, const String& tooltip)
: juce::TextButton (buttonText, tooltip)
{
    onClick = std::move (whenClicked);
    
    this->setName (buttonText);
    this->setComponentID (buttonText);
}


}
