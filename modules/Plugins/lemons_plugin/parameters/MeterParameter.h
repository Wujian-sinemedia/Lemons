/*
 ======================================================================================

 ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝

 This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.

 ======================================================================================
 */

#pragma once

namespace lemons::plugin
{

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
	                std::function<String (float, int)>      stringFromValue   = nullptr,
	                std::function<float (String)>           valueFromString   = nullptr,
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
