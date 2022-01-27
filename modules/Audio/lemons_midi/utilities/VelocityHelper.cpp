
namespace lemons::midi
{

float getGainMultFromMidiVelocity (float midiVelocity, float sensitivity)
{
	jassert (midiVelocity >= 0.0f && midiVelocity <= 1.0f && sensitivity >= 0.0f && sensitivity <= 1.0f);
	return (1.0f - midiVelocity) * (1.0f - sensitivity) + midiVelocity;
}

float getGainMultFromMidiVelocity (int midiVelocity, float sensitivity)
{
	jassert (midiVelocity >= 0 && midiVelocity <= 127);
	constexpr auto inv127 = 1.0f / 127.0f;
	return getGainMultFromMidiVelocity (static_cast<float> (midiVelocity) * inv127,
										sensitivity);
}


void VelocityHelper::setSensitivity (float newSensitivity)
{
	jassert (newSensitivity >= 0.0f && newSensitivity <= 1.0f);
	sensitivity = newSensitivity;
}

void VelocityHelper::setSensitivity (int newSensitivity)
{
	setSensitivity (static_cast<float> (newSensitivity) * 0.01f);
}

float VelocityHelper::getGainForVelocity (float midiVelocity)
{
	return getGainMultFromMidiVelocity (midiVelocity, sensitivity);
}

float VelocityHelper::getGainForVelocity (int midiVelocity)
{
	return getGainMultFromMidiVelocity (midiVelocity, sensitivity);
}

}  // namespace lemons::midi
