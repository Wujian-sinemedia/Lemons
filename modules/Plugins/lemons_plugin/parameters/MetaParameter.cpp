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

MetaParameterBase::MetaParameterBase (const ParameterList& parameterListToUse)
    : parameterList (parameterListToUse)
{
}

void MetaParameterBase::applyAllConnections() const
{
	const auto newValue = getParameter().getNormalizedValue();

	for (const auto* connection : connections)
		if (connection->isMapped())
			connection->apply (newValue);
}

MetaParameterBase::Connection& MetaParameterBase::getConnectionToAssign (const Parameter& parameter)
{
	if (auto* connection = getConnection (parameter))
		return *connection;

	if (auto* connection = getInvalidConnection())
		return *connection;

	return *connections.add (new Connection (parameterList));
}

MetaParameterBase::Connection* MetaParameterBase::getConnection (const Parameter& parameter)
{
	for (auto* connection : connections)
		if (connection->isMappedTo (parameter))
			return connection;

	return nullptr;
}

const MetaParameterBase::Connection* MetaParameterBase::getConnection (const Parameter& parameter) const
{
	for (const auto* connection : connections)
		if (connection->isMappedTo (parameter))
			return connection;

	return nullptr;
}

MetaParameterBase::Connection* MetaParameterBase::getInvalidConnection()
{
	for (auto* connection : connections)
		if (! connection->isMapped())
			return connection;

	return nullptr;
}

void MetaParameterBase::createOrEditConnection (Parameter& parameter, float minAmt, float maxAmt)
{
	jassert (&parameter != &getParameter());

	if (! parameter.isAutomatable())
		return;

	getConnectionToAssign (parameter).editMapping (parameter, minAmt, maxAmt);
}

bool MetaParameterBase::removeConnection (const Parameter& parameter)
{
	if (auto* connection = getConnection (parameter))
	{
		connection->removeMapping();
		return true;
	}

	return false;
}

bool MetaParameterBase::hasConnection (const Parameter& parameter) const
{
	return getConnection (parameter) != nullptr;
}

ValueTree MetaParameterBase::saveConnectionsToValueTree() const
{
	ValueTree tree { valueTreeType };

	for (const auto* connection : connections)
		if (connection->isMapped())
			tree.appendChild (connection->saveToValueTree(), nullptr);

	return tree;
}

void MetaParameterBase::loadConnectionsFromValueTree (const ValueTree& tree)
{
	if (! tree.hasType (valueTreeType))
		return;

	connections.clearQuick (true);

	for (int i = 0; i < tree.getNumChildren(); ++i)
	{
		const auto child = tree.getChild (i);

		if (child.hasType (Connection::valueTreeType))
		{
			auto* connection = connections.add (new Connection (parameterList));

			connection->loadFromValueTree (child);
		}
	}
}


/*-------------------------------------------------------------------------------------------------------------------------------*/


namespace ConnectionVTproperties
{
static constexpr auto parameterName = "parameter_name";
static constexpr auto minAmount     = "minimum_amount";
static constexpr auto maxAmount     = "maximum_amount";
}  // namespace ConnectionVTproperties

MetaParameterBase::Connection::Connection (const ParameterList& list)
    : paramList (list)
{
}

void MetaParameterBase::Connection::apply (float newMetaVal) const
{
	if (parameter == nullptr) return;

	parameter->setNormalizedValue (juce::jmap (newMetaVal, min, max));
}

ValueTree MetaParameterBase::Connection::saveToValueTree() const
{
	jassert (isMapped());

	ValueTree tree { valueTreeType };

	using namespace ConnectionVTproperties;

	if (parameter != nullptr)
		tree.setProperty (parameterName, parameter->getParameterName(), nullptr);

	tree.setProperty (minAmount, min, nullptr);
	tree.setProperty (maxAmount, max, nullptr);

	return tree;
}

