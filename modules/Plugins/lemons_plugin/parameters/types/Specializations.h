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

/** @defgroup parameter_specializations Special parameter types
	@ingroup lemons_plugin_parameters
	Special pre-made parameter classes.
 */


/** @ingroup parameter_specializations
	A parameter that represents a gain value in decibels.
	@see FloatParameter, Parameter
 */
struct GainParameter : public FloatParameter
{
	/** Creates a new gain parameter.
		@param paramName The name of this parameter.
		@param defaultVal The default value of this parameter.
		@param parameterCategory The category of this parameter.
	*/
	explicit GainParameter (const String&	  paramName,
							float			  defaultVal,
							ParameterCategory parameterCategory = ParameterCategory::genericParameter);
};

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

/** @ingroup parameter_specializations
	A parameter that represents a boolean on/off toggle.
	@see BoolParameter, Parameter
 */
struct ToggleParameter final : public BoolParameter
{
	/** Creates a new toggle parameter.
		@param paramName The name of this parameter.
		@param defaultVal Indicates whether the parameter should initially be on or off by default.
	*/
	explicit ToggleParameter (const String& paramName, bool defaultVal);
};

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

/** @ingroup parameter_specializations
	A parameter that represents a value in a normalized 0-1 range.
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
	explicit FloatAmountParameter (const String&	 paramName,
								   float			 defaultVal,
								   ParameterCategory parameterCategory = ParameterCategory::genericParameter,
								   const String&	 parameterLabel	   = {});
};

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

/** @ingroup parameter_specializations
	A parameter that represents a frequency in Hz.
	@see FloatParameter, Parameter
 */
struct FrequencyParameter final : public FloatParameter
{
	/** Creates a new frequency parameter.
		@param paramName The name of this parameter.
		@param defaultVal The default value of this parameter.
	*/
	explicit FrequencyParameter (const String& paramName, float defaultVal);
};

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

/** @ingroup parameter_specializations
	A parameter that represents a MIDI pitch.
	@see IntParameter, Parameter
 */
struct MidiPitchParameter final : public IntParameter
{
	/** Creates a new MIDI pitch parameter.
		@param paramName The name of this parameter.
		@param defaultVal The default value of this parameter.
	*/
	explicit MidiPitchParameter (const String& paramName, int defaultVal);
};

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

/** @ingroup parameter_specializations
	A parameter that represents a MIDI panning value in the range 0 to 127.
	@see IntParameter, Parameter
 */
struct MidiPanParameter final : public IntParameter
{
	/** Creates a new MIDI pan parameter.
		@param paramName The name of this parameter.
		@param defaultVal The default value of this parameter.
	*/
	explicit MidiPanParameter (const String& paramName, int defaultVal = 64);
};

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

/** @ingroup parameter_specializations
	A parameter that represents a number of semitones, as an integer.
	@see IntParameter, Parameter
 */
struct SemitonesParameter : public IntParameter
{
	/** Creates a new semitones parameter.
		@param maxSemitones The maximum number of semitones that this parameter can represent.
		@param paramName The name of this parameter.
		@param defaultVal The default value of this parameter.
	*/
	explicit SemitonesParameter (const String& paramName, int minSemitones = 0, int maxSemitones = 12, int defaultVal = 1);
};

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

/** @ingroup parameter_specializations
	A parameter that represents an integer value in the range 0-100.
	@see FloatAmountParameter, IntParameter, Parameter
 */
struct PercentParameter final : public IntParameter
{
	/** Creates a new percent parameter.
		@param paramName The name of this parameter.
		@param defaultVal The default value of this parameter.
	*/
	explicit PercentParameter (const String& paramName, int defaultVal);
};

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

/** @ingroup parameter_specializations
	A parameter that represents a time value in seconds, expressed as a float.
	@see FloatParameter, Parameter
 */
struct SecondsParameter : public FloatParameter
{
	/** Creates a new seconds parameter.
		@param maxSeconds The maximum number of seconds that this parameter can represent.
		@param paramName The name of this parameter.
		@param defaultVal The default value of this parameter.
	*/
	explicit SecondsParameter (const String& paramName, float minSeconds = 0.f, float maxSeconds = 1.f, float defaultVal = 0.5f);
};


}  // namespace lemons::plugin
