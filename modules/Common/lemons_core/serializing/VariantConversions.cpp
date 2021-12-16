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


namespace juce
{

using namespace lemons::serializing;


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

BigInteger VariantConverter<BigInteger>::fromVar (const var& v)
{
	BigInteger i;

	if (const auto* obj = v.getDynamicObject())
	{
		if (obj->hasProperty (data))
			if (const auto* block = obj->getProperty (data).getBinaryData())
				i.loadFromMemoryBlock (*block);

		if (obj->hasProperty (negative))
			i.setNegative ((bool) obj->getProperty (negative));
	}

	return i;
}

var VariantConverter<BigInteger>::toVar (const BigInteger& i)
{
	DynamicObject obj;

	obj.setProperty (data, i.toMemoryBlock());
	obj.setProperty (negative, i.isNegative());

	return { obj.clone().get() };
}

Colour VariantConverter<Colour>::fromVar (const var& v)
{
	return Colour::fromString (v.toString());
}

var VariantConverter<Colour>::toVar (const Colour& c)
{
	return { c.toString() };
}

DynamicObject VariantConverter<DynamicObject>::fromVar (const var& v)
{
	if (auto* obj = v.getDynamicObject())
		return *obj;

	return {};
}

var VariantConverter<DynamicObject>::toVar (const DynamicObject& d)
{
	DynamicObject clone { d };
	return { clone.clone().get() };
}

Identifier VariantConverter<Identifier>::fromVar (const var& v)
{
	return { v.toString() };
}

var VariantConverter<Identifier>::toVar (const Identifier& i)
{
	return { i.toString() };
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

Justification VariantConverter<Justification>::fromVar (const var& v)
{
	return { (int) v };
}

var VariantConverter<Justification>::toVar (const Justification& j)
{
	return { j.getFlags() };
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
	juce::MidiFile file;

	if (const auto* block = v.getBinaryData())
	{
		juce::MemoryInputStream stream { *block, false };
		file.readFrom (stream);
	}

	return file;
}

var VariantConverter<MidiFile>::toVar (const MidiFile& f)
{
	MemoryBlock              block;
	juce::MemoryOutputStream stream { block, false };

	f.writeTo (stream);

	return { block };
}

MidiMessage VariantConverter<MidiMessage>::fromVar (const var& v)
{
	if (const auto* obj = v.getDynamicObject())
	{
		if (obj->hasProperty (data_prop))
		{
			if (const auto* data = obj->getProperty (data_prop).getBinaryData())
			{
				const auto timestamp = [&]() -> double
				{
					if (obj->hasProperty (time_prop))
						return obj->getProperty (time_prop);

					return 0.;
				}();

				return { data->getData(), static_cast<int> (data->getSize()), timestamp };
			}
		}
	}

	return {};
}

var VariantConverter<MidiMessage>::toVar (const MidiMessage& m)
{
	DynamicObject obj;

	MemoryBlock block { m.getRawData(), static_cast<size_t> (m.getRawDataSize()) };

	obj.setProperty (data_prop, block);
	obj.setProperty (time_prop, m.getTimeStamp());

	return { obj.clone().get() };
}

NamedValueSet VariantConverter<NamedValueSet>::fromVar (const var& v)
{
	if (auto* obj = v.getDynamicObject())
		return obj->getProperties();

	return {};
}

var VariantConverter<NamedValueSet>::toVar (const NamedValueSet& s)
{
	DynamicObject obj;

	for (const auto& prop : s)
		obj.setProperty (prop.name, prop.value);

	return { obj.clone().get() };
}

PluginDescription VariantConverter<PluginDescription>::fromVar (const var& v)
{
	PluginDescription d;

	d.loadFromXml (VariantConverter<XmlElement>::fromVar (v));

	return d;
}

var VariantConverter<PluginDescription>::toVar (const PluginDescription& d)
{
	return VariantConverter<XmlElement>::toVar (*d.createXml());
}

PropertySet VariantConverter<PropertySet>::fromVar (const var& v)
{
	PropertySet set;

	set.restoreFromXml (VariantConverter<XmlElement>::fromVar (v));

	return set;
}

var VariantConverter<PropertySet>::toVar (const PropertySet& s)
{
	return VariantConverter<XmlElement>::toVar (*s.createXml ("Properties"));
}

RelativeTime VariantConverter<RelativeTime>::fromVar (const var& v)
{
	return RelativeTime::milliseconds ((int64) v);
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
	return { a };
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

	return { obj.clone().get() };
}

Time VariantConverter<Time>::fromVar (const var& v)
{
	Time t { (int64) v };
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

XmlElement VariantConverter<XmlElement>::fromVar (const var& v)
{
	return *XmlDocument::parse (v.toString());
}

var VariantConverter<XmlElement>::toVar (const XmlElement& e)
{
	return { e.toString() };
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
