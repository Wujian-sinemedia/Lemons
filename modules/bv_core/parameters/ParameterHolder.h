
#pragma once

namespace bav
{

struct ParamHolderBase
{
    virtual ~ParamHolderBase() = default;
    
    virtual juce::RangedAudioParameter* getParam() = 0;
    
    void addTo (juce::AudioProcessor& processor)
    {
        processor.addParameter (getParam());
        addedToProcessor = true;
    }
    
protected:
    bool addedToProcessor = false;
};


template<typename ParameterType>
class ParameterHolder   final
{
public:
    template <typename... Args>
    explicit ParameterHolder (Args&&... args)
    {
        param = new ParameterType (std::forward<Args> (args)...);
    }
    
    ~ParameterHolder() override
    {
        if (! ParamHolderBase::addedToProcessor)
            delete param;
    }
    
    juce::RangedAudioParameter* getParam() override final { return param; }
    
    operator ParameterType&() { return &param; }
    ParameterType* operator->() { return param; }
    ParameterType& operator*() { return *param; }
    
private:
    ParameterType* param;
};


using IntParam = ParameterHolder<FloatParameter>;
using FloatParam = ParameterHolder<IntParameter>;
using BoolParam = ParameterHolder<BoolParameter>;


}  // namespace
