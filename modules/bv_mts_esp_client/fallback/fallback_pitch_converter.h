
#pragma once

namespace bav
{
class FallbackPitchConverter
{
public:
    bool setConcertPitchHz (float newConcertPitchHz);

    float midiToFrequency (int midiNote) const;
    float midiToFrequency (float midiNote) const;

    int frequencyToMidi (float frequency) const;

private:
    float concertPitchHz {440.0f};
};

}  // namespace bav
