
namespace lemons::midi
{


void copyRangeOfMidiBuffer (const MidiBuffer& readingBuffer,
                            MidiBuffer&       destBuffer,
                            const int         startSampleOfInput,
                            const int         startSampleOfOutput,
                            const int         numSamples)
{
	if (numSamples == 0) return;

	destBuffer.clear (startSampleOfOutput, numSamples);
	destBuffer.addEvents (readingBuffer,
	                      startSampleOfInput,
	                      numSamples,
	                      startSampleOfOutput - startSampleOfInput);
}


String getControllerName (int controllerNumber)
{
	jassert (controllerNumber >= 0 && controllerNumber <= 127);

	static constexpr const char* controllerNames[128] = {
		"Bank Select", "Modulation Wheel (coarse)", "Breath controller (coarse)", nullptr,
		"Foot Pedal (coarse)", "Portamento Time (coarse)", "Data Entry (coarse)", "Volume (coarse)",
		"Balance (coarse)", nullptr, "Pan position (coarse)", "Expression (coarse)",
		"Effect Control 1 (coarse)", "Effect Control 2 (coarse)", nullptr, nullptr,
		"General Purpose Slider 1", "General Purpose Slider 2", "General Purpose Slider 3", "General Purpose Slider 4",
		nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr,
		"Bank Select (fine)", "Modulation Wheel (fine)", "Breath controller (fine)", nullptr,
		"Foot Pedal (fine)", "Portamento Time (fine)", "Data Entry (fine)", "Volume (fine)",
		"Balance (fine)", nullptr, "Pan position (fine)", "Expression (fine)",
		"Effect Control 1 (fine)", "Effect Control 2 (fine)", nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr,
		"Hold Pedal", "Portamento", "Sustenuto Pedal", "Soft Pedal",
		"Legato Pedal", "Hold 2 Pedal", "Sound Variation", "Sound Timbre",
		"Sound Release Time", "Sound Attack Time", "Sound Brightness", "Sound Control 6",
		"Sound Control 7", "Sound Control 8", "Sound Control 9", "Sound Control 10",
		"General Purpose Button 1", "General Purpose Button 2", "General Purpose Button 3", "General Purpose Button 4",
		nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, "Reverb Level",
		"Tremolo Level", "Chorus Level", "Celeste Level", "Phaser Level",
		"Data Button increment", "Data Button decrement", "Non-registered Parameter (fine)", "Non-registered Parameter (coarse)",
		"Registered Parameter (fine)", "Registered Parameter (coarse)", nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr,
		nullptr, nullptr, nullptr, nullptr,
		"All Sound Off", "All Controllers Off", "Local Keyboard", "All Notes Off",
		"Omni Mode Off", "Omni Mode On", "Mono Operation", "Poly Operation"
	};

	if (auto name = controllerNames[controllerNumber])
		return { name };

	String asNumber { controllerNumber };

	return asNumber;
}

}  // namespace lemons::midi
