
/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 * ======================================================================================
 */

#pragma once


namespace juce
{

template <>
struct VariantConverter<BigInteger> final
{
	[[nodiscard]] static BigInteger fromVar (const var& v);
	[[nodiscard]] static var		toVar (const BigInteger& i);

private:

	static constexpr auto data	   = "Data";
	static constexpr auto negative = "Negative";
};

template <>
struct VariantConverter<DynamicObject> final
{
	[[nodiscard]] static DynamicObject fromVar (const var& v);
	[[nodiscard]] static var		   toVar (const DynamicObject& d);
};

template <>
struct VariantConverter<File> final
{
	[[nodiscard]] static File fromVar (const var& v);
	[[nodiscard]] static var  toVar (const File& f);
};

template <>
struct VariantConverter<Identifier> final
{
	[[nodiscard]] static Identifier fromVar (const var& v);
	[[nodiscard]] static var		toVar (const Identifier& i);
};

template <>
struct VariantConverter<IPAddress> final
{
	[[nodiscard]] static IPAddress fromVar (const var& v);
	[[nodiscard]] static var	   toVar (const IPAddress& a);
};

template <>
struct VariantConverter<MACAddress> final
{
	[[nodiscard]] static MACAddress fromVar (const var& v);
	[[nodiscard]] static var		toVar (const MACAddress& a);
};

template <>
struct VariantConverter<MemoryBlock> final
{
	[[nodiscard]] static MemoryBlock fromVar (const var& v);
	[[nodiscard]] static var		 toVar (const MemoryBlock& b);
};

template <>
struct VariantConverter<NamedValueSet> final
{
	[[nodiscard]] static NamedValueSet fromVar (const var& v);
	[[nodiscard]] static var		   toVar (const NamedValueSet& s);
};


template <>
struct VariantConverter<PropertySet> final
{
	[[nodiscard]] static PropertySet fromVar (const var& v);
	[[nodiscard]] static var		 toVar (const PropertySet& s);
};

template <>
struct VariantConverter<Random> final
{
	[[nodiscard]] static Random fromVar (const var& v);
	[[nodiscard]] static var	toVar (const Random& r);
};

template <>
struct VariantConverter<RelativeTime> final
{
	[[nodiscard]] static RelativeTime fromVar (const var& v);
	[[nodiscard]] static var		  toVar (const RelativeTime& t);
};

template <>
struct VariantConverter<StringArray> final
{
	[[nodiscard]] static StringArray fromVar (const var& v);
	[[nodiscard]] static var		 toVar (const StringArray& a);
};

template <>
struct VariantConverter<StringPairArray> final
{
	[[nodiscard]] static StringPairArray fromVar (const var& v);
	[[nodiscard]] static var			 toVar (const StringPairArray& a);
};

template <>
struct VariantConverter<Time> final
{
	[[nodiscard]] static Time fromVar (const var& v);
	[[nodiscard]] static var  toVar (const Time& t);
};

template <>
struct VariantConverter<Uuid> final
{
	[[nodiscard]] static Uuid fromVar (const var& v);
	[[nodiscard]] static var  toVar (const Uuid& u);
};

template <>
struct VariantConverter<URL> final
{
	[[nodiscard]] static URL fromVar (const var& v);
	[[nodiscard]] static var toVar (const URL& u);
};

template <>
struct VariantConverter<ValueTree> final
{
	[[nodiscard]] static ValueTree fromVar (const var& v);
	[[nodiscard]] static var	   toVar (const ValueTree& t);

private:

	static constexpr auto NAME_PROP		= "_name";
	static constexpr auto CHILDREN_PROP = "_children";
	static constexpr auto BASE64_PROP	= "_base64:";
	static constexpr auto ARRAY_PROP	= "_array:";
};

template <>
struct VariantConverter<XmlElement> final
{
	[[nodiscard]] static XmlElement fromVar (const var& v);
	[[nodiscard]] static var		toVar (const XmlElement& e);
};

/*---------------------------------------------------------------------------------------------------------------------------------*/

template <>
struct VariantConverter<std::string> final
{
	[[nodiscard]] static std::string fromVar (const var& v);
	[[nodiscard]] static var		 toVar (const std::string& s);
};

/*---------------------------------------------------------------------------------------------------------------------------------*/

template <>
struct VariantConverter<lemons::Dimensions> final
{
	[[nodiscard]] static lemons::Dimensions fromVar (const var& v);
	[[nodiscard]] static var				toVar (const lemons::Dimensions& d);
};

template <>
struct VariantConverter<lemons::Version> final
{
	[[nodiscard]] static lemons::Version fromVar (const var& v);
	[[nodiscard]] static var			 toVar (const lemons::Version& v);
};

}  // namespace juce
