
/** @ingroup client
 *  @{
 */
#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

#include <mts_esp_cpp/warnings/begin_warnings.h>
#include <libMTSClient.h>
#include <mts_esp_cpp/warnings/end_warnings.h>


namespace MTSESP
{

using juce::String;

class Client
{
public:
    template < typename T >
    float midiToFrequency (T midiNote, int midiChannel = -1) const
    {
        static_assert (std::is_arithmetic< T >(), "This function only works with arithmetic types!");
        return static_cast< float > (MTS_NoteToFrequency (client.get(),
                                                          char (midiNote),
                                                          char (midiChannel)));
    }

    template < typename T >
    float frequencyToMidi (T frequency, int midiChannel = -1) const
    {
        static_assert (std::is_arithmetic< T >(), "This function only works with arithmetic types!");
        return static_cast< float > (MTS_FrequencyToNote (client.get(),
                                                          static_cast< double > (frequency),
                                                          char (midiChannel)));
    }

    template < typename T >
    float retuneAmountInSemitones (T midiNote, int midiChannel = -1) const
    {
        static_assert (std::is_arithmetic< T >(), "This function only works with arithmetic types!");
        return static_cast< float > (MTS_RetuningInSemitones (client.get(),
                                                              char (midiNote),
                                                              char (midiChannel)));
    }

    bool shouldFilterNote (int midiPitch, int midiChannel = -1) const;

    bool isConnected() const;

    String getScaleName() const;

    // call this to parse SysEx tuning data
    void parseMidiMessageForSysEx (const juce::MidiMessage& m) const;

private:
    struct Deleter
    {
        void operator() (MTSClient* c);
    };

    std::unique_ptr< MTSClient, Deleter > client {MTS_RegisterClient()};
};

}  // namespace MTSESP

/** @}*/
