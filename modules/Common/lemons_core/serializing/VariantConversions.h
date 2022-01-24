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


#pragma once


namespace juce
{

template <>
struct VariantConverter<BigInteger>
{
	static BigInteger fromVar (const var& v);
	static var		  toVar (const BigInteger& i);

private:

	static constexpr auto data	   = "Data";
	static constexpr auto negative = "Negative";
};

template <>
struct VariantConverter<DynamicObject>
{
	static DynamicObject fromVar (const var& v);
	static var			 toVar (const DynamicObject& d);
};

template <>
struct VariantConverter<Identifier>
{
	static Identifier fromVar (const var& v);
	static var		  toVar (const Identifier& i);
};

template <>
struct VariantConverter<IPAddress>
{
	static IPAddress fromVar (const var& v);
	static var		 toVar (const IPAddress& a);
};

template <>
struct VariantConverter<MACAddress>
{
	static MACAddress fromVar (const var& v);
	static var		  toVar (const MACAddress& a);
};

template <>
struct VariantConverter<MemoryBlock>
{
	static MemoryBlock fromVar (const var& v);
	static var		   toVar (const MemoryBlock& b);
};

template <>
struct VariantConverter<NamedValueSet>
{
	static NamedValueSet fromVar (const var& v);
	static var			 toVar (const NamedValueSet& s);
};


template <>
struct VariantConverter<PropertySet>
{
	static PropertySet fromVar (const var& v);
	static var		   toVar (const PropertySet& s);
};

template <>
struct VariantConverter<RelativeTime>
{
	static RelativeTime fromVar (const var& v);
	static var			toVar (const RelativeTime& t);
};

template <>
struct VariantConverter<StringArray>
{
	static StringArray fromVar (const var& v);
	static var		   toVar (const StringArray& a);
};

template <>
struct VariantConverter<StringPairArray>
{
	static StringPairArray fromVar (const var& v);
	static var			   toVar (const StringPairArray& a);
};

template <>
struct VariantConverter<Time>
{
	static Time fromVar (const var& v);
	static var	toVar (const Time& t);
};

template <>
struct VariantConverter<Uuid>
{
	static Uuid fromVar (const var& v);
	static var	toVar (const Uuid& u);
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
	static var		 toVar (const ValueTree& t);

private:

	static constexpr auto NAME_PROP		= "_name";
	static constexpr auto CHILDREN_PROP = "_children";
	static constexpr auto BASE64_PROP	= "_base64:";
};

template <>
struct VariantConverter<XmlElement>
{
	static XmlElement fromVar (const var& v);
	static var		  toVar (const XmlElement& e);
};


/*---------------------------------------------------------------------------------------------------------------------------------*/

template <>
struct VariantConverter<lemons::Dimensions>
{
	static lemons::Dimensions fromVar (const var& v);
	static var				  toVar (const lemons::Dimensions& d);
};

template <>
struct VariantConverter<lemons::Version>
{
	static lemons::Version fromVar (const var& v);
	static var			   toVar (const lemons::Version& v);
};

}  // namespace juce
