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
struct VariantConverter<OSCAddressPattern>
{
	static OSCAddressPattern fromVar (const var& v);
	static var				 toVar (const OSCAddressPattern& p);
};

template <>
struct VariantConverter<OSCAddress>
{
	static OSCAddress fromVar (const var& v);
	static var		  toVar (const OSCAddress& p);
};

template <>
struct VariantConverter<OSCColour>
{
	static OSCColour fromVar (const var& v);
	static var		 toVar (const OSCColour& c);
};

template <>
struct VariantConverter<OSCArgument>
{
	static OSCArgument fromVar (const var& v);
	static var		   toVar (const OSCArgument& a);
};

}  // namespace juce
