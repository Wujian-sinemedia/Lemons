
#pragma once

namespace bav
{

struct ParamHolderBase
{
    virtual ~ParamHolderBase() = default;
    
    virtual void addTo (juce::AudioProcessor& processor) = 0;
    
protected:
    bool addedToProcessor = false;
};


template<typename ParameterType>
class ParameterHolder :     public ParamHolderBase
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
    
    void addTo (juce::AudioProcessor& processor) override final
    {
        processor.addParameter (param);
        ParamHolderBase::addedToProcessor = true;
    }
    
    juce::RangedAudioParameter* getParam() { return param; }
    
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
