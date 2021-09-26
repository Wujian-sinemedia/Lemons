#pragma once

#include <lemons_serializing/lemons_serializing.h>


namespace bav::plugin
{
class Parameter : public juce::RangedAudioParameter, public SerializableData
{
public:
    Parameter (String paramName,
               juce::NormalisableRange< float >
                                                       paramRange,
               float                                   paramDefaultValue    = 1.f,
               std::function< String (float) >         valueToTextFuncToUse = nullptr,
               std::function< float (const String&) >  textToValueFuncToUse = nullptr,
               String                                  paramLabel           = {},
               bool                                    isAutomatable        = true,
               bool                                    metaParam            = false,
               juce::AudioProcessorParameter::Category parameterCategory    = AudioProcessorParameter::genericParameter);

    float getMax() const;
    float getMin() const;

    const juce::NormalisableRange< float >& getNormalisableRange() const final;

    int  getMidiControllerNumber() const;
    bool isMidiControllerMapped() const;
    void setMidiControllerNumber (int newControllerNumber);
    void removeMidiControllerMapping();
    void processNewControllerMessage (int controllerNumber, int controllerValue);

    void  refreshDefault();  // sets the parameter's current value to be the default value
    void  resetToDefault();  // resets the parameter's value to the default
    float getNormalizedDefault() const;
    float getDenormalizedDefault() const;
    void  setNormalizedDefault (float value);
    void  setDenormalizedDefault (float value);

    float getNormalizedValue() const;
    float getDenormalizedValue() const;
    void  setNormalizedValue (float value);
    void  setDenormalizedValue (float value);

    void beginGesture();
    void endGesture();
    bool isChanging() const;

    float normalize (float input) const;
    float denormalize (float input) const;

    void setUndoManager (UndoManager& managerToUse);

    bool isAutomatable() const final;
    bool isMetaParameter() const final;

    float getValueForText (const String& text) const final;

    String getTextForNormalizedValue (float value) const;
    String getTextForDenormalizedValue (float value) const;
    String getTextForMax() const;
    String getTextForMin() const;

    String getName (int maxLength = 0) const final;

    //==============================================================================

    struct Listener
    {
        Listener (Parameter& paramToUse);
        virtual ~Listener();

        virtual void parameterValueChanged (float newNormalizedValue);
        virtual void gestureStateChanged (bool gestureIsStarting);
        virtual void parameterDefaultChanged (float newNormalizedDefault);
        virtual void controllerNumberChanged (int newControllerNumber);

    private:
        Parameter& param;
    };

    //==============================================================================

private:
    void serialize (TreeReflector& ref) final;

    float getValue() const final;
    void  setValue (float newValue) final;
    float getDefaultValue() const final;

    void setValueInternal (float value);
    void setDefaultInternal (float value);
    void setMidiControllerInternal (int controller);

    const bool automatable, metaParameter;

    const juce::NormalisableRange< float > range;

    std::atomic< float > currentValue {1.f}, currentDefault {1.f};
    std::atomic< bool >  changing {false};
    std::atomic< int >   midiControllerNumber {-1};

    std::function< String (float) >        valueToTextFunc;
    std::function< float (const String&) > textToValueFunc;

    UndoManager* um = nullptr;

    juce::ListenerList< Listener > listeners;

    const String parameterName, valueChangeTransactionName, defaultChangeTransactionName, midiControllerChangeTransactionName;
};

}  // namespace bav::plugin
