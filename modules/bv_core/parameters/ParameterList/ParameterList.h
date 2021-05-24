
#pragma once

namespace bav
{
class ParameterList
{
public:
    virtual ~ParameterList() = default;

    template < typename... Args >
    void add (ParamHolderBase& param, Args&&... args);
    void add (ParamHolderBase& param);

    template < typename... Args >
    void addInternal (ParamHolderBase& param, Args&&... args);
    void addInternal (ParamHolderBase& param);

    void addParameter (ParamHolderBase& param, bool isInternal);

    void addParametersTo (juce::AudioProcessor& processor);
    void addAllParametersAsInternal();

    Parameter* getParameter (int key) const;

    int getNextKeyNumber() const;

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
