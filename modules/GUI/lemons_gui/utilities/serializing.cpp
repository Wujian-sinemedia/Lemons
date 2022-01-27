namespace lemons::serializing
{

Image imageFromBinary (const MemoryBlock& block)
{
	juce::MemoryInputStream stream { block, false };
	juce::PNGImageFormat	format;

	return format.decodeImage (stream);
}

MemoryBlock imageToBinary (const Image& image)
{
	MemoryBlock				 block;
	juce::MemoryOutputStream stream { block, false };
	juce::PNGImageFormat	 format;

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

/*---------------------------------------------------------------------------------------------------------------------------*/

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


static const juce::StringArray& getImageFileExtensions()
{
	// retrieving valid image file extensions isn't currently supported in the JUCE API, so hard code them for now.....
	static const juce::StringArray imageXtns { ".jpg", ".jpeg", ".png", ".gif" };

	return imageXtns;
}

bool isImageFile (const File& file)
{
	for (const auto& xtn : getImageFileExtensions())
		if (file.hasFileExtension (xtn))
			return true;

	return false;
}

}  // namespace lemons::files

/*---------------------------------------------------------------------------------------------------------------------------*/

namespace lemons::binary
{

juce::Image getImage (const String& imageFileName)
{
	const Data d { imageFileName };

	jassert (d.isValid());

	return juce::ImageCache::getFromMemory (d.getData(), d.getSize());
}

juce::StringArray getImageFileNames()
{
	juce::StringArray imageFilenames;

	for (const auto& filename : getFilenames())
	{
		const auto isImage = [&filename]
		{
			for (const auto& xtn : files::getImageFileExtensions())
				if (filename.endsWith (xtn))
					return true;

			return false;
		}();

		if (isImage)
			imageFilenames.add (filename);
	}

	return imageFilenames;
}

std::unique_ptr<juce::CustomTypeface> getFont (const String& fontFileName)
{
	const auto block = getBlob (fontFileName);

	juce::MemoryInputStream is { block, false };

	return std::make_unique<juce::CustomTypeface> (is);
}

}  // namespace lemons::binary

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
	return { (int) v };	 // NOLINT
}

var VariantConverter<Justification>::toVar (const Justification& j)
{
	return { j.getFlags() };
}

}  // namespace juce
