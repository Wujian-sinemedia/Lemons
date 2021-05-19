
/*
    This file includes some useful data serialization functions.
*/


#ifndef BV_HAS_BINARY_DATA
#define BV_HAS_BINARY_DATA 0
#endif

#if BV_HAS_BINARY_DATA
#include "BinaryData.h"
#endif


namespace bav
{
#if BV_HAS_BINARY_DATA

struct RawData
{
    explicit RawData (const char* fileToFind)
    {
        using namespace BinaryData;

        for (int index = 0; index < namedResourceListSize; ++index)
        {
            auto binaryName = namedResourceList[index];
            auto fileName   = getNamedResourceOriginalFilename (binaryName);

            if (fileName == fileToFind)
            {
                data = BinaryData::getNamedResource (binaryName, size);
                break;
            }
        }

        jassert (data != nullptr); // File not found
    }

    const char* data = nullptr;
    int         size = 0;
};

/** To easily access some binary data:
 @code
    RawData binary {"File.wav"};
    doSomethingWithData (binary.data, binary.size);
 @end-code
 */

#endif /* if BV_HAS_BINARY_DATA */


/*
 Dumps a given image to a File in png format.
 If the file parameter is nonexistant a temp file will be created on the desktop.
 */
static inline void saveImageToFile (const juce::Image& image, juce::File file)
{
    if (!file.exists())
        file = juce::File::getSpecialLocation (juce::File::userDesktopDirectory)
                   .getNonexistentChildFile ("tempImage", ".png");

    juce::PNGImageFormat format;

    if (auto os = file.createOutputStream()) format.writeImageToStream (image, *os);
}


//  THESE FUNCTONS ARE ON MY TO DO LIST...

//static inline juce::Image getImageFromFile (juce::File file)
//{
//    juce::ignoreUnused (file);
//}


template < typename SampleType >
static inline void saveAudioToFile (const juce::AudioBuffer< SampleType > audio,
                                    juce::File                            file)
{
    juce::ignoreUnused (audio, file);
}

//static inline void juce::AudioBuffer<float> getAudioFromFile (juce::File file)
//{
//    juce::ignoreUnused (file);
//}


/**
 Helper function to serialise a system font to a file.
 This is useful if you want to include a custom font in an application so that is
 guarenteed to be avialiable cross-platform.
 
 Once you have the generated file you can include it as BinarayData (as generated
 by ProJucer) and then reload it in a look and feel.
 
 In your LookAndFeel subclass overide getTypeFaceForFont and create a Typeface::Ptr
 to hold the font e.g.
 
 @code
 //==============================================================================
 class FontLookAndFeel : public LookAndFeel
 {
 public:
    const Typeface::Ptr getTypefaceForFont (const Font& font);
 
 private:
    Typeface::Ptr customTypeface;
 }
 @endcode
 
 Then in your constructor reload the font using deserializeFont() and then
 return your custom typeface in getTypefaceForFont if the requested font names match.
 
 @code
 //==============================================================================
 FontLookAndFeel::FontLookAndFeel()
 {
    customTypeface = deserializeFont (BinaryData::Custom_Font, BinaryData::Custom_FontSize);
 }
 //==============================================================================
 const Typeface::Ptr FontLookAndFeel::getTypefaceForFont (const Font& font)
 {
    if (customTypeface != nullptr && font.getTypefaceName() == customTypeface->getName())
        return customTypeface;
 
    return LookAndFeel::getTypefaceForFont (font);
 }
 @endcode
 */
static inline bool serializeFont (const juce::Font& font,
                                  juce::File&       destinationFile,
                                  int               maxNumChars = 127)
{
    destinationFile.deleteFile();
    auto outFileStream = destinationFile.createOutputStream();

    if (outFileStream == nullptr) return false;

    juce::CustomTypeface customTypeface;
    customTypeface.setCharacteristics (font.getTypefaceName(),
                                       font.getAscent(),
                                       font.isBold(),
                                       font.isItalic(),
                                       ' ');
    customTypeface.addGlyphsFromOtherTypeface (*font.getTypeface(), 0, maxNumChars);

    return customTypeface.writeToStream (*outFileStream);
}


static inline juce::Typeface::Ptr deserializeFont (const void* data, size_t dataSize)
{
    std::unique_ptr< juce::MemoryInputStream > fontStream (
        new juce::MemoryInputStream (data, dataSize, false));

    if (fontStream == nullptr) return nullptr;

    return new juce::CustomTypeface (*fontStream);
}


} // namespace bav
