
namespace bav
{
ParameterList::ParameterList (juce::Identifier listName)
    : SerializableData (listName)
{
}

void ParameterList::add (ParamHolderBase& param)
{
    addParameter (param, false);
}

void ParameterList::addInternal (ParamHolderBase& param)
{
    addParameter (param, true);
}

void ParameterList::addParameter (ParamHolderBase& param, bool isInternal)
{
    param.isInternal = isInternal;
    params.add (&param);
    addDataChild (param.getParam());
}

void ParameterList::addParametersTo (juce::AudioProcessor& processor)
{
    for (auto* holder : params)
    {
        if (holder->isInternal)
            holder->addTo (processor);
        else
            holder->addTo (dummyProcessor);

        holder->getParam()->sendListenerSyncCallback();
    }
}

void ParameterList::addAllParametersAsInternal()
{
    for (auto* holder : params)
        holder->addTo (dummyProcessor);

    sendCallbackToAllListeners();
}

void ParameterList::setPitchbendParameter (IntParam& param)
{
    pitchwheelParameter = param.get();
}

void ParameterList::sendCallbackToAllListeners()
{
    for (auto* holder : params)
        holder->getParam()->sendListenerSyncCallback();
}

int ParameterList::getNumParameters() const
{
    return params.size();
}

void ParameterList::refreshAllDefaults()
{
    for (auto* holder : params)
        holder->getParam()->refreshDefault();
}

void ParameterList::resetAllToDefault()
{
    for (auto* holder : params)
        holder->getParam()->resetToDefault();
}

void ParameterList::setUndoManager (UndoManager& um)
{
    for (auto* holder : params)
        holder->getParam()->setUndoManager (um);
}

void ParameterList::processMidi (const juce::MidiBuffer& midiMessages)
{
    for (auto meta : midiMessages)
        processMidiMessage (meta.getMessage());
}

void ParameterList::processMidiMessage (const juce::MidiMessage& message)
{
    if (message.isController())
        processNewControllerMessage (message.getControllerNumber(), message.getControllerValue());
    else if (message.isPitchWheel())
        processNewPitchwheelMessage (message.getPitchWheelValue());
}

void ParameterList::processNewControllerMessage (int controllerNumber, int controllerValue)
{
    for (auto* holder : params)
        holder->getParam()->processNewControllerMessage (controllerNumber, controllerValue);
}

void ParameterList::processNewPitchwheelMessage (int pitchwheelValue)
{
    if (auto* param = pitchwheelParameter)
    {
        param->set (juce::jmap (pitchwheelValue, 0, 16383, param->getMinimum(), param->getMaximum()));
    }
}

}  // namespace bav
