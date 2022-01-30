
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

namespace lemons::plugin
{

template <typename ValType>
ParameterTraits::ParameterTraits (ValType minimum, ValType maximum, ValType defaultVal,
								  const String& nameToUse, const String& labelToUse,
								  ValToStringFunc<ValType> stringFromValue,
								  StringToValFunc<ValType> valueFromString,
								  bool automatable, bool metaParameter,
								  ParameterCategory categoryToUse)
	: name (nameToUse), label (labelToUse), range (ranges::create (minimum, maximum)), defaultValue (static_cast<float> (defaultVal)), valueToText (detail::convertValToStringFuncFromTyped (stringFromValue, label, range.interval)), textToValue (detail::convertStringToValFuncFromTyped (valueFromString)), isAutomatable (automatable), isMetaParameter (metaParameter), category (categoryToUse)
{
	jassert (valueToText != nullptr);
	jassert (textToValue != nullptr);

	if constexpr (std::is_same_v<ValType, float>)
	{
		valueType = ValueType::floatValue;
	}
	else if constexpr (std::is_same_v<ValType, int>)
	{
		valueType = ValueType::intValue;
	}
	else
	{
		static_assert (std::is_same_v<ValType, bool>, "");

		valueType = ValueType::boolValue;
	}
}

template ParameterTraits::ParameterTraits (float, float, float, const String&, const String&, std::function<String (float, int)>, std::function<float (const String&)>, bool, bool, ParameterCategory);
template ParameterTraits::ParameterTraits (int, int, int, const String&, const String&, std::function<String (int, int)>, std::function<int (const String&)>, bool, bool, ParameterCategory);
template ParameterTraits::ParameterTraits (bool, bool, bool, const String&, const String&, std::function<String (bool, int)>, std::function<bool (const String&)>, bool, bool, ParameterCategory);


bool ParameterTraits::isValid() const
{
	return name.isNotEmpty();
}


namespace ParameterTraitsVTProperties
{
static constexpr auto typeProp		  = "value_type";
static constexpr auto nameProp		  = "name";
static constexpr auto labelProp		  = "label";
static constexpr auto defaultProp	  = "default_value";
static constexpr auto automatableProp = "automatable";
static constexpr auto metaProp		  = "meta_parameter";
static constexpr auto categoryProp	  = "category";
}  // namespace ParameterTraitsVTProperties

ValueTree ParameterTraits::toValueTree() const
{
	if (! isValid())
		return {};

	using namespace ParameterTraitsVTProperties;

	ValueTree tree { valueTreeType };

	tree.setProperty (typeProp, static_cast<int> (valueType), nullptr);
	tree.setProperty (nameProp, name, nullptr);
	tree.setProperty (labelProp, label, nullptr);
	tree.setProperty (defaultProp, defaultValue, nullptr);
	tree.setProperty (automatableProp, isAutomatable, nullptr);
	tree.setProperty (metaProp, isMetaParameter, nullptr);
	tree.setProperty (categoryProp, static_cast<int> (category), nullptr);

	tree.appendChild (ranges::toValueTree (range), nullptr);

	return tree;
}

ParameterTraits ParameterTraits::fromValueTree (const ValueTree& tree)
{
	ParameterTraits traits;

	if (! tree.hasType (valueTreeType))
		return traits;

	using namespace ParameterTraitsVTProperties;

	if (tree.hasProperty (typeProp))
		traits.valueType = static_cast<ValueType> ((int) tree.getProperty (typeProp));	// NOLINT

	if (tree.hasProperty (nameProp))
		traits.name = tree.getProperty (nameProp).toString();

	if (tree.hasProperty (labelProp))
		traits.label = tree.getProperty (labelProp).toString();

	if (tree.hasProperty (defaultProp))
		traits.defaultValue = (float) tree.getProperty (defaultProp);  // NOLINT

	if (tree.hasProperty (automatableProp))
		traits.isAutomatable = (bool) tree.getProperty (automatableProp);  // NOLINT

	if (tree.hasProperty (metaProp))
		traits.isMetaParameter = (bool) tree.getProperty (metaProp);  // NOLINT

	if (tree.hasProperty (categoryProp))
		traits.category = static_cast<ParameterCategory> ((int) tree.getProperty (categoryProp));  // NOLINT

	const auto rangeTree = tree.getChildWithName (ranges::valueTreeType);

	if (rangeTree.isValid())
		traits.range = ranges::fromValueTree<float> (rangeTree);

	return traits;
}


template <template <typename ValueType> class ParameterKind, typename... Args>
static inline std::unique_ptr<Parameter> createParameterOfKind (const ParameterTraits& traits, Args&&... args)
{
	static_assert (std::is_base_of_v<Parameter, ParameterKind<float>>, "");
	static_assert (std::is_base_of_v<Parameter, ParameterKind<int>>, "");
	static_assert (std::is_base_of_v<Parameter, ParameterKind<bool>>, "");

	using VT = ParameterTraits::ValueType;

	if (traits.valueType == VT::floatValue)
	{
		return std::make_unique<ParameterKind<float>> (traits, std::forward<Args> (args)...);
	}

	if (traits.valueType == VT::intValue)
	{
		return std::make_unique<ParameterKind<int>> (traits, std::forward<Args> (args)...);
	}

	jassert (traits.valueType == VT::boolValue);

	return std::make_unique<ParameterKind<bool>> (traits, std::forward<Args> (args)...);
}

std::unique_ptr<Parameter> ParameterTraits::createParameter (const ParameterList& list) const
{
	jassert (isValid());

	if (isMetaParameter)
		return createParameterOfKind<MetaParameter> (*this, list);

	if (! isAutomatable)
		return createParameterOfKind<MeterParameter> (*this);

	return createParameterOfKind<TypedParameter> (*this);
}

/*-----------------------------------------------------------------------------------------------------------------------*/

ParameterLayout ParameterLayout::fromValueTree (const ValueTree& tree)
{
	ParameterLayout layout;

	if (! tree.hasType (valueTreeType))
		return layout;

	for (auto i = 0; i < tree.getNumChildren(); ++i)
	{
		const auto child = tree.getChild (i);

		if (child.hasType (ParameterTraits::valueTreeType))
		{
			const auto traits = ParameterTraits::fromValueTree (child);

			if (traits.isValid())
				layout.parameters.push_back (traits);
		}
	}

	return layout;
}

ValueTree ParameterLayout::saveToValueTree() const
{
	ValueTree tree { valueTreeType };

	for (const auto& param : parameters)
	{
		const auto child = param.toValueTree();

		if (child.isValid())
			tree.appendChild (child, nullptr);
	}

	return tree;
}

}  // namespace lemons::plugin


