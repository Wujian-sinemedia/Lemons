
#pragma once

namespace bav
{

struct ParamHolderBase
{
    virtual ~ParamHolderBase() = default;
    
    virtual Parameter* getParam() = 0;
    
    void addTo (juce::AudioProcessor& processor)
    {
        processor.addParameter (&getParam()->rap);
        addedToProcessor = true;
    }
    
    bool isInternal = false;
    
protected:
    bool addedToProcessor = false;
};


template<typename ParameterType>
class ParameterHolder :     public ParamHolderBase
{
public:
    template <typename... Args>
    explicit ParameterHolder (int keyID, Args&&... args)
    {
        param = new ParameterType (keyID, std::forward<Args> (args)...);
    }
    
    explicit ParameterHolder (Parameter* p)
    {
        param = std::move (p);
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
