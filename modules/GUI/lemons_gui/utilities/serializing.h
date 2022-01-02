#pragma once

#include <juce_graphics/juce_graphics.h>
#include <lemons_core/lemons_core.h>

namespace lemons::serializing
{

using juce::File;
using juce::Image;
using juce::juce_wchar;
using juce::MemoryBlock;

/** @defgroup lemons_gui lemons_gui
    @ingroup GUI
    GUI building blocks.
 */


/** @ingroup lemons_gui lemons_serializing
    Encodes an Image object as a blob of binary data. Internally, this uses JUCE's PNGImageFormat.
    @see imageFromBinary(), saveImageToFile()
 */
[[nodiscard]] MemoryBlock imageToBinary (const Image& image);

/** @ingroup lemons_gui lemons_serializing
    Returns an Image object from a binary representation of one. Internally, this uses JUCE's PNGImageFormat.
    @see imageToBinary(), loadImageFromFile(), binary::Data::getImage()
 */
[[nodiscard]] Image imageFromBinary (const MemoryBlock& block);


/** @ingroup lemons_gui lemons_serializing
 */
[[nodiscard]] MemoryBlock fontToBinary (const juce::Font& font, int maxNumChars = 127, juce_wchar defaultChar = ' ');

/** @ingroup lemons_gui lemons_serializing
 */
[[nodiscard]] std::unique_ptr<juce::CustomTypeface> fontFromBinary (const MemoryBlock& block);

}  // namespace lemons::serializing


namespace lemons::files
{

using juce::Image;

/** @ingroup lemons_gui lemons_files
    Saves an Image to a File.
    @see loadImageFromFile(), imageToBinary()
 */
bool saveImage (const Image& image, const File& file);

/** @ingroup lemons_gui lemons_files
    Loads an Image from a File.
    @see saveImageToFile(), imageFromBinary(),  binary::Data::getImage()
 */
[[nodiscard]] Image loadImage (const File& file);

/** @ingroup lemons_gui lemons_files
 */
bool saveFont (const juce::Font& font, const File& file, int maxNumChars = 127, juce_wchar defaultChar = ' ');

/** @ingroup lemons_gui lemons_files
 */
[[nodiscard]] std::unique_ptr<juce::CustomTypeface> loadFont (const File& file);

}


namespace lemons::binary
{

using juce::String;

/** @ingroup lemons_gui lemons_binary
    Returns an image object from an image file in the BinaryData target.
    If the image can't be loaded, an assertion will be thrown.
    @see serializing::loadImageFromFile(), serializing::saveImageToFile()
 */
[[nodiscard]] juce::Image getImage (const String& imageFileName);

/** @ingroup lemons_gui lemons_binary
    Returns a font that has been previously serialized with the serializing::serializeFont() method.
    If the font can't be loaded, an assertion will be thrown.
    @see serializing::serializeFont()
 */
[[nodiscard]] std::unique_ptr<juce::CustomTypeface> getFont (const String& filename);

}

/*---------------------------------------------------------------------------------------------------------------------------*/

namespace juce
{

template <>
struct VariantConverter<Colour>
{
	static Colour fromVar (const var& v);
	static var    toVar (const Colour& c);
};

template <>
struct VariantConverter<Image>
{
	static Image fromVar (const var& v);
	static var   toVar (const Image& i);
};

template <>
struct VariantConverter<Justification>
{
	static Justification fromVar (const var& v);
	static var           toVar (const Justification& j);
};

}  // namespace juce

/*---------------------------------------------------------------------------------------------------------------------------*/

#if LEMONS_UNIT_TESTS

namespace lemons::tests
{

struct GuiSerializingTests : public CoreTest
{
	GuiSerializingTests();

private:
	void runTest() final;
};

static GuiSerializingTests guiSerializingTest;

}  // namespace lemons::tests

#endif
