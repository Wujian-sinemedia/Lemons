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

/*---------------------------------------------------------------------------------------------------------------------------*/

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


/** Returns true if the file has an extension that matches a known image file format.
 */
[[nodiscard]] bool isImageFile (const File& file);

}  // namespace lemons::files

/*---------------------------------------------------------------------------------------------------------------------------*/

namespace lemons::binary
{

using juce::String;

/** @ingroup lemons_gui lemons_binary
	Returns an image object from an image file in the BinaryData target.
	If the image can't be loaded, an assertion will be thrown.
	@see serializing::loadImageFromFile(), serializing::saveImageToFile()
 */
[[nodiscard]] juce::Image getImage (const String& imageFileName);

/** @ingroup lemons_binary
	Returns a list of names of binary resource files that have file extensions matching known image file formats.
 */
[[nodiscard]] juce::StringArray getImageFileNames();

/** @ingroup lemons_gui lemons_binary
	Returns a font that has been previously serialized with the serializing::serializeFont() method.
	If the font can't be loaded, an assertion will be thrown.
	@see serializing::serializeFont()
 */
[[nodiscard]] std::unique_ptr<juce::CustomTypeface> getFont (const String& filename);

}  // namespace lemons::binary

/*---------------------------------------------------------------------------------------------------------------------------*/

namespace juce
{

template <>
struct VariantConverter<Colour> final
{
	[[nodiscard]] static Colour fromVar (const var& v);
	[[nodiscard]] static var	toVar (const Colour& c);
};

template <>
struct VariantConverter<Font> final
{
	[[nodiscard]] static Font fromVar (const var& v);
	[[nodiscard]] static var  toVar (const Font& f);

private:

	static constexpr auto NAME_PROP					= "Name";
	static constexpr auto STYLE_FLAGS_PROP			= "Style_flags";
	static constexpr auto HEIGHT_PROP				= "Height";
	static constexpr auto HORIZONTAL_SCALE_PROP		= "Horizontal_scale";
	static constexpr auto EXTRA_KERNING_FACTOR_PROP = "Extra_kerning_factor";
};

template <>
struct VariantConverter<Image> final
{
	[[nodiscard]] static Image fromVar (const var& v);
	[[nodiscard]] static var   toVar (const Image& i);
};

template <>
struct VariantConverter<Justification> final
{
	[[nodiscard]] static Justification fromVar (const var& v);
	[[nodiscard]] static var		   toVar (const Justification& j);
};

template <>
struct VariantConverter<RectanglePlacement> final
{
	[[nodiscard]] static RectanglePlacement fromVar (const var& v);
	[[nodiscard]] static var				toVar (const RectanglePlacement& r);
};

#define LEMONS_CREATE_POINT_CONVERTER(Type)                          \
	template <>                                                      \
	struct VariantConverter<Point<Type>> final                       \
	{                                                                \
		using Converter = lemons::serializing::ArrayConverter<Type>; \
		[[nodiscard]] static Point<Type> fromVar (const var& v)      \
		{                                                            \
			const auto arr = Converter::fromVar (v);                 \
			return { arr[0], arr[1] };                               \
		}                                                            \
		[[nodiscard]] static var toVar (const Point<Type>& p)        \
		{                                                            \
			return Converter::toVar ({ p.x, p.y });                  \
		}                                                            \
	};

LEMONS_CREATE_POINT_CONVERTER (int)
LEMONS_CREATE_POINT_CONVERTER (float)
LEMONS_CREATE_POINT_CONVERTER (double)

#undef LEMONS_CREATE_POINT_CONVERTER


#define LEMONS_CREATE_RECT_CONVERTER(Type)                                                 \
	template <>                                                                            \
	struct VariantConverter<Rectangle<Type>> final                                         \
	{                                                                                      \
		using Converter = lemons::serializing::ArrayConverter<Type>;                       \
		[[nodiscard]] static Rectangle<Type> fromVar (const var& v)                        \
		{                                                                                  \
			const auto arr = Converter::fromVar (v);                                       \
			return { arr[0], arr[1], arr[2], arr[3] };                                     \
		}                                                                                  \
		[[nodiscard]] static var toVar (const Rectangle<Type>& r)                          \
		{                                                                                  \
			return Converter::toVar ({ r.getX(), r.getY(), r.getWidth(), r.getHeight() }); \
		}                                                                                  \
	};

LEMONS_CREATE_RECT_CONVERTER (int)
LEMONS_CREATE_RECT_CONVERTER (float)
LEMONS_CREATE_RECT_CONVERTER (double)

#undef LEMONS_CREATE_RECT_CONVERTER


#define LEMONS_CREATE_LINE_CONVERTER(Type)                                                        \
	template <>                                                                                   \
	struct VariantConverter<Line<Type>> final                                                     \
	{                                                                                             \
		using Converter = lemons::serializing::ArrayConverter<Type>;                              \
		[[nodiscard]] static Line<Type> fromVar (const var& v)                                    \
		{                                                                                         \
			const auto arr = Converter::fromVar (v);                                              \
			return { arr[0], arr[1], arr[2], arr[3] };                                            \
		}                                                                                         \
		[[nodiscard]] static var toVar (const Line<Type>& l)                                      \
		{                                                                                         \
			return Converter::toVar ({ l.getStartX(), l.getStartY(), l.getEndX(), l.getEndX() }); \
		}                                                                                         \
	};

LEMONS_CREATE_LINE_CONVERTER (int)
LEMONS_CREATE_LINE_CONVERTER (float)
LEMONS_CREATE_LINE_CONVERTER (double)

#undef LEMONS_CREATE_LINE_CONVERTER


#define LEMONS_CREATE_PARALLELOGRAM_CONVERTER(Type)                                                                             \
	template <>                                                                                                                 \
	struct VariantConverter<Parallelogram<Type>> final                                                                          \
	{                                                                                                                           \
		using Converter = lemons::serializing::ArrayConverter<Type>;                                                            \
		[[nodiscard]] static Parallelogram<Type> fromVar (const var& v)                                                         \
		{                                                                                                                       \
			const auto arr = Converter::fromVar (v);                                                                            \
			return {                                                                                                            \
				{arr[0],  arr[1]},                                                                                             \
				{ arr[2], arr[3]},                                                                                             \
				{ arr[4], arr[5]}                                                                                              \
			};                                                                                                                  \
		}                                                                                                                       \
		[[nodiscard]] static var toVar (const Parallelogram<Type>& p)                                                           \
		{                                                                                                                       \
			return Converter::toVar ({ p.topLeft.x, p.topLeft.y, p.topRight.x, p.topRight.y, p.bottomLeft.x, p.bottomLeft.y }); \
		}                                                                                                                       \
	};

LEMONS_CREATE_PARALLELOGRAM_CONVERTER (int)
LEMONS_CREATE_PARALLELOGRAM_CONVERTER (float)
LEMONS_CREATE_PARALLELOGRAM_CONVERTER (double)

#undef LEMONS_CREATE_PARALLELOGRAM_CONVERTER

}  // namespace juce
