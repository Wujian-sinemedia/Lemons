#pragma once

namespace bav::gui
{
struct TextButton : public juce::TextButton
{
    TextButton (const String& buttonText, std::function< void() > whenClicked, const String& tooltip = {});
};

}  // namespace bav::gui
