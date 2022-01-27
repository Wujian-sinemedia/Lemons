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


BigInteger VariantConverter<BigInteger>::fromVar (const var& v)
{
	BigInteger i;

	if (const auto* obj = v.getDynamicObject())
	{
		if (obj->hasProperty (data))
			if (const auto* block = obj->getProperty (data).getBinaryData())
				i.loadFromMemoryBlock (*block);

		if (obj->hasProperty (negative))
			i.setNegative ((bool) obj->getProperty (negative));	 // NOLINT
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
	return RelativeTime::milliseconds ((int64) v);	// NOLINT
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
	Time t { (int64) v };  // NOLINT
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
