#pragma once

namespace lemons::gui
{
struct Label : juce::Label
{
    Label (const String& textToUse = {});

    void set (const String& newText);
};

}  // namespace lemons::gui
