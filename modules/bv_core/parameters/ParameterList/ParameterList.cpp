
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
    ParamHolderMetadata.add ({ &param, isInternal });
}

void ParameterList::addParametersTo (juce::AudioProcessor& processor)
{
    for (auto* meta : params)
    {
        if (meta->holder->isInternal)
            meta->holder->addTo (processor);
        else
            meta->holder->addTo (dummyProcessor);
    }
}

void ParameterList::addAllParametersAsInternal()
{
    for (auto* meta : params)
        meta->holder->addTo (dummyProcessor);
}

Parameter* ParameterList::getParameter (int key) const
{
    for (auto* meta : params)
    {
        auto* param = meta->holder->getParam();
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
        auto* param = meta->holder->getParam();
        if (param->key > highestKey)
            highestKey = param->key;
    }

    return ++highestKey;
}

}  // namespace bav
