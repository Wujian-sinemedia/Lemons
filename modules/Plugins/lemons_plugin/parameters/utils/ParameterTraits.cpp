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


namespace lemons::plugin
{

template <typename ValType>
ParameterTraits::ParameterTraits (ValType minimum, ValType maximum, ValType defaultVal,
                                  std::function<String (ValType, int)>   stringFromValue,
                                  std::function<ValType (const String&)> valueFromString)
    : range (detail::createRange (minimum, maximum))
    , defaultValue (static_cast<float> (defaultVal))
    , valueToText (detail::convertValToStringFuncFromTyped (stringFromValue, label))
    , textToValue (detail::convertStringToValFuncFromTyped (valueFromString))
{
	using namespace detail;

	if (valueToText == nullptr)
		valueToText = convertValToStringFuncFromTyped (createDefaultStringFromValueFunc<ValType> (range.interval, label));

	if (textToValue == nullptr)
		textToValue = convertStringToValFuncFromTyped (createDefaultValueFromStringFunc<ValType>());

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

template ParameterTraits::ParameterTraits (float, float, float, std::function<String (float, int)>, std::function<float (const String&)>);
template ParameterTraits::ParameterTraits (int, int, int, std::function<String (int, int)>, std::function<int (const String&)>);
template ParameterTraits::ParameterTraits (bool, bool, bool, std::function<String (bool, int)>, std::function<bool (const String&)>);


bool ParameterTraits::isValid() const
{
	return name.isNotEmpty();
}


namespace ParameterTraitsVTProperties
{
static constexpr auto typeProp        = "value_type";
static constexpr auto nameProp        = "name";
static constexpr auto labelProp       = "label";
static constexpr auto defaultProp     = "default_value";
static constexpr auto automatableProp = "automatable";
static constexpr auto metaProp        = "meta_parameter";
static constexpr auto categoryProp    = "category";
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

	tree.appendChild (serializing::rangeToValueTree (range), nullptr);

	return tree;
}

ParameterTraits ParameterTraits::fromValueTree (const ValueTree& tree)
{
	ParameterTraits traits;

	if (! tree.hasType (valueTreeType))
		return traits;

	using namespace ParameterTraitsVTProperties;

	if (tree.hasProperty (typeProp))
		traits.valueType = static_cast<ValueType> ((int) tree.getProperty (typeProp));

	if (tree.hasProperty (nameProp))
		traits.name = tree.getProperty (nameProp).toString();

	if (tree.hasProperty (labelProp))
		traits.label = tree.getProperty (labelProp).toString();

	if (tree.hasProperty (defaultProp))
		traits.defaultValue = (float) tree.getProperty (defaultProp);

	if (tree.hasProperty (automatableProp))
		traits.isAutomatable = (bool) tree.getProperty (automatableProp);

	if (tree.hasProperty (metaProp))
		traits.isMetaParameter = (bool) tree.getProperty (metaProp);

	if (tree.hasProperty (categoryProp))
		traits.category = static_cast<ParameterCategory> ((int) tree.getProperty (categoryProp));

	const auto rangeTree = tree.getChildWithName (serializing::rangeTreeType);

	if (rangeTree.isValid())
		traits.range = serializing::valueTreeToRange<float> (rangeTree);

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
	if (! tree.hasType (valueTreeType))
		return {};

	ParameterLayout layout;

	for (int i = 0; i < tree.getNumChildren(); ++i)
	{
		const auto child = tree.getChild (i);

		if (child.hasType (ParameterTraits::valueTreeType))
		{
			auto traits = ParameterTraits::fromValueTree (child);

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
