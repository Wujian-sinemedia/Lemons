namespace lemons::serializing
{

Image imageFromBinary (const MemoryBlock& block)
{
	juce::MemoryInputStream stream { block, false };
	juce::PNGImageFormat    format;

	return format.decodeImage (stream);
}

MemoryBlock imageToBinary (const Image& image)
{
	MemoryBlock              block;
	juce::MemoryOutputStream stream { block, false };
	juce::PNGImageFormat     format;

	format.writeImageToStream (image, stream);

	return block;
}

bool saveImageToFile (const Image& image, const File& file)
{
	return files::saveBlockToFile (imageToBinary (image), file);
}

Image loadImageFromFile (const File& file)
{
	return imageFromBinary (files::loadFileAsBlock (file));
}


MemoryBlock fontToBinary (const juce::Font& font, int maxNumChars, juce_wchar defaultChar)
{
    MemoryBlock output;
    
    juce::MemoryOutputStream os { output, false };
    
    juce::CustomTypeface customTypeface;
    
    customTypeface.setCharacteristics (font.getTypefaceName(), font.getAscent(),
                                       font.isBold(), font.isItalic(), defaultChar);
    
    customTypeface.addGlyphsFromOtherTypeface (*font.getTypefacePtr(), 0, maxNumChars);
    
    customTypeface.writeToStream (os);
    
    return output;
}

std::unique_ptr<juce::CustomTypeface> fontFromBinary (const MemoryBlock& block)
{
    juce::MemoryInputStream is { block, false };
    
    return std::make_unique<juce::CustomTypeface> (is);
}


}  // namespace lemons::serializing

namespace lemons::files
{

bool saveImage (const Image& image, const File& file)
{
    return saveBlockToFile (serializing::imageToBinary (image), file);
}

Image loadImage (const File& file)
{
    return serializing::imageFromBinary (loadFileAsBlock (file));
}


bool saveFont (const juce::Font& font, const File& file, int maxNumChars, juce_wchar defaultChar)
{
    return saveBlockToFile (serializing::fontToBinary (font, maxNumChars, defaultChar), file);
}

std::unique_ptr<juce::CustomTypeface> loadFont (const File& file)
{
    return serializing::fontFromBinary (loadFileAsBlock (file));
}

}

/*---------------------------------------------------------------------------------------------------------------------------*/

namespace lemons::binary
{

juce::Image getImage (const String& imageFileName)
{
    const Data d { imageFileName };
    
    jassert (d.isValid());
    
    return juce::ImageCache::getFromMemory (d.getData(), d.getSize());
}

std::unique_ptr<juce::CustomTypeface> getFont (const String& fontFileName)
{
    const auto block = getBlob (fontFileName);
    
    juce::MemoryInputStream is { block, false };
    
    return std::make_unique<juce::CustomTypeface> (is);
}

}

/*---------------------------------------------------------------------------------------------------------------------------*/

namespace juce
{

using namespace lemons::serializing;

Colour VariantConverter<Colour>::fromVar (const var& v)
{
	return Colour::fromString (v.toString());
}

var VariantConverter<Colour>::toVar (const Colour& c)
{
	return { c.toString() };
}

Image VariantConverter<Image>::fromVar (const var& v)
{
	return imageFromBinary (memoryBlockFromString (v.toString()));
}

var VariantConverter<Image>::toVar (const Image& i)
{
	return { memoryBlockToString (imageToBinary (i)) };
}

Justification VariantConverter<Justification>::fromVar (const var& v)
{
	return { (int) v };
}

var VariantConverter<Justification>::toVar (const Justification& j)
{
	return { j.getFlags() };
}

}  // namespace juce

/*---------------------------------------------------------------------------------------------------------------------------*/

#if LEMONS_UNIT_TESTS

namespace lemons::tests
{

GuiSerializingTests::GuiSerializingTests()
    : CoreTest ("GUI Serializing")
{
}

void GuiSerializingTests::runTest()
{
	Image image { juce::Image::PixelFormat::RGB, 250, 250, true };

	fillImageWithRandomPixels (image, getRandom());

	const auto blob         = serializing::imageToBinary (image);
	const auto decodedImage = serializing::imageFromBinary (blob);

	//    expect (imagesAreEqual (decodedImage, image));

	auto rand = getRandom();

	{
		const auto subtest = beginSubtest ("Colour");

		const juce::Colour orig { rand.nextFloat(), rand.nextFloat(), rand.nextFloat(), rand.nextFloat() };

		expect (orig == toVarAndBack (orig));
	}

	{
		const auto subtest = beginSubtest ("Image");

		Image orig { juce::Image::PixelFormat::RGB, 250, 250, true };

		fillImageWithRandomPixels (orig, rand);

		//        expect (imagesAreEqual (orig, toVarAndBack (orig)));
	}

	{
		const auto subtest = beginSubtest ("Justification");

		using juce::Justification;

		const Justification orig { Justification::Flags::left };

		expect (orig == toVarAndBack (orig));
	}
}

}  // namespace lemons::tests

#endif
