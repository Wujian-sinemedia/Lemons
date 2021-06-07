
#pragma once

namespace bav
{
class ParameterList : public SerializableData
{
public:
    using IntParam     = bav::IntParam;
    using FloatParam   = bav::FloatParam;
    using BoolParam    = bav::BoolParam;
    using GainParam    = bav::GainParam;
    using ToggleParam  = bav::ToggleParam;
    using PercentParam = bav::PercentParam;
    using GainMeter    = bav::ParameterHolder< bav::GainMeterParameter >;

    //--------------------------------------------------

    ParameterList (juce::Identifier listName);
    virtual ~ParameterList() override = default;

    //--------------------------------------------------
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
    //--------------------------------------------------

    void addParametersTo (juce::AudioProcessor& processor);
    void addAllParametersAsInternal();
    
    void setPitchbendParameter (IntParam& param);

    int getNumParameters() const;

    void refreshAllDefaults();
    void resetAllToDefault();

    void doAllActions (bool force = true);

    void sendCallbackToAllListeners();

    void setUndoManager (juce::UndoManager& um);

    void processMidi (const juce::MidiBuffer& midiMessages, bool triggerActions = false);
    void processMidiMessage (const juce::MidiMessage& message, bool triggerAction = false);

private:
    struct ParamHolderMetadata
    {
        ParamHolderMetadata (ParamHolderBase& h, bool internal);

        ParamHolderBase& holder;
        bool             isInternal;
    };

    virtual void toValueTree (ValueTree& tree) override;
    virtual void fromValueTree (const ValueTree& tree) override;

    void processNewControllerMessage (int controllerNumber, int controllerValue, bool triggerAction);
    void processNewPitchwheelMessage (int pitchwheelValue, bool triggerAction);

    juce::Array< ParamHolderMetadata > params;
    dsp::BasicProcessorBase            dummyProcessor;
    
    IntParameter* pitchwheelParameter;
};


}  // namespace bav
