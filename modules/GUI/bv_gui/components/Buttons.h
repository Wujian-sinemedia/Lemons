#pragma once

namespace bav::gui
{

struct TextButton : juce::TextButton
{
    TextButton (const String& buttonText, std::function< void() > whenClicked, const String& tooltip = {});
};

}
