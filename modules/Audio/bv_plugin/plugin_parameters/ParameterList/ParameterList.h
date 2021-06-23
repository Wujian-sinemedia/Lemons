
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

    ParameterList (juce::Identifier listName);
    virtual ~ParameterList() override = default;

    void addParametersTo (juce::AudioProcessor& processor);
    void addAllParametersAsInternal();

    void refreshAllDefaults();
    void resetAllToDefault();

    void sendCallbackToAllListeners();

    void setUndoManager (UndoManager& um);

    void processMidi (const juce::MidiBuffer& midiMessages);
    void processMidiMessage (const juce::MidiMessage& message);

protected:
    template < typename... Args >
    void add (ParamHolderBase& param, Args&&... args)
    {
        add (param);
        add (std::forward< Args > (args)...);
    }
    void add (ParamHolderBase& param);

    template < typename... Args >
    void add (StringProperty& param, Args&&... args)
    {
        add (param);
        add (std::forward< Args > (args)...);
    }
    void add (StringProperty& param);

    template < typename... Args >
    void addInternal (StringProperty& param, Args&&... args)
    {
        addInternal (param);
        addInternal (std::forward< Args > (args)...);
    }
    void addInternal (StringProperty& param);

    template < typename... Args >
    void addInternal (ParamHolderBase& param, Args&&... args)
    {
        addInternal (param);
        addInternal (std::forward< Args > (args)...);
    }
    void addInternal (ParamHolderBase& param);

    void setPitchbendParameter (IntParam& param);
    void setLastMovedMidiControllerNumberParameter (IntParam& param);
    void setLastMovedMidiControllerValueParameter (IntParam& param);

private:
    virtual void toValueTree (ValueTree&) final { }
    virtual void fromValueTree (const ValueTree&) final { }

    void addParameter (ParamHolderBase& param, bool isInternal);
    void addStringProperty (StringProperty& property);

    void processNewControllerMessage (int controllerNumber, int controllerValue);
    void processNewPitchwheelMessage (int pitchwheelValue);

    juce::Array< ParamHolderBase* > params;
    dsp::BasicProcessorBase         dummyProcessor;

    juce::Array< StringProperty* > strings;

    IntParameter* pitchwheelParameter;
    IntParameter* lastMovedControllerNumberParameter;
    IntParameter* lastMovedControllerValueParameter;
};


}  // namespace bav
