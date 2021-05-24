
namespace bav
{
template < typename... Args >
void ParameterList::add (ParamHolderBase& param, Args&&... args)
{
    add (param);
    add (std::forward< Args > (args)...);
}

void ParameterList::add (ParamHolderBase& param)
{
    addParameter (param, true);
}

template < typename... Args >
void ParameterList::addInternal (ParamHolderBase& param, Args&&... args)
{
    addInternal (param);
    addInternal (std::forward< Args > (args)...);
}

void ParameterList::addInternal (ParamHolderBase& param)
{
    addParameter (param, false);
}

void ParameterList::addParameter (ParamHolderBase& param, bool isInternal)
{
    auto* parameter = param.getParam();

    if (auto* f = dynamic_cast< FloatParameter* > (parameter))
        params.add (new FloatParam (f, isInternal));
    else if (auto* i = dynamic_cast< IntParameter* > (parameter))
        params.add (new IntParam (i, isInternal));
    else if (auto* b = dynamic_cast< BoolParameter* > (parameter))
        params.add (new BoolParam (b, isInternal));
    else
        jassertfalse;  // if you hit this assertion, you're attempting to add a parameter that is not derived from the FloatParam, IntParam, or BoolParam classes!
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
    for (auto* holder : params)
    {
        auto* param = holder->getParam();
        if (param->key == key)
            return param;
    }

    return nullptr;
}

int ParameterList::getNextKeyNumber() const
{
    int highestKey = 0;

    for (auto* holder : params)
    {
        auto* param = holder->getParam();
        if (param->key > highestKey)
            highestKey = param->key;
    }

    return ++highestKey;
}

}  // namespace bav
