/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 * ======================================================================================
 */

#include <lemons_musicology/lemons_musicology.h>

namespace lemons::plugin
{
static const auto
	gain_stringFromFloat = [] (float value, int maxLength) -> String
{
	return (String (value) + " " + TRANS ("dB")).substring (0, maxLength);
};

static const auto gain_floatFromString =
	[] (const String& text) -> float
{
	const auto token_location = text.indexOfWholeWordIgnoreCase (TRANS ("dB"));

	if (token_location > -1)
		return text.substring (0, token_location).trim().getFloatValue();

	return text.trim().getFloatValue();
};


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

GainParameter::GainParameter (const String&		paramName,
							  float				defaultVal,
							  ParameterCategory parameterCategory)
	: FloatParameter (-60.f, 0.f, defaultVal,
					  paramName,
					  gain_stringFromFloat,
					  gain_floatFromString,
					  TRANS ("dB"), true, false, parameterCategory)
{
}


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

static const auto
	toggle_stringFromBool = [] (bool value, int maxLength) -> String
{
	return value ? TRANS ("On").substring (0, maxLength)
				 : TRANS ("Off").substring (0, maxLength);
};

static const auto toggle_boolFromString =
	[] (const String& text) -> bool
{
	return (text.containsIgnoreCase (TRANS ("On"))
			|| text.containsIgnoreCase (TRANS ("Yes"))
			|| text.containsIgnoreCase (TRANS ("True")));
};

ToggleParameter::ToggleParameter (const String& paramName,
								  bool			defaultVal)
	: BoolParameter (defaultVal,
					 paramName,
					 toggle_stringFromBool,
					 toggle_boolFromString)
{
}

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

static const auto
	normPcnt_stringFromFloat = [] (float value, int maxLength) -> String
{
	return (String (value * 100.0f) + "%").substring (0, maxLength);
};

static const auto normPcnt_floatFromString =
	[] (const String& text) -> float
{
	const auto token_location = text.indexOf ("%");

	if (token_location > -1)
		return text.substring (0, token_location).trim().getFloatValue() * 0.01f;

	return text.trim().getFloatValue();
};

FloatAmountParameter::FloatAmountParameter (const String&	  paramName,
											float			  defaultVal,
											ParameterCategory parameterCategory,
											const String&	  parameterLabel)
	: FloatParameter (0.f, 1.f, defaultVal,
					  paramName,
					  normPcnt_stringFromFloat,
					  normPcnt_floatFromString,
					  parameterLabel, true, false, parameterCategory)
{
}

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

static const auto
	hz_stringFromFloat = [] (float value, int maxLength) -> String
{
	auto string = (value < 1000.0f)
					? String (value) + " " + TRANS ("Hz")
					: String (value * 0.001f) + " " + TRANS ("kHz");

	return string.substring (0, maxLength);
};

static const auto hz_floatFromString =
	[] (const String& text) -> float
{
	const auto kHz_token_location = text.indexOfWholeWordIgnoreCase (TRANS ("kHz"));

	if (kHz_token_location > -1)
		return text.substring (0, kHz_token_location).trim().getFloatValue()
			 * 1000.0f;

	const auto hz_token_location = text.indexOfWholeWordIgnoreCase (TRANS ("Hz"));

	if (hz_token_location > -1)
		return text.substring (0, hz_token_location).trim().getFloatValue();

	return text.trim().getFloatValue();
};

FrequencyParameter::FrequencyParameter (const String& paramName,
										float		  defaultVal)
	: FloatParameter (40.f, 10000.f, defaultVal,
					  paramName,
					  hz_stringFromFloat,
					  hz_floatFromString,
					  TRANS ("Hz"))
{
}

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

using namespace lemons::music;

static const auto
	pitch_stringFromInt = [] (int value, int maxLength) -> String
{
	return pitchToString (value, true).substring (0, maxLength);
};

static const auto pitch_intFromString =
	[] (const String& text) -> int
{
	static const auto pitchClassTokens =
		String ("AaBbCcDdEeFfGg#") + getSharpSymbol() + getFlatSymbol();

	if (text.containsAnyOf (pitchClassTokens)) return stringToPitch (text.trim());

	return text.trim().getIntValue();
};


MidiPitchParameter::MidiPitchParameter (const String& paramName, int defaultVal)
	: IntParameter (0, 127, defaultVal, paramName, pitch_stringFromInt, pitch_intFromString)
{
}

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

static const auto
	midiPan_stringFromInt = [] (int value, int maxLength) -> String
{
	return midiPanIntToString (value).substring (0, maxLength);
};

static const auto midiPan_intFromString =
	[] (const String& text) -> int
{
	return midiPanStringToInt (text);
};

MidiPanParameter::MidiPanParameter (const String& paramName, int defaultVal)
	: IntParameter (0, 127, defaultVal, paramName,
					midiPan_stringFromInt,
					midiPan_intFromString)
{
}

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

static const auto
	st_stringFromInt = [] (int value, int maxLength) -> String
{
	return (String (value) + " " + TRANS ("st")).substring (0, maxLength);
};

static auto st_intFromString =
	[] (const String& text) -> int
{
	const auto token_location = text.indexOfWholeWordIgnoreCase (TRANS ("st"));

	if (token_location > -1)
		return text.substring (0, token_location).trim().getIntValue();

	return text.trim().getIntValue();
};

SemitonesParameter::SemitonesParameter (const String& paramName, int minSemitones, int maxSemitones, int defaultVal)
	: IntParameter (minSemitones, maxSemitones, defaultVal, paramName,
					st_stringFromInt,
					st_intFromString,
					"semitones")
{
}

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

static const auto
	pcnt_stringFromInt = [] (int value, int maxLength) -> String
{
	return (String (value) + "%").substring (0, maxLength);
};

static const auto pcnt_intFromString =
	[] (const String& text) -> int
{
	const auto token_location = text.indexOf ("%");

	if (token_location > -1)
		return text.substring (0, token_location).trim().getIntValue();

	return text.trim().getIntValue();
};

PercentParameter::PercentParameter (const String& paramName,
									int			  defaultVal)
	: IntParameter (0, 100, defaultVal,
					paramName,
					pcnt_stringFromInt,
					pcnt_intFromString,
					"%")
{
}

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

static const auto
	sec_stringFromFloat = [] (float value, int maxLength) -> String
{
	return (String (value) + " " + TRANS ("sec")).substring (0, maxLength);
};

static const auto sec_floatFromString =
	[] (const String& text) -> float
{
	const auto token_location = text.indexOfWholeWordIgnoreCase (TRANS ("sec"));

	if (token_location > -1)
		return text.substring (0, token_location).trim().getFloatValue();

	return text.trim().getFloatValue();
};

SecondsParameter::SecondsParameter (const String& paramName, float minSeconds, float maxSeconds, float defaultVal)
	: FloatParameter (minSeconds, maxSeconds, defaultVal, paramName,
					  sec_stringFromFloat, sec_floatFromString,
					  "sec")
{
}

}  // namespace lemons::plugin
