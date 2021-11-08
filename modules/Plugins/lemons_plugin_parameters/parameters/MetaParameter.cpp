
namespace lemons::plugin
{

template <typename ValueType>
MetaParameter<ValueType>::MetaParameter (//ParameterList& parameterListToUse,
                                         ValueType minimum, ValueType maximum, ValueType defaultValue,
                                         String                                   paramName,
                                         std::function<String (ValueType, int)>   stringFromValue,
                                         std::function<ValueType (const String&)> valueFromString,
                                         String paramLabel, bool isAutomatable,
                                         juce::AudioProcessorParameter::Category parameterCategory)
    : TypedParameter<ValueType> (minimum, maximum, defaultValue, paramName, stringFromValue, valueFromString, paramLabel, isAutomatable, true, parameterCategory)
    //, parameterList (parameterListToUse)
{
}

template <typename ValueType>
void MetaParameter<ValueType>::removeInvalidConnections()
{
	for (const auto& connection : connections)
		if (connection.parameter == nullptr)
			connections.remove (&connection);
}

template <typename ValueType>
bool MetaParameter<ValueType>::hasConnection (Parameter& parameter) const
{
	return getConnection (parameter) != nullptr;
}

template <typename ValueType>
void MetaParameter<ValueType>::removeConnection (Parameter& parameter)
{
	if (auto* connection = getConnection (parameter))
		connection->parameter = nullptr;
}

template <typename ValueType>
typename MetaParameter<ValueType>::Connection* MetaParameter<ValueType>::getConnection (Parameter& parameter)
{
	for (auto& connection : connections)
		if (connection.parameter == &parameter)
			return &connection;

	return nullptr;
}

template <typename ValueType>
const typename MetaParameter<ValueType>::Connection* MetaParameter<ValueType>::getConnection (Parameter& parameter) const
{
	for (const auto& connection : connections)
		if (connection.parameter == &parameter)
			return &connection;

	return nullptr;
}

template <typename ValueType>
typename MetaParameter<ValueType>::Connection* MetaParameter<ValueType>::getInvalidConnection()
{
	for (auto& connection : connections)
		if (connection.parameter == nullptr)
			return &connection;

	return nullptr;
}

template <typename ValueType>
void MetaParameter<ValueType>::createOrEditConnection (Parameter& parameter, float minAmt, float maxAmt)
{
	jassert (&parameter != this);
	jassert (parameter.isAutomatable());

	auto assignConnection = [&] (Connection& connection)
	{
		connection.parameter = &parameter;
		connection.min       = minAmt;
		connection.max       = maxAmt;
		//connection.paramList = &parameterList;
	};

	if (auto* connection = getConnection (parameter))
	{
		assignConnection (*connection);
		return;
	}

	if (auto* connection = getInvalidConnection())
	{
		assignConnection (*connection);
		return;
	}

	Connection newConnection;
	assignConnection (newConnection);
	connections.add (newConnection);
}

template <typename ValueType>
void MetaParameter<ValueType>::createOrEditConnection (BoolParameter& parameter)
{
	createOrEditConnection (parameter, 0.f, 1.f);
}

/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/


template <typename ValueType>
MetaParameter<ValueType>::Updater::Updater (MetaParameter& paramToUse)
    : Parameter::Listener (paramToUse)
    , metaParam (paramToUse)
{
}

template <typename ValueType>
void MetaParameter<ValueType>::Updater::parameterValueChanged (float newNormalizedValue)
{
	for (auto& connection : metaParam.connections)
		connection.apply (newNormalizedValue);
}


/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/


template <typename ValueType>
void MetaParameter<ValueType>::Connection::apply (float newMetaVal)
{
	if (parameter == nullptr) return;

	parameter->setNormalizedValue (juce::jmap (newMetaVal, min, max));
}

template class MetaParameter<float>;
template class MetaParameter<int>;


/*----------------------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/


DefaultMetaParameter::DefaultMetaParameter (//ParameterList& parameterListToUse,
                                            String         paramName)
    : MetaParameter<int> (/*parameterListToUse,*/ 0, 100, 50, paramName)
{
}

}  // namespace lemons::plugin
