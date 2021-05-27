
namespace bav
{
ParameterList::ParameterList (juce::Identifier listName)
    : SerializableData (listName)
{
}

void ParameterList::add (ParamHolderBase& param)
{
    addParameter (param, true);
}

void ParameterList::addInternal (ParamHolderBase& param)
{
    addParameter (param, false);
}

void ParameterList::addParameter (ParamHolderBase& param, bool isInternal)
{
    params.add ({param, isInternal});
}

void ParameterList::addParametersTo (juce::AudioProcessor& processor)
{
    for (auto meta : params)
    {
        if (meta.holder.isInternal)
            meta.holder.addTo (processor);
        else
            meta.holder.addTo (dummyProcessor);
    }
}

void ParameterList::addAllParametersAsInternal()
{
    for (auto meta : params)
        meta.holder.addTo (dummyProcessor);
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

void ParameterList::doAllActions (bool force)
{
    for (auto meta : params)
        meta.holder.getParam()->doAction (force);
}

void ParameterList::toValueTree (ValueTree& tree)
{
    for (auto meta : params)
        meta.holder.getParam()->serialize (tree);
}

void ParameterList::fromValueTree (const ValueTree& tree)
{
    for (auto meta : params)
        meta.holder.getParam()->deserialize (tree);
}

void ParameterList::setUndoManager (juce::UndoManager& um)
{
    for (auto meta : params)
        meta.holder.getParam()->setUndoManager (um);
}


/*-------------------------------------------------------------*/


ParameterList::ParamHolderMetadata::ParamHolderMetadata (ParamHolderBase& h, bool internal)
    : holder (h), isInternal (internal)
{
}


}  // namespace bav