/*-----------------------------------------------------------------------------------------------------------------------*/

namespace lemons::binary
{

template <files::FileType Type>
plugin::ParameterLayout getParameterLayout (const String& fileName)
{
	return plugin::ParameterLayout::fromValueTree (getValueTree<Type> (fileName));
}

template plugin::ParameterLayout getParameterLayout<files::FileType::JSON> (const String&);
template plugin::ParameterLayout getParameterLayout<files::FileType::XML> (const String&);
template plugin::ParameterLayout getParameterLayout<files::FileType::Opaque> (const String&);

}  // namespace lemons::binary


namespace lemons::files
{

template <FileType Type>
plugin::ParameterLayout loadParameterLayout (const File& file)
{
	return plugin::ParameterLayout::fromValueTree (loadValueTree<Type> (file));
}

template plugin::ParameterLayout loadParameterLayout<files::FileType::JSON> (const File&);
template plugin::ParameterLayout loadParameterLayout<files::FileType::XML> (const File&);
template plugin::ParameterLayout loadParameterLayout<files::FileType::Opaque> (const File&);


template <FileType Type>
bool saveParameterLayout (const plugin::ParameterLayout& layout, const File& file)
{
	const auto tree = layout.saveToValueTree();

	if (! tree.isValid())
		return false;

	return saveValueTree<Type> (file, tree);
}

template bool saveParameterLayout<files::FileType::JSON> (const plugin::ParameterLayout&, const File&);
template bool saveParameterLayout<files::FileType::XML> (const plugin::ParameterLayout&, const File&);
template bool saveParameterLayout<files::FileType::Opaque> (const plugin::ParameterLayout&, const File&);

}  // namespace lemons::files