void MetaParameterBase::Connection::loadFromValueTree (const ValueTree& tree)
{
	if (! tree.hasType (valueTreeType))
		return;

	using namespace ConnectionVTproperties;

	parameter = nullptr;

	if (tree.hasProperty (parameterName))
	{
		if (auto* param = paramList.getNamedParameter (tree.getProperty (parameterName).toString()))
		{
			if (! param->isAutomatable())
				return;

			parameter = param;
		}
	}

	if (parameter == nullptr)
		return;

	if (tree.hasProperty (minAmount))
		min = (float) tree.getProperty (minAmount);

	if (tree.hasProperty (maxAmount))
		max = (float) tree.getProperty (maxAmount);
}

bool MetaParameterBase::Connection::isMapped() const noexcept
{
	return parameter != nullptr;
}

bool MetaParameterBase::Connection::isMappedTo (const Parameter& param) const
{
	return parameter == (&param);
}

void MetaParameterBase::Connection::editMapping (Parameter& param, float minimum, float maximum)
{
	jassert ((! isMapped()) || (isMappedTo (param)));
	jassert (param.isAutomatable());

	parameter = &param;
	min       = minimum;
	max       = maximum;
}

void MetaParameterBase::Connection::removeMapping()
{
	parameter = nullptr;
}


/*-------------------------------------------------------------------------------------------------------------------------------*/


template <typename ValueType>
MetaParameter<ValueType>::MetaParameter (
    const ParameterList& parameterListToUse,
    ValueType minimum, ValueType maximum, ValueType defaultValue,
    const String&                            paramName,
    std::function<String (ValueType, int)>   stringFromValue,
    std::function<ValueType (const String&)> valueFromString,
    const String& paramLabel, bool isAutomatable,
    ParameterCategory parameterCategory)
    : TypedParameter<ValueType> (minimum, maximum, defaultValue, paramName, stringFromValue, valueFromString, paramLabel, isAutomatable, true, parameterCategory)
    , MetaParameterBase (parameterListToUse)
{
}

template <typename ValueType>
MetaParameter<ValueType>::MetaParameter (const ParameterTraits& traits, const ParameterList& parameterListToUse)
    : MetaParameter (
        parameterListToUse,
        static_cast<ValueType> (traits.range.start), static_cast<ValueType> (traits.range.end), static_cast<ValueType> (traits.defaultValue), traits.name,
        detail::convertValToStringFuncToTyped<ValueType> (traits.valueToText, traits.label),
        detail::convertStringToValFuncToTyped<ValueType> (traits.textToValue),
        traits.label,
        traits.isAutomatable,
        traits.category)
{
}

template <typename ValueType>
ValueTree MetaParameter<ValueType>::saveToValueTree() const
{
	auto tree = TypedParameter<ValueType>::saveToValueTree();

	tree.appendChild (saveConnectionsToValueTree(), nullptr);

	return tree;
}

template <typename ValueType>
void MetaParameter<ValueType>::loadFromValueTree (const ValueTree& tree)
{
	if (! tree.hasType (Parameter::valueTreeType))
		return;

	TypedParameter<ValueType>::loadFromValueTree (tree);

	loadConnectionsFromValueTree (tree.getChildWithName (MetaParameterBase::valueTreeType));
}

template <typename ValueType>
Parameter& MetaParameter<ValueType>::getParameter()
{
	return *this;
}

template <typename ValueType>
const Parameter& MetaParameter<ValueType>::getParameter() const
{
	return *this;
}

/*----------------------------------------------------------------------------------------------------------------*/


template <typename ValueType>
MetaParameter<ValueType>::Updater::Updater (MetaParameter& paramToUse)
    : Parameter::Listener (paramToUse)
    , metaParam (paramToUse)
{
}

template <typename ValueType>
void MetaParameter<ValueType>::Updater::parameterValueChanged (float)
{
	metaParam.applyAllConnections();
}


template class MetaParameter<float>;
template class MetaParameter<int>;
template class MetaParameter<bool>;

}  // namespace lemons::plugin
