
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
    params.add ({param, isInternal});
    addDataChild (param.getParam());
}

void ParameterList::addParametersTo (juce::AudioProcessor& processor)
{
    for (auto meta : params)
    {
        if (meta.holder.isInternal)
            meta.holder.addTo (processor);
        else
            meta.holder.addTo (dummyProcessor);

        meta.holder.getParam()->sendListenerSyncCallback();
    }
}

void ParameterList::addAllParametersAsInternal()
{
    for (auto meta : params)
        meta.holder.addTo (dummyProcessor);

    sendCallbackToAllListeners();
}

void ParameterList::setPitchbendParameter (IntParam& param)
{
    pitchwheelParameter = param.get();
}

void ParameterList::sendCallbackToAllListeners()
{
    for (auto meta : params)
        meta.holder.getParam()->sendListenerSyncCallback();
}

int ParameterList::getNumParameters() const
{
    return params.size();
}

void ParameterList::refreshAllDefaults()
{
    for (auto meta : params)
        meta.holder.getParam()->refreshDefault();
}

void ParameterList::resetAllToDefault()
{
    for (auto meta : params)
        meta.holder.getParam()->resetToDefault();
}

void ParameterList::toValueTree (ValueTree&) { }

void ParameterList::fromValueTree (const ValueTree&) { }

void ParameterList::setUndoManager (UndoManager& um)
{
    for (auto meta : params)
        meta.holder.getParam()->setUndoManager (um);
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
    for (auto meta : params)
        meta.holder.getParam()->processNewControllerMessage (controllerNumber, controllerValue);
}

void ParameterList::processNewPitchwheelMessage (int pitchwheelValue)
{
    if (auto* param = pitchwheelParameter)
    {
        param->set (juce::jmap (pitchwheelValue, 0, 16383, 0, 127));
    }
}


/*-------------------------------------------------------------*/


ParameterList::ParamHolderMetadata::ParamHolderMetadata (ParamHolderBase& h, bool internal)
    : holder (h), isInternal (internal)
{
}


}  // namespace bav
