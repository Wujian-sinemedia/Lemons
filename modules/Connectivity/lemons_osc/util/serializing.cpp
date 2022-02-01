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

#include <lemons_core/lemons_core.h>

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

OSCColour VariantConverter<OSCColour>::fromVar (const var& v)
{
	return OSCColour::fromInt32 (static_cast<uint32> ((int) v));  // NOLINT
}

var VariantConverter<OSCColour>::toVar (const OSCColour& c)
{
	return { static_cast<int> (c.toInt32()) };
}

static constexpr auto osc_arg_content_prop = "CONTENT";
static constexpr auto osc_arg_type_prop	   = "TYPE";

OSCArgument VariantConverter<OSCArgument>::fromVar (const var& v)
{
	if (const auto* obj = v.getDynamicObject())
	{
		if (obj->hasProperty (osc_arg_type_prop) && obj->hasProperty (osc_arg_content_prop))
		{
			const auto type = obj->getProperty (osc_arg_type_prop).toString();

			const auto& content = obj->getProperty (osc_arg_content_prop);

			if (type == "int32")
				return { static_cast<int32> ((int) content) };	// NOLINT

			if (type == "float32")
				return { (float) content };	 // NOLINT

			if (type == "string")
				return { content.toString() };

			if (type == "blob")
				return { VariantConverter<MemoryBlock>::fromVar (content) };

			if (type == "color")
				return { VariantConverter<OSCColour>::fromVar (content) };
		}
	}

	jassertfalse;

	return { "invalid" };
}

var VariantConverter<OSCArgument>::toVar (const OSCArgument& a)
{
	DynamicObject obj;

	if (a.isInt32())
	{
		obj.setProperty (osc_arg_type_prop, "int32");
		obj.setProperty (osc_arg_content_prop, a.getInt32());
	}
	else if (a.isFloat32())
	{
		obj.setProperty (osc_arg_type_prop, "float32");
		obj.setProperty (osc_arg_content_prop, a.getFloat32());
	}
	else if (a.isString())
	{
		obj.setProperty (osc_arg_type_prop, "string");
		obj.setProperty (osc_arg_content_prop, a.getString());
	}
	else if (a.isBlob())
	{
		obj.setProperty (osc_arg_type_prop, "blob");
		obj.setProperty (osc_arg_content_prop,
						 VariantConverter<MemoryBlock>::toVar (a.getBlob()));
	}
	else
	{
		jassert (a.isColour());

		obj.setProperty (osc_arg_type_prop, "color");

		obj.setProperty (osc_arg_content_prop,
						 VariantConverter<OSCColour>::toVar (a.getColour()));
	}

	return { obj.clone().get() };
}

}  // namespace juce
