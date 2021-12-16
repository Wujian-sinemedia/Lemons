#pragma once

#include <juce_graphics/juce_graphics.h>

namespace lemons::serializing
{

using juce::Image;
using juce::MemoryBlock;

/** Encodes an Image object as a blob of binary data.
 @see imageFromBinary()
 */
[[nodiscard]] MemoryBlock imageToBinary (const Image& image);

/** Returns an Image object from a binary representation of one.
 @see imageToBinary()
 */
[[nodiscard]] Image imageFromBinary (const MemoryBlock& block);

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


}
