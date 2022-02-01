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

namespace juce
{

OSCAddressPattern VariantConverter<OSCAddressPattern>::fromVar (const var& v)
{
	return { v.toString() };
}

var VariantConverter<OSCAddressPattern>::toVar (const OSCAddressPattern& p)
{
	return { p.toString() };
}


OSCAddress VariantConverter<OSCAddress>::fromVar (const var& v)
{
	return { v.toString() };
}

var VariantConverter<OSCAddress>::toVar (const OSCAddress& p)
{
	return { p.toString() };
}

}  // namespace juce
