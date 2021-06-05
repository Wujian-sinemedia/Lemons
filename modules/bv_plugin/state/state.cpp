
namespace bav
{
StateBase::StateBase (juce::Identifier name)
    : SerializableData (name)
{
}

void StateBase::add (ParameterList& list)
{
#if JUCE_DEBUG
    for (auto* l : lists)
        jassert (l->dataIdentifier != list.dataIdentifier);
#endif

    lists.add (&list);
}

void StateBase::addTo (dsp::ProcessorBase& p)
{
    for (auto* list : lists)
        list->addParametersTo (p);

    pb = &p;
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

void StateBase::doAllActions (bool force)
{
    for (auto* list : lists)
        list->doAllActions (force);
}

void StateBase::sendCallbackToAllListeners()
{
    for (auto* list : lists)
        list->sendCallbackToAllListeners();
}

void StateBase::setUndoManager (juce::UndoManager& um)
{
    for (auto* list : lists)
        list->setUndoManager (um);
}

void StateBase::toValueTree (ValueTree& tree)
{
    lastSavedEditorSize.serialize (tree);

    for (auto* list : lists)
        list->serialize (tree);
}

void StateBase::fromValueTree (const ValueTree& tree)
{
    lastSavedEditorSize.deserialize (tree);

    for (auto* list : lists)
        list->deserialize (tree);
}

StateBase::LastSavedEditorSize::LastSavedEditorSize(StateBase& b)
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
