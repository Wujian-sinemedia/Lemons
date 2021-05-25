
#pragma once

namespace bav
{
class ParameterList  :      public SerializableData
{
public:
    ParameterList (juce::Identifier listName);
    virtual ~ParameterList() override = default;

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

    Parameter* getParameter (const juce::String& parameterNameVerbose) const;

    int getNumParameters() const;
    
    void refreshAllDefaults();
    void resetAllToDefault();
    
    void doAllActions();
    
    
private:
    struct ParamHolderMetadata
    {
        ParamHolderMetadata (ParamHolderBase& h, bool internal);
        
        ParamHolderBase& holder;
        bool isInternal;
    };
    
    virtual void toValueTree (ValueTree& tree) override;
    virtual void fromValueTree (const ValueTree& tree) override;
    
    juce::Array< ParamHolderMetadata > params;
    dsp::ProcessorBase                 dummyProcessor;
};


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/


class ParameterListSynchronizer  :      private juce::Timer
{
public:
    explicit ParameterListSynchronizer (ParameterList& listToUse);
    
    virtual ~ParameterListSynchronizer() override;
    
    void applyChangeData (const void* data, size_t dataSize);
    
private:
    void timerCallback() override final;
    
    virtual void sendChangeData (const void* data, size_t dataSize) const = 0;
    
    ParameterList& list;
};


}  // namespace bav
