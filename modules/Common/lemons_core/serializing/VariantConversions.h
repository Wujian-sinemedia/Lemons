#pragma once


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
struct VariantConverter<BigInteger>
{
	static BigInteger fromVar (const var& v);
	static var        toVar (const BigInteger& i);
};

template <>
struct VariantConverter<Colour>
{
	static Colour fromVar (const var& v);
	static var    toVar (const Colour& c);
};

template <>
struct VariantConverter<Identifier>
{
	static Identifier fromVar (const var& v);
	static var        toVar (const Identifier& i);
};

template <>
struct VariantConverter<Image>
{
	static Image fromVar (const var& v);
	static var   toVar (const Image& i);
};

template <>
struct VariantConverter<IPAddress>
{
	static IPAddress fromVar (const var& v);
	static var       toVar (const IPAddress& a);
};

template <>
struct VariantConverter<Justification>
{
	static Justification fromVar (const var& v);
	static var           toVar (const Justification& j);
};

template <>
struct VariantConverter<MACAddress>
{
	static MACAddress fromVar (const var& v);
	static var        toVar (const MACAddress& a);
};

template <>
struct VariantConverter<MemoryBlock>
{
	static MemoryBlock fromVar (const var& v);
	static var         toVar (const MemoryBlock& b);
};

template <>
struct VariantConverter<MidiBuffer>
{
	static MidiBuffer fromVar (const var& v);
	static var        toVar (const MidiBuffer& b);
};

template <>
struct VariantConverter<MidiFile>
{
	static MidiFile fromVar (const var& v);
	static var      toVar (const MidiFile& f);
};

template <>
struct VariantConverter<MidiMessage>
{
	static MidiMessage fromVar (const var& v);
	static var         toVar (const MidiMessage& m);

private:
	static constexpr auto data_prop = "Data";
	static constexpr auto time_prop = "Timestamp";
};

template <>
struct VariantConverter<NamedValueSet>
{
	static NamedValueSet fromVar (const var& v);
	static var           toVar (const NamedValueSet& s);
};

template <>
struct VariantConverter<RelativeTime>
{
	static RelativeTime fromVar (const var& v);
	static var          toVar (const RelativeTime& t);
};

template <>
struct VariantConverter<StringArray>
{
	static StringArray fromVar (const var& v);
	static var         toVar (const StringArray& a);
};

template <>
struct VariantConverter<StringPairArray>
{
	static StringPairArray fromVar (const var& v);
	static var             toVar (const StringPairArray& a);
};

template <>
struct VariantConverter<Time>
{
	static Time fromVar (const var& v);
	static var  toVar (const Time& t);
};

template <>
struct VariantConverter<Uuid>
{
	static Uuid fromVar (const var& v);
	static var  toVar (const Uuid& u);
};

template <>
struct VariantConverter<URL>
{
	static URL fromVar (const var& v);
	static var toVar (const URL& u);
};

template <>
struct VariantConverter<ValueTree>
{
	static ValueTree fromVar (const var& v);
	static var       toVar (const ValueTree& t);

private:
	static constexpr auto NAME_PROP     = "_name";
	static constexpr auto CHILDREN_PROP = "_children";
	static constexpr auto BASE64_PROP   = "_base64:";
};


/*---------------------------------------------------------------------------------------------------------------------------------*/

template <>
struct VariantConverter<lemons::Dimensions>
{
	static lemons::Dimensions fromVar (const var& v);
	static var                toVar (const lemons::Dimensions& d);
};

template <>
struct VariantConverter<lemons::Version>
{
	static lemons::Version fromVar (const var& v);
	static var             toVar (const lemons::Version& v);
};

}  // namespace juce
