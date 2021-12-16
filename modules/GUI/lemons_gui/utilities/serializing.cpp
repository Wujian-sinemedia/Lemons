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

}
