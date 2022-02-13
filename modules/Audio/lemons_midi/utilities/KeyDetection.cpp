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

namespace lemons::midi
{

using namespace music;

using Key = scales::KeySignature;

struct NoteData final
{
	explicit NoteData (const PitchClass& pitch, int sample)
		: pitchClass (pitch), startSample (sample)
	{
	}

	[[nodiscard]] int getDurationSamples() const noexcept
	{
		jassert (endSample > startSample);
		return endSample - startSample;
	}

	PitchClass pitchClass;

	int startSample { 0 }, endSample { 0 };
};

[[nodiscard]] inline juce::Array<NoteData> getNoteDataFromMidiBuffer (const MidiBuffer& midi)
{
	juce::Array<NoteData> notes;

	for (const auto& meta : midi)
	{
		const auto message = meta.getMessage();

		if (! message.isNoteOnOrOff())
			continue;

		const auto		 sample = meta.samplePosition;
		const PitchClass pitchClass { message.getNoteNumber() };

		if (message.isNoteOn())
		{
			notes.add (NoteData { pitchClass, sample });
			continue;
		}

		if (auto* data = [&notes, &pitchClass]() -> NoteData*
			{
				for (auto& note : notes)
					if (note.pitchClass == pitchClass)	// cppcheck-suppress useStlAlgorithm
						return &note;

				return nullptr;
			}())
		{
			data->endSample = sample;
		}
	}

	return notes;
}

Key estimateKey (const MidiBuffer& midi)
{
	struct KeyResult final
	{
		KeyResult() = default;

		explicit KeyResult (double correlation, const PitchClass& root, Key::Type type)
			: correlationCoefficient (correlation), rootPitchClass (root), scaleType (type)
		{
		}

		[[nodiscard]] bool operator< (const KeyResult& other) const
		{
			return correlationCoefficient < other.correlationCoefficient;
		}

		double correlationCoefficient { 0. };

		PitchClass rootPitchClass { 0 };

		Key::Type scaleType { Key::Type::Major };
	};

	constexpr auto NUM_KEYS = 12;

	double durations[NUM_KEYS] = { 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0. };

	for (const auto& n : getNoteDataFromMidiBuffer (midi))
		durations[n.pitchClass.getAsInt()] += n.getDurationSamples();

	constexpr const double major[NUM_KEYS] = { 6.35, 2.23, 3.48, 2.33, 4.38, 4.09, 2.52, 5.19, 2.39, 3.66, 2.29, 2.88 };
	constexpr const double minor[NUM_KEYS] = { 6.33, 2.68, 3.52, 5.38, 2.60, 3.53, 2.54, 4.75, 3.98, 2.69, 3.34, 3.17 };

	double xAveMajor = 0., xAveMinor = 0., yAve = 0.;

	for (auto i = 0; i < NUM_KEYS; i++)
	{
		xAveMajor += major[i];
		xAveMinor += minor[i];
		yAve += durations[i];
	}

	xAveMajor /= static_cast<double> (NUM_KEYS);
	xAveMinor /= static_cast<double> (NUM_KEYS);
	yAve /= static_cast<double> (NUM_KEYS);

	juce::Array<KeyResult> results;

	for (auto key = 0; key < NUM_KEYS; key++)
	{
		double s1Major = 0., s2Major = 0., s3Major = 0.;
		double s1Minor = 0., s2Minor = 0., s3Minor = 0.;

		for (auto i = 0; i < NUM_KEYS; i++)
		{
			const auto xiMajor = major[(i - key + NUM_KEYS) % NUM_KEYS];
			const auto xiMinor = minor[(i - key + NUM_KEYS) % NUM_KEYS];

			s1Major += (xiMajor - xAveMajor) * (durations[i] - yAve);
			s2Major += (xiMajor - xAveMajor) * (xiMajor - xAveMajor);
			s3Major += (durations[i] - yAve) * (durations[i] - yAve);

			s1Minor += (xiMinor - xAveMinor) * (durations[i] - yAve);
			s2Minor += (xiMinor - xAveMinor) * (xiMinor - xAveMinor);
			s3Minor += (durations[i] - yAve) * (durations[i] - yAve);
		}

		if (s2Major * s3Major > 0)
		{
			const auto rMajor = s1Major / std::sqrt (s2Major * s3Major);
			const auto rMinor = s1Minor / std::sqrt (s2Minor * s3Minor);

			results.add (KeyResult { rMajor, PitchClass { key }, Key::Type::Major });
			results.add (KeyResult { rMinor, PitchClass { key }, Key::Type::NaturalMinor });
		}
	}

	results.sort();

	const auto result = results.getFirst();

	return Key { result.scaleType, result.rootPitchClass };
}

Key estimateKey (const MidiFile& midi)
{
	return estimateKey (midiBufferFromFile (midi));
}

}  // namespace lemons::midi
