namespace juce
{

using namespace lemons::serializing;

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

BigInteger VariantConverter<BigInteger>::fromVar (const var& v)
{
	BigInteger i;

	if (const auto* block = v.getBinaryData())
		i.loadFromMemoryBlock (*block);

	return i;
}

var VariantConverter<BigInteger>::toVar (const BigInteger& i)
{
	return { i.toMemoryBlock() };
}

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

IPAddress VariantConverter<IPAddress>::fromVar (const var& v)
{
	IPAddress a { v.toString() };
	return a;
}

var VariantConverter<IPAddress>::toVar (const IPAddress& a)
{
	return { a.toString() };
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

MemoryBlock VariantConverter<MemoryBlock>::fromVar (const var& v)
{
	return memoryBlockFromString (v.toString());
}

var VariantConverter<MemoryBlock>::toVar (const MemoryBlock& b)
{
	return { memoryBlockToString (b) };
}

MidiBuffer VariantConverter<MidiBuffer>::fromVar (const var& v)
{
	return midiFromBinary (memoryBlockFromString (v.toString()));
}

var VariantConverter<MidiBuffer>::toVar (const MidiBuffer& b)
{
	return { memoryBlockToString (midiToBinary (b)) };
}

MidiFile VariantConverter<MidiFile>::fromVar (const var& v)
{
	return midiBufferToFile (VariantConverter<MidiBuffer>::fromVar (v));
}

var VariantConverter<MidiFile>::toVar (const MidiFile& f)
{
	return VariantConverter<MidiBuffer>::toVar (midiBufferFromFile (f));
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

StringArray VariantConverter<StringArray>::fromVar (const var& v)
{
	StringArray arr;

	if (const auto* vars = v.getArray())
		for (const auto& var : *vars)
			arr.add (var.toString());

	return arr;
}

var VariantConverter<StringArray>::toVar (const StringArray& a)
{
	Array<var> vars;

	for (const auto& string : a)
		vars.add (string);

	return { vars };
}

StringPairArray VariantConverter<StringPairArray>::fromVar (const var& v)
{
	StringPairArray arr;

	if (auto* obj = v.getDynamicObject())
	{
		for (const auto& itr : obj->getProperties())
		{
			arr.set (itr.name.toString(),
			         itr.value.toString());
		}
	}

	return arr;
}

var VariantConverter<StringPairArray>::toVar (const StringPairArray& a)
{
	DynamicObject obj;

	const auto& keys   = a.getAllKeys();
	const auto& values = a.getAllValues();

	jassert (keys.size() == values.size());

	for (int i = 0; i < keys.size(); ++i)
	{
		obj.setProperty (keys.strings.getUnchecked (i),
		                 values.strings.getUnchecked (i));
	}

	return { &obj };
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

Uuid VariantConverter<Uuid>::fromVar (const var& v)
{
	return { v.toString() };
}

var VariantConverter<Uuid>::toVar (const Uuid& u)
{
	return { u.toString() };
}

URL VariantConverter<URL>::fromVar (const var& v)
{
	return { v.toString() };
}

var VariantConverter<URL>::toVar (const URL& u)
{
	return { u.toString (true) };
}


/*---------------------------------------------------------------------------------------------------------------------------------*/


lemons::Dimensions VariantConverter<lemons::Dimensions>::fromVar (const var& v)
{
	return lemons::Dimensions::fromString (v.toString());
}

var VariantConverter<lemons::Dimensions>::toVar (const lemons::Dimensions& d)
{
	return { d.toString() };
}

lemons::Version VariantConverter<lemons::Version>::fromVar (const var& v)
{
	return lemons::Version::fromString (v.toString());
}

var VariantConverter<lemons::Version>::toVar (const lemons::Version& v)
{
	return { v.toString() };
}

}  // namespace juce
