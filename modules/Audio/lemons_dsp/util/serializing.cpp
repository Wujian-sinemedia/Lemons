/*
 ======================================================================================

 ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝

 This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.

 ======================================================================================
 */

namespace lemons::serializing
{

template <typename SampleType>
MemoryBlock audioToBinary (const AudioBuffer<SampleType>& buffer, double samplerate)
{
    juce::FlacAudioFormat format;
    
    MemoryBlock block;
    
    dsp::formats::writeAudioToBlock (block, buffer, samplerate, format);
    
    return block;
}

template MemoryBlock audioToBinary (const AudioBuffer<float>&, double);
template MemoryBlock audioToBinary (const AudioBuffer<double>&, double);


template <typename SampleType>
AudioBuffer<SampleType> audioFromBinary (const MemoryBlock& block)
{
    juce::FlacAudioFormat format;
    
    AudioBuffer<SampleType> buffer;

    dsp::formats::readAudioFromBlock (buffer, block, format);
    
    return buffer;
}

template AudioBuffer<float> audioFromBinary (const MemoryBlock&);
template AudioBuffer<double> audioFromBinary (const MemoryBlock&);


namespace ChannelSetVT
{
static constexpr auto valueTreeType = "AudioChannelSet";

static constexpr auto channelTreeType = "ChannelType";
static constexpr auto channelTypeProp = "channel_type";

[[nodiscard]] static ValueTree channelTypeToValueTree (juce::AudioChannelSet::ChannelType type)
{
    ValueTree tree { channelTreeType };
    
    tree.setProperty (channelTypeProp, static_cast<int>(type), nullptr);
    
    return tree;
}

[[nodiscard]] static juce::AudioChannelSet::ChannelType channelTypeFromValueTree (const ValueTree& tree)
{
    if (! tree.hasType (channelTreeType))
        return {};
    
    if (tree.hasProperty (channelTypeProp))
        return static_cast<juce::AudioChannelSet::ChannelType>((int) tree.getProperty (channelTypeProp));
    
    return {};
}

}

juce::AudioChannelSet channelSetFromValueTree (const ValueTree& tree)
{
    using namespace ChannelSetVT;
    
    juce::AudioChannelSet set;
    
    if (! tree.hasType (valueTreeType))
        return set;
    
    for (int i = 0; i < tree.getNumChildren(); ++i)
    {
        const auto child = tree.getChild (i);
        
        if (child.hasType (channelTreeType))
            set.addChannel (channelTypeFromValueTree (child));
    }
    
    return set;
}

ValueTree channelSetToValueTree (const juce::AudioChannelSet& set)
{
    using namespace ChannelSetVT;
    
    ValueTree tree { valueTreeType };
    
    for (int i = 0; i < set.size(); ++i)
    {
        const auto child = channelTypeToValueTree (set.getTypeOfChannel (i));
        
        if (child.isValid())
            tree.appendChild (child, nullptr);
    }
    
    return tree;
}

}  // namespace lemons::serializing


namespace lemons::files
{

template <typename SampleType>
AudioBuffer<SampleType> loadAudioBuffer (const File& file)
{
	return serializing::audioFromBinary<SampleType> (loadFileAsBlock (file));
}

template <typename SampleType>
bool saveAudioBuffer (const AudioBuffer<SampleType>& audio, const File& file, double samplerate)
{
	return saveBlockToFile (serializing::audioToBinary (audio, samplerate), file);
}

}  // namespace lemons::files


namespace lemons::binary
{

template <typename SampleType>
AudioBuffer<SampleType> getAudioBuffer (const String& filename)
{
	return serializing::audioFromBinary<SampleType> (getBlob (filename));
}

}  // namespace lemons::binary

/*---------------------------------------------------------------------------------------------------------------------------*/

namespace juce
{

using namespace lemons::serializing;

AudioChannelSet VariantConverter<AudioChannelSet>::fromVar (const var& v)
{
    const auto tree = VariantConverter<ValueTree>::fromVar (v);
    return channelSetFromValueTree (tree);
}

var VariantConverter<AudioChannelSet>::toVar (const AudioChannelSet& s)
{
    const auto tree = channelSetToValueTree (s);
    return VariantConverter<ValueTree>::toVar (tree);
}

ADSR::Parameters VariantConverter<ADSR::Parameters>::fromVar (const var& v)
{
	ADSR::Parameters p;

	if (const auto* obj = v.getDynamicObject())
	{
		if (obj->hasProperty (attack))
			p.attack = obj->getProperty (attack);

		if (obj->hasProperty (decay))
			p.decay = obj->getProperty (decay);

		if (obj->hasProperty (sustain))
			p.sustain = obj->getProperty (sustain);

		if (obj->hasProperty (release))
			p.release = obj->getProperty (release);
	}

	return p;
}

var VariantConverter<ADSR::Parameters>::toVar (const ADSR::Parameters& p)
{
	DynamicObject obj;

	obj.setProperty (attack, p.attack);
	obj.setProperty (decay, p.decay);
	obj.setProperty (sustain, p.sustain);
	obj.setProperty (release, p.release);

	return { obj.clone().get() };
}

AudioBuffer<float> VariantConverter<AudioBuffer<float>>::fromVar (const var& v)
{
	return audioFromBinary<float> (memoryBlockFromString (v.toString()));
}

var VariantConverter<AudioBuffer<float>>::toVar (const AudioBuffer<float>& b)
{
	return { memoryBlockToString (audioToBinary (b)) };
}

AudioBuffer<double> VariantConverter<AudioBuffer<double>>::fromVar (const var& v)
{
	return audioFromBinary<double> (memoryBlockFromString (v.toString()));
}

var VariantConverter<AudioBuffer<double>>::toVar (const AudioBuffer<double>& b)
{
	return { memoryBlockToString (audioToBinary (b)) };
}

}  // namespace juce
