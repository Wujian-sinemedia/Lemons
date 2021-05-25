
namespace bav
{

ParameterList::ParameterList (juce::Identifier listName)
: SerializableData (listName)
{ }

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
    params.add ({ param, isInternal });
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

Parameter* ParameterList::getParameter (const juce::String& parameterNameVerbose) const
{
    for (auto meta : params)
    {
        auto* param = meta.holder.getParam();
        if (param->parameterNameVerbose == parameterNameVerbose)
            return param;
    }

    return nullptr;
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

void ParameterList::doAllActions()
{
    for (auto meta : params)
        meta.holder.getParam()->doAction();
}

void ParameterList::toValueTree (ValueTree& tree) const
{
    for (auto meta : params)
        meta.holder.getParam()->serialize (tree);
}

void ParameterList::fromValueTree (const ValueTree& tree)
{
    for (auto meta : params)
        meta.holder.getParam()->deserialize (tree);
    
    doAllActions();
}


/*-------------------------------------------------------------*/


ParameterList::ParamHolderMetadata::ParamHolderMetadata (ParamHolderBase& h, bool internal)
: holder (h), isInternal(internal)
{ }


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/


ParameterListSynchronizer::ParameterListSynchronizer (ParameterList& listToUse)
    : list (listToUse)
{
    Timer::startTimerHz (10);
}

ParameterListSynchronizer::~ParameterListSynchronizer()
{
    Timer::stopTimer();
}

void ParameterListSynchronizer::applyChangeData (void* data, size_t dataSize)
{
    auto newTree = juce::ValueTree::readFromData (data, dataSize);
    
    if (! newTree.isValid()) return;
    
    list.deserialize (newTree);
}

void ParameterListSynchronizer::timerCallback()
{
    juce::MemoryOutputStream m;
    
    juce::ValueTree {"ParameterListSync"};
    list.serialize (tree).writeToStream (m);
    
    sendChangeData (m.getData(), m.getDataSize());
}

}  // namespace bav
