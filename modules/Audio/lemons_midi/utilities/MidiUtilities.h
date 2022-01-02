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

namespace lemons::midi
{

using juce::MidiBuffer;
using juce::MidiMessage;
using juce::String;

// copies a range of events from one juce::MidiBuffer to another, optionally providing a sample offset
void copyRangeOfMidiBuffer (const MidiBuffer& readingBuffer,
                            MidiBuffer&       destBuffer,
                            int               startSampleOfInput,
                            int               startSampleOfOutput,
                            int               numSamples);


/** Utility RAII struct for copying a section of one MIDI buffer to the beginning of another buffer, and then back to the original section of the original buffer.
    This code:
    @code
    MidiBuffer orig, alias;

    int startSample = 10, numSamples = 40;

    copyRangeOfMidiBuffer (orig, alias, startSample, 0, numSamples);

    // process the midi inside 'alias'

    copyRangeOfMidiBuffer (alias, orig, 0, startSample, numSamples);
    @endcode
    is identical to doing the following:
    @code
    MidiBuffer orig, alias;

    const ScopedMidiBufferAlias scopedAlias { orig, alias, 10, 40 };

    // process the midi inside 'alias'
    @endcode
 */
struct ScopedMidiBufferAlias final
{
	/** Creates a scoped alias. This copies the events from originalBuffer, starting at startSampleInOrigBuffer, into the beginning of aliasBufferToUse. */
	explicit ScopedMidiBufferAlias (MidiBuffer& originalBuffer,
	                                MidiBuffer& aliasBufferToUse,
	                                int         startSampleInOrigBuffer,
	                                int         numSamplesToCopy);

	/** Copies the events now in the alias buffer back to the original range of the orignal buffer. */
	~ScopedMidiBufferAlias();

private:
	MidiBuffer& origBuffer;
	MidiBuffer& aliasBuffer;

	const int startSample, numSamples;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScopedMidiBufferAlias)
};

}  // namespace lemons::midi
