
namespace bav
{
StateBase::StateBase (juce::Identifier name)
    : SerializableData (name)
{
}

void StateBase::add (ParameterList& list)
{
    lists.add (&list);
    addDataChild (list);
}

void StateBase::addTo (dsp::ProcessorBase& p)
{
    for (auto* list : lists)
        list->addParametersTo (p);
}

void StateBase::addTo (dsp::ProcessorBase* p)
{
    if (p != nullptr)
        addTo (*p);
}

void StateBase::addAllAsInternal()
{
    for (auto* list : lists)
        list->addAllParametersAsInternal();
}

void StateBase::refreshAllDefaults()
{
    for (auto* list : lists)
        list->refreshAllDefaults();
}

void StateBase::resetAllToDefault()
{
    for (auto* list : lists)
        list->resetAllToDefault();
}

void StateBase::sendCallbackToAllListeners()
{
    for (auto* list : lists)
        list->sendCallbackToAllListeners();
}

void StateBase::setUndoManager (UndoManager& um)
{
    for (auto* list : lists)
        list->setUndoManager (um);
}

void StateBase::processMidi (const juce::MidiBuffer& midiMessages)
{
    for (auto meta : midiMessages)
        processMidiMessage (meta.getMessage());
}

void StateBase::processMidiMessage (const juce::MidiMessage& message)
{
    for (auto* list : lists)
        list->processMidiMessage (message);
}

void StateBase::toValueTree (ValueTree&) { }

void StateBase::fromValueTree (const ValueTree&) { }

}  // namespace bav
