
#pragma once

namespace bav
{

struct ParamHolderBase
{
    virtual ~ParamHolderBase() = default;
    
    Parameter* getParam() = 0;
    
    void addTo (juce::AudioProcessor& processor)
    {
        processor.addParameter (param);
        addedToProcessor = true;
    }
    
    bool isInternal;
    
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
    
    Parameter* getParam() override final { return param; }
    
    operator ParameterType&() { return &param; }
    ParameterType* operator->() { return param; }
    ParameterType& operator*() { return *param; }
    
private:
    ParameterType* param;
};


using FloatParam = ParameterHolder<FloatParameter>;
using IntParam   = ParameterHolder<IntParameter>;
using BoolParam  = ParameterHolder<BoolParameter>;


}  // namespace
