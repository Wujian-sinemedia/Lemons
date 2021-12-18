#pragma once

#include <juce_graphics/juce_graphics.h>

namespace lemons::serializing
{

using juce::File;
using juce::Image;
using juce::juce_wchar;
using juce::MemoryBlock;

/** Encodes an Image object as a blob of binary data. Internally, this uses JUCE's PNGImageFormat.
    @see imageFromBinary(), saveImageToFile()
 */
[[nodiscard]] MemoryBlock imageToBinary (const Image& image);

/** Returns an Image object from a binary representation of one. Internally, this uses JUCE's PNGImageFormat.
    @see imageToBinary(), loadImageFromFile(), binary::Data::getImage()
 */
[[nodiscard]] Image imageFromBinary (const MemoryBlock& block);

/** Saves an Image to a File.
    @see loadImageFromFile(), imageToBinary()
 */
bool saveImageToFile (const Image& image, const File& file);

/** Loads an Image from a File.
    @see saveImageToFile(), imageFromBinary(),  binary::Data::getImage()
 */
[[nodiscard]] Image loadImageFromFile (const File& file);


/** Serializes a font to a file that can then be reloaded as a BinaryData file.
    @see binary::Data::getFont()
 */
bool serializeFont (const juce::Font& font, const File& destFile, int maxNumChars = 127, juce_wchar defaultChar = ' ');

}  // namespace lemons::serializing

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
