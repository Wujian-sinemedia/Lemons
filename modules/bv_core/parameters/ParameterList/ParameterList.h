
#pragma once

namespace bav
{
class ParameterList
{
public:
    virtual ~ParameterList() = default;

    template < typename... Args >
    void add (ParamHolderBase& param, Args&&... args)
    {
        add (param);
        add (std::forward< Args > (args)...);
    }
    void add (ParamHolderBase& param);

    template < typename... Args >
    void addInternal (ParamHolderBase& param, Args&&... args)
    {
        addInternal (param);
        addInternal (std::forward< Args > (args)...);
    }
    void addInternal (ParamHolderBase& param);

    void addParameter (ParamHolderBase& param, bool isInternal);

    void addParametersTo (juce::AudioProcessor& processor);
    void addAllParametersAsInternal();

    Parameter* getParameter (int key) const;

    int getNextKeyNumber() const;
    
    void refreshAllDefaults();
    void resetAllToDefault();
    
    void doAllActions();
    
    virtual juce::ValueTree toValueTree() const = 0;
    virtual void restoreFromValueTree (const juce::ValueTree& tree) = 0;

    
private:
    struct ParamHolderMetadata
    {
        ParamHolderMetadata (ParamHolderBase& h, bool internal)
            : holder (h), isInternal(internal)
        { }
        
        ParamHolderBase& holder;
        bool isInternal;
    };
    
    juce::Array< ParamHolderMetadata > params;
    dsp::ProcessorBase                 dummyProcessor;
};

}  // namespace bav
