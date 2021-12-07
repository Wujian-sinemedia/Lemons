namespace juce
{

using namespace lemons::binary;

Image VariantConverter<Image>::fromVar (const var& v)
{
	return imageFromBinary (memoryBlockFromString (v.toString()));
}

var VariantConverter<Image>::toVar (const Image& i)
{
	return { memoryBlockToString (imageToBinary (i)) };
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

MidiBuffer VariantConverter<MidiBuffer>::fromVar (const var& v)
{
	return midiFromBinary (memoryBlockFromString (v.toString()));
}

var VariantConverter<MidiBuffer>::toVar (const MidiBuffer& b)
{
	return { memoryBlockToString (midiToBinary (b)) };
}

MemoryBlock VariantConverter<MemoryBlock>::fromVar (const var& v)
{
	return memoryBlockFromString (v.toString());
}

var VariantConverter<MemoryBlock>::toVar (const MemoryBlock& b)
{
	return { memoryBlockToString (b) };
}

ValueTree VariantConverter<ValueTree>::fromVar (const var& v)
{
	return valueTreefromVar (v);
}

var VariantConverter<ValueTree>::toVar (const ValueTree& t)
{
	return valueTreeToVar (t);
}

URL VariantConverter<URL>::fromVar (const var& v)
{
	return { v.toString() };
}

var VariantConverter<URL>::toVar (const URL& u)
{
	return { u.toString (true) };
}

Uuid VariantConverter<Uuid>::fromVar (const var& v)
{
	return { v.toString() };
}

var VariantConverter<Uuid>::toVar (const Uuid& u)
{
	return { u.toString() };
}

MACAddress VariantConverter<MACAddress>::fromVar (const var& v)
{
	MACAddress a { v.toString() };
	return a;
}

var VariantConverter<MACAddress>::toVar (const MACAddress& a)
{
	return { a.toString() };
}

IPAddress VariantConverter<IPAddress>::fromVar (const var& v)
{
	IPAddress a { v.toString() };
	return a;
}

var VariantConverter<IPAddress>::toVar (const IPAddress& a)
{
	return { a.toString() };
}

Time VariantConverter<Time>::fromVar (const var& v)
{
	const auto ms = (int64) v;
	Time       t { ms };
	return t;
}

var VariantConverter<Time>::toVar (const Time& t)
{
	return { t.toMilliseconds() };
}

RelativeTime VariantConverter<RelativeTime>::fromVar (const var& v)
{
	const auto ms = (int64) v;
	return RelativeTime::milliseconds (ms);
}

var VariantConverter<RelativeTime>::toVar (const RelativeTime& t)
{
	return { t.inMilliseconds() };
}

Colour VariantConverter<Colour>::fromVar (const var& v)
{
	const auto argb = static_cast<uint32> ((int) v);
	Colour     c { argb };
	return c;
}

var VariantConverter<Colour>::toVar (const Colour& c)
{
	return { static_cast<int> (c.getARGB()) };
}

}  // namespace juce
