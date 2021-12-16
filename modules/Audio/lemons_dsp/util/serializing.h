#pragma once

namespace lemons::serializing
{

template <typename SampleType>
using AudioBuffer = juce::AudioBuffer<SampleType>;


/** Encodes an AudioBuffer as a blob of binary data. Note that the sample rate may be optionally specified.
    @see audioFromBinary()
 */
template <typename SampleType>
[[nodiscard]] MemoryBlock audioToBinary (const AudioBuffer<SampleType>& buffer, double samplerate = 48000.);

/** Returns an AudioBuffer from a binary representation of one.
    @see audioToBinary()
 */
template <typename SampleType>
[[nodiscard]] AudioBuffer<SampleType> audioFromBinary (const MemoryBlock& block);

}

/*---------------------------------------------------------------------------------------------------------------------------*/

namespace juce
{

template <>
struct VariantConverter<ADSR::Parameters>
{
    static ADSR::Parameters fromVar (const var& v);
    static var              toVar (const ADSR::Parameters& p);
    
private:
    static constexpr auto attack  = "Attack";
    static constexpr auto decay   = "Decay";
    static constexpr auto sustain = "Sustain";
    static constexpr auto release = "Release";
};

template <>
struct VariantConverter<AudioBuffer<float>>
{
    static AudioBuffer<float> fromVar (const var& v);
    static var                toVar (const AudioBuffer<float>& b);
};

template <>
struct VariantConverter<AudioBuffer<double>>
{
    static AudioBuffer<double> fromVar (const var& v);
    static var                 toVar (const AudioBuffer<double>& b);
};

template <>
struct VariantConverter<PluginDescription>
{
    static PluginDescription fromVar (const var& v);
    static var               toVar (const PluginDescription& d);
};

}
