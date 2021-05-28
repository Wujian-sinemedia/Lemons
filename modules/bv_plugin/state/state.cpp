
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
    if (pb != nullptr)
    {
        int w, h;
        pb->getSavedEditorSize (w, h);
        tree.setProperty ("editorSizeX", w, nullptr);
        tree.setProperty ("editorSizeY", h, nullptr);
    }
    
    for (auto* list : lists)
        list->serialize (tree);
}

void StateBase::fromValueTree (const ValueTree& tree)
{
    if (pb != nullptr)
    {
        pb->saveEditorSize (tree.getProperty ("editorSizeX"),
                            tree.getProperty ("editorSizeY"));
    }
    
    for (auto* list : lists)
        list->deserialize (tree);
}


}  // namespace
