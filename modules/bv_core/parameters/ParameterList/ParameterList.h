
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
    
    int getNumParameters() const;
    
    void refreshAllDefaults();
    void resetAllToDefault();
    
    void doAllActions();
    
    virtual juce::ValueTree toValueTree() const;
    virtual void restoreFromValueTree (const juce::ValueTree& tree);

    
private:
    struct ParamHolderMetadata  :   SerializableData
    {
        ParamHolderMetadata (ParamHolderBase& h, bool internal)
            : holder (h), isInternal(internal)
        { }
        
        ParamHolderBase& holder;
        bool isInternal;
        
    private:
        void toValueTree (ValueTree& tree) const override final
        {
            holder.getParam()->serialize (tree);
        }
        
        void fromValueTree (const ValueTree& tree) override final
        {
            holder.getParam()->deserialize (tree);
        }
    };
    
    juce::Array< ParamHolderMetadata > params;
    dsp::ProcessorBase                 dummyProcessor;
};

}  // namespace bav
