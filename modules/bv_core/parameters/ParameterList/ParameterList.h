
#pragma once

namespace bav
{
class ParameterList  :      public SerializableData
{
public:
    ParameterList (juce::Identifier listName);
    
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

    Parameter* getParameter (juce::String parameterNameVerbose) const;

    int getNumParameters() const;
    
    void refreshAllDefaults();
    void resetAllToDefault();
    
    void doAllActions();
    
    
private:
    struct ParamHolderMetadata  :   SerializableData
    {
        ParamHolderMetadata (ParamHolderBase& h, bool internal);
        
        ParamHolderBase& holder;
        bool isInternal;
        
    private:
        void toValueTree (ValueTree& tree) const override final;
        void fromValueTree (const ValueTree& tree) override final;
    };
    
    
    virtual void toValueTree (ValueTree& tree) const override;
    virtual void fromValueTree (const ValueTree& tree) override;
    
    
    juce::Array< ParamHolderMetadata > params;
    dsp::ProcessorBase                 dummyProcessor;
};

}  // namespace bav
