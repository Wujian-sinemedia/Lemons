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

#include <juce_osc/juce_osc.h>

namespace juce
{

template <>
struct VariantConverter<OSCAddressPattern> final
{
	[[nodiscard]] static OSCAddressPattern fromVar (const var& v);
	[[nodiscard]] static var			   toVar (const OSCAddressPattern& p);
};

template <>
struct VariantConverter<OSCAddress> final
{
	[[nodiscard]] static OSCAddress fromVar (const var& v);
	[[nodiscard]] static var		toVar (const OSCAddress& p);
};

template <>
struct VariantConverter<OSCColour> final
{
	[[nodiscard]] static OSCColour fromVar (const var& v);
	[[nodiscard]] static var	   toVar (const OSCColour& c);
};

template <>
struct VariantConverter<OSCTimeTag> final
{
	[[nodiscard]] static OSCTimeTag fromVar (const var& v);
	[[nodiscard]] static var		toVar (const OSCTimeTag& t);
};

template <>
struct VariantConverter<OSCArgument> final
{
	[[nodiscard]] static OSCArgument fromVar (const var& v);
	[[nodiscard]] static var		 toVar (const OSCArgument& a);

private:

	static constexpr auto CONTENT_PROP = "content";
	static constexpr auto TYPE_PROP	   = "type";
};

}  // namespace juce
