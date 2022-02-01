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

static constexpr auto font_typeface_name_prop		 = "Typeface_name";
static constexpr auto font_style_flags_prop			 = "Style_flags";
static constexpr auto font_height_prop				 = "Height";
static constexpr auto font_horizontal_scale_prop	 = "Horizontal_scale";
static constexpr auto font_extra_kerning_factor_prop = "Extra_kerning_factor";

Font VariantConverter<Font>::fromVar (const var& v)
{
	Font f;

	if (const auto* obj = v.getDynamicObject())
	{
		if (obj->hasProperty (font_typeface_name_prop))
			f.setTypefaceName (obj->getProperty (font_typeface_name_prop).toString());

		if (obj->hasProperty (font_style_flags_prop))
			f.setStyleFlags ((int) obj->getProperty (font_style_flags_prop));  // NOLINT

		if (obj->hasProperty (font_height_prop))
			f.setHeight ((float) obj->getProperty (font_height_prop));	// NOLINT

		if (obj->hasProperty (font_horizontal_scale_prop))
			f.setHorizontalScale ((float) obj->getProperty (font_horizontal_scale_prop));  // NOLINT

		if (obj->hasProperty (font_extra_kerning_factor_prop))
			f.setExtraKerningFactor ((float) obj->getProperty (font_extra_kerning_factor_prop));  // NOLINT
	}

	return f;
}

var VariantConverter<Font>::toVar (const Font& f)
{
	DynamicObject obj;

	obj.setProperty (font_typeface_name_prop, f.getTypefaceName());
	obj.setProperty (font_style_flags_prop, f.getStyleFlags());
	obj.setProperty (font_height_prop, f.getHeight());
	obj.setProperty (font_horizontal_scale_prop, f.getHorizontalScale());
	obj.setProperty (font_extra_kerning_factor_prop, f.getExtraKerningFactor());

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

}  // namespace juce
