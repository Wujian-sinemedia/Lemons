
namespace bav
{

template <typename... Args>
void ParameterList::add (ParamHolderBase& param, Args&&... args)
{
    add (param);
    add (std::forward<Args> (args)...);
}

void ParameterList::add (ParamHolderBase& param)
{
    addParameter (param, true);
}

template <typename... Args>
void ParameterList::addInternal (ParamHolderBase& param, Args&&... args)
{
    addInternal (param);
    addInternal (std::forward<Args> (args)...);
}

void ParameterList::addInternal (ParamHolderBase& param)
{
    addParameter (param, false);
}

void ParameterList::addParameter (ParamHolderBase& param, bool isInternal)
{
    param.isInternal = isInternal;
    internal_add (param.getParam());
}

void ParameterList::addParametersTo (juce::AudioProcessor& processor)
{
    for (auto* param : params)
    {
        if (param->isInternal)
            param->addTo (dummyProcessor);
        else
            param->addTo (processor);
    }
}

void ParameterList::addAllParametersAsInternal()
{
    for (auto* param : params)
        param->addTo (dummyProcessor);
}

Parameter* ParameterList::getParameter (int key) const
{
    for (auto* param : params)
        if (param->getParam()->key == key)
            return param;
    
    return nullptr;
}

int ParameterList::getNextKeyNumber() const
{
    int highestKey = 0;
    
    for (auto* param : params)
        if (param->getParam()->key > highestKey)
            highestKey = param->key;
    
    return ++highestKey;
}

void ParameterList::internal_add (FloatParameter* param)
{
    params.add (new FloatParam (param));
}

void ParameterList::internal_add (IntParameter* param)
{
    params.add (new IntParam (param));
}

void ParameterList::internal_add (BoolParameter* param)
{
    params.add (new BoolParam (param));
}

}  // namespace
