namespace lemons::gui
{

bool serializeFont (const juce::Font& font, File& destFile, int maxNumChars, juce_wchar defaultChar)
{
    destFile.deleteFile();
    
    auto outStream = destFile.createOutputStream();
    
    if (outStream.get() == nullptr)
        return false;
    
    juce::CustomTypeface customTypeface;
    
    customTypeface.setCharacteristics (font.getTypefaceName(), font.getAscent(),
                                       font.isBold(), font.isItalic(), defaultChar);
    
    customTypeface.addGlyphsFromOtherTypeface (*font.getTypefacePtr(), 0, maxNumChars);
    
    return customTypeface.writeToStream (*outStream);
}

}

