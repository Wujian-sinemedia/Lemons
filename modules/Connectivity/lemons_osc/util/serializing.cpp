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

OSCTimeTag VariantConverter<OSCTimeTag>::fromVar (const var& v)
{
	return { VariantConverter<Time>::fromVar (v) };
}

var VariantConverter<OSCTimeTag>::toVar (const OSCTimeTag& t)
{
	return VariantConverter<Time>::toVar (t.toTime());
}

OSCArgument VariantConverter<OSCArgument>::fromVar (const var& v)
{
	if (const auto* obj = v.getDynamicObject())
	{
		if (obj->hasProperty (TYPE_PROP) && obj->hasProperty (CONTENT_PROP))
		{
			const auto type = obj->getProperty (TYPE_PROP).toString()[0];

			const auto& content = obj->getProperty (CONTENT_PROP);

			if (type == OSCTypes::int32)
				return { static_cast<int32> ((int) content) };	// NOLINT

			if (type == OSCTypes::float32)
				return { (float) content };	 // NOLINT

			if (type == OSCTypes::string)
				return { content.toString() };

			if (type == OSCTypes::blob)
				return { VariantConverter<MemoryBlock>::fromVar (content) };

			if (type == OSCTypes::colour)
				return { VariantConverter<OSCColour>::fromVar (content) };
		}
	}

	jassertfalse;

	return { "invalid" };
}

var VariantConverter<OSCArgument>::toVar (const OSCArgument& a)
{
	DynamicObject obj;

	obj.setProperty (TYPE_PROP, a.getType());

	if (a.isInt32())
	{
		obj.setProperty (CONTENT_PROP, a.getInt32());
	}
	else if (a.isFloat32())
	{
		obj.setProperty (CONTENT_PROP, a.getFloat32());
	}
	else if (a.isString())
	{
		obj.setProperty (CONTENT_PROP, a.getString());
	}
	else if (a.isBlob())
	{
		obj.setProperty (CONTENT_PROP, VariantConverter<MemoryBlock>::toVar (a.getBlob()));
	}
	else
	{
		jassert (a.isColour());

		obj.setProperty (CONTENT_PROP, VariantConverter<OSCColour>::toVar (a.getColour()));
	}

	return { obj.clone().get() };
}

}  // namespace juce
