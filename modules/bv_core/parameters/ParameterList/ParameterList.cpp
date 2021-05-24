
namespace bav
{

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

Parameter* ParameterList::getParameter (int key) const
{
    for (auto meta : params)
    {
        auto* param = meta.holder.getParam();
        if (param->key == key)
            return param;
    }

    return nullptr;
}

int ParameterList::getNextKeyNumber() const
{
    int highestKey = 0;

    for (auto meta : params)
    {
        auto* param = meta.holder.getParam();
        if (param->key > highestKey)
            highestKey = param->key;
    }

    return ++highestKey;
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

juce::ValueTree ParameterList::toValueTree() const
{
    juce::ValueTree tree {"Parameters"};
    
    for (auto meta : params)
        tree.appendChild (meta.holder.getParam()->toValueTree());
    
    return tree;
}

void ParameterList::restoreFromValueTree (const juce::ValueTree& tree)
{
    for (auto meta : params)
        meta.holder.getParam().restoreFromValueTree (tree);
}

}  // namespace bav
