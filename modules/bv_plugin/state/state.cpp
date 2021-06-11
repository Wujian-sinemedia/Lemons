
namespace bav
{
StateBase::StateBase (juce::Identifier name)
    : SerializableData (name)
{
    addDataChild (lastSavedEditorSize, true);
}

void StateBase::add (ParameterList& list)
{
    addList (list, false);
}

void StateBase::addInternal (ParameterList& list)
{
    addList (list, true);
}

void StateBase::addList (ParameterList& list, bool excludedFromPresets)
{
    lists.add (&list);
    addDataChild (list, excludedFromPresets);
}

void StateBase::addTo (dsp::ProcessorBase& p)
{
    for (auto* list : lists)
        list->addParametersTo (p);

    pb = &p;
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

    pb = nullptr;
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

StateBase::LastSavedEditorSize::LastSavedEditorSize (StateBase& b)
    : SerializableData ("LastSavedEditorSize"),
      base (b)
{
}

void StateBase::LastSavedEditorSize::toValueTree (ValueTree& tree)
{
    if (auto* processor = base.pb)
    {
        int w, h;
        processor->getSavedEditorSize (w, h);
        tree.setProperty ("editorSizeX", w, nullptr);
        tree.setProperty ("editorSizeY", h, nullptr);
    }
}

void StateBase::LastSavedEditorSize::fromValueTree (const ValueTree& tree)
{
    if (auto* processor = base.pb)
    {
        processor->saveEditorSize (tree.getProperty ("editorSizeX"),
                                   tree.getProperty ("editorSizeY"));
    }
}


}  // namespace bav
