
#pragma once

namespace lemons::plugin
{
/** A parameter that represents a gain value in decibels.
    @see FloatParameter, Parameter
 */
struct GainParameter : public FloatParameter
{
    /** Creates a new gain parameter.
        @param paramName The name of this parameter.
        @param defaultVal The default value of this parameter.
        @param parameterCategory The category of this parameter.
     */
    GainParameter (String                                  paramName,
                   float                                   defaultVal,
                   juce::AudioProcessorParameter::Category parameterCategory =
                       juce::AudioProcessorParameter::genericParameter);

    using Listener = FloatParameter::Listener;
};

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

/** A parameter that represents a boolean on/off toggle.
    @see BoolParameter, Parameter
 */
struct ToggleParameter : public BoolParameter
{
    /** Creates a new toggle parameter.
        @param paramName The name of this parameter.
        @param defaultVal Indicates whether the parameter should initially be on or off by default.
     */
    ToggleParameter (String paramName,
                     bool   defaultVal);

    using Listener = BoolParameter::Listener;
};

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

/** A parameter that represents a value in a normalized 0-1 range.
    @see PercentParameter, FloatParameter, Parameter
 */
struct FloatAmountParameter : public FloatParameter
{
    /** Creates a new float amount parameter.
        @param paramName The name of this parameter.
        @param defaultVal The default value of this parameter.
        @param parameterCategory The category of this parameter.
        @param parameterLabel An optional label to use for this parameter's units.
     */
    FloatAmountParameter (String                                  paramName,
                          float                                   defaultVal,
                          juce::AudioProcessorParameter::Category parameterCategory =
                              juce::AudioProcessorParameter::genericParameter,
                          String parameterLabel = String());

    using Listener = FloatParameter::Listener;
};

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

/** A parameter that represents a frequency in Hz.
    @see FloatParameter, Parameter
 */
struct FrequencyParameter : public FloatParameter
{
    /** Creates a new frequency parameter.
        @param paramName The name of this parameter.
        @param defaultVal The default value of this parameter.
     */
    FrequencyParameter (String paramName,
                        float  defaultVal);

    using Listener = FloatParameter::Listener;
};

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

/** A parameter that represents a MIDI pitch.
    @see IntParameter, Parameter
 */
struct MidiPitchParameter : public IntParameter
{
    /** Creates a new MIDI pitch parameter.
        @param paramName The name of this parameter.
        @param defaultVal The default value of this parameter.
     */
    MidiPitchParameter (String paramName, int defaultVal);

    using Listener = IntParameter::Listener;
};

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

/** A parameter that represents a MIDI panning value in the range 0 to 127.
    @see IntParameter, Parameter
 */
struct MidiPanParameter : public IntParameter
{
    /** Creates a new MIDI pan parameter.
        @param paramName The name of this parameter.
        @param defaultVal The default value of this parameter.
     */
    MidiPanParameter (String paramName, int defaultVal = 64);

    using Listener = IntParameter::Listener;
};

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

/** A parameter that represents a number of semitones, as an integer.
    @see IntParameter, Parameter
 */
struct SemitonesParameter : public IntParameter
{
    /** Creates a new semitones parameter.
        @param maxSemitones The maximum number of semitones that this parameter can represent.
        @param paramName The name of this parameter.
        @param defaultVal The default value of this parameter.
     */
    SemitonesParameter (int maxSemitones, String paramName, int defaultVal);

    using Listener = IntParameter::Listener;
};

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

/** A parameter that represents an integer value in the range 0-100.
    @see FloatAmountParameter, IntParameter, Parameter
 */
struct PercentParameter : public IntParameter
{
    /** Creates a new percent parameter.
        @param paramName The name of this parameter.
        @param defaultVal The default value of this parameter.
     */
    PercentParameter (String paramName,
                      int    defaultVal);

    using Listener = IntParameter::Listener;
};

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

/** A parameter that represents a time value in seconds, expressed as a float.
    @see FloatParameter, Parameter
 */
struct SecondsParameter : public FloatParameter
{
    /** Creates a new seconds parameter.
        @param maxSeconds The maximum number of seconds that this parameter can represent.
        @param paramName The name of this parameter.
        @param defaultVal The default value of this parameter.
     */
    SecondsParameter (float maxSeconds, String paramName, float defaultVal);

    using Listener = FloatParameter::Listener;
};

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

/** A parameter that represents a general kind of meter, used for reporting things like level or gain reduction back to the DAW.
    This wasn't my idea. Yes, the JUCE API uses parameter objects to communicate info from the plugin back to the host. Yes, it's weird.
    @see GainMeterParameter
 */
struct MeterParameter : public FloatParameter
{
    /** Creates a new meter parameter.
        @param min The minimum possible value of this parameter.
        @param max The maximum possible value of this parameter.
        @param defaultVal The default value of this parameter.
        @param paramName The name of this parameter.
        @param stringFromValue An optional lambda that returns a textual description of a value for this parameter.
        @param valueFromString An optional lambda that converts a user input text string into a parameter value. This may never be called with meter parameters.
        @param parameterLabel An optional label to use for the parameter's units.
        @param parameterCategory The parameter's category. This is used to indicate to the DAW whether the parameter is a level meter, a gain reduction meter, etc.
     */
    MeterParameter (float min, float max, float defaultVal, String paramName,
                    std::function< String (float, int) >    stringFromValue   = nullptr,
                    std::function< float (String) >         valueFromString   = nullptr,
                    String                                  parameterLabel    = {},
                    juce::AudioProcessorParameter::Category parameterCategory = juce::AudioProcessorParameter::genericParameter);

    using Listener = FloatParameter::Listener;
};


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

/** A parameter that represents a meter displaying values in decibels.
    This can be used to report level or gain reduction.
    @see MeterParameter
 */
struct GainMeterParameter : public MeterParameter
{
    /** Creates a new gain meter parameter.
        @param paramName The name of this parameter.
        @param parameterCategory The category of this meter parameter. This is used to indicate to the DAW whether the parameter is a level meter, a gain reduction meter, etc.
     */
    GainMeterParameter (String                                  paramName,
                        juce::AudioProcessorParameter::Category parameterCategory =
                            juce::AudioProcessorParameter::genericParameter);

    using Listener = MeterParameter::Listener;
};

}  // namespace lemons::plugin
