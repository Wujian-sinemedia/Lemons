/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 * ======================================================================================
 */

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

#if LEMONS_HAS_BINARY_DATA
	jassert (d.isValid());
#endif

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
	juce::MemoryInputStream is { getBlob (fontFileName), false };

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

Font VariantConverter<Font>::fromVar (const var& v)
{
	Font f;

	if (const auto* obj = v.getDynamicObject())
	{
		if (obj->hasProperty (NAME_PROP))
			f.setTypefaceName (obj->getProperty (NAME_PROP).toString());

		if (obj->hasProperty (STYLE_FLAGS_PROP))
			f.setStyleFlags ((int) obj->getProperty (STYLE_FLAGS_PROP));  // NOLINT

		if (obj->hasProperty (HEIGHT_PROP))
			f.setHeight ((float) obj->getProperty (HEIGHT_PROP));  // NOLINT

		if (obj->hasProperty (HORIZONTAL_SCALE_PROP))
			f.setHorizontalScale ((float) obj->getProperty (HORIZONTAL_SCALE_PROP));  // NOLINT

		if (obj->hasProperty (EXTRA_KERNING_FACTOR_PROP))
			f.setExtraKerningFactor ((float) obj->getProperty (EXTRA_KERNING_FACTOR_PROP));	 // NOLINT
	}

	return f;
}

var VariantConverter<Font>::toVar (const Font& f)
{
	DynamicObject obj;

	obj.setProperty (NAME_PROP, f.getTypefaceName());
	obj.setProperty (STYLE_FLAGS_PROP, f.getStyleFlags());
	obj.setProperty (HEIGHT_PROP, f.getHeight());
	obj.setProperty (HORIZONTAL_SCALE_PROP, f.getHorizontalScale());
	obj.setProperty (EXTRA_KERNING_FACTOR_PROP, f.getExtraKerningFactor());

	return { obj.clone().get() };
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

RectanglePlacement VariantConverter<RectanglePlacement>::fromVar (const var& v)
{
	return { (int) v };	 // NOLINT
}

var VariantConverter<RectanglePlacement>::toVar (const RectanglePlacement& r)
{
	return { r.getFlags() };
}

}  // namespace juce
