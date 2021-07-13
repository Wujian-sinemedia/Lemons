
#pragma once

namespace bav::plugin
{
/*
 - metronome for usage in realtime callbacks (tempo syncing FX, LFOs, etc)
 - callbacks on msg thread approx on beats (listeners? -- async updater)
 */

class Transport
{
public:
    Transport() = default;

    ~Transport() = default;

    struct TimeInfo
    {
        double bpm;
        bool   isPlaying;

        // samples after last bar downbeat...?
        // samples until next beat...?

        enum Source
        {
            Host,
            Internal
        };
        Source source;
    };


    // this method should be fed the output of audioProcessor.getPlayHead()
    TimeInfo processNextBlock (juce::AudioPlayHead* playHead);

private:
    TimeInfo fromPlayheadInfo() const;
    TimeInfo fromAbletonLink() const;  // this will return an invalid TimeInfo object if you're not using AbletonLink
    TimeInfo fromInternalMetronome() const;

    juce::AudioPlayHead::CurrentPositionInfo playheadInfo;
};

}  // namespace bav
