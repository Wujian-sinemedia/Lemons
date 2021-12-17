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

bool serializeFont (const juce::Font& font, const File& destFile, int maxNumChars, juce_wchar defaultChar)
{
	destFile.deleteFile();
    
    if (auto outStream = destFile.createOutputStream())
    {
        if (outStream->openedOk())
        {
            juce::CustomTypeface customTypeface;
            
            customTypeface.setCharacteristics (font.getTypefaceName(), font.getAscent(),
                                               font.isBold(), font.isItalic(), defaultChar);
            
            customTypeface.addGlyphsFromOtherTypeface (*font.getTypefacePtr(), 0, maxNumChars);
            
            return customTypeface.writeToStream (*outStream);
        }
    }
    
    return false;
}


}  // namespace lemons::serializing

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
