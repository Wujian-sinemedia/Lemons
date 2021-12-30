#pragma once

namespace lemons::plugin
{

struct ParameterHolderBase
{
public:
    virtual ~ParameterHolderBase() = default;
    
    [[nodiscard]] virtual Parameter* getParameter() const = 0;
    
    void addTo (juce::AudioProcessor& processor)
    {
        processor.addParameter (getParameter());
        addedToProcessor = true;
    }
    
    Parameter* operator->() { return getParameter(); }
    
    const Parameter* operator->() const { return getParameter(); }
    
protected:
    bool addedToProcessor { false };
};


template<typename ParameterType, LEMONS_MUST_INHERIT_FROM (ParameterType, Parameter)>
class ParameterHolder final : public ParameterHolderBase
{
public:
    
    template<typename... Args>
    explicit ParameterHolder (Args&&... args)
    {
        param = new ParameterType (std::forward<Args>(args)...);
    }
    
    ~ParameterHolder()
    {
        if (! addedToProcessor)
            delete param;
    }
    
    [[nodiscard]] Parameter* getParameter() const { return param; }
    
    [[nodiscard]] ParameterType* get() const { return param; }
    
    ParameterType* operator->() { return param; }
    
private:
    ParameterType* param;
};

}
