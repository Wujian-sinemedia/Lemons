#pragma once

namespace lemons::gui
{

using juce::File;
using juce::juce_wchar;

/** Serializes a font to a file that can then be reloaded as a BinaryData file.
 */
bool serializeFont (const juce::Font& font, const File& destFile, int maxNumChars = 127, juce_wchar defaultChar = ' ');

}
