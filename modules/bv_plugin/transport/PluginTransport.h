
#pragma once

#if BV_USE_ABLETON_LINK
#    include "Link.hpp"
#endif


namespace bav
{

/*
 - metronome for usage in realtime callbacks (tempo syncing FX, LFOs, etc)
 - callbacks on msg thread approx on beats (listeners? -- async updater)
 */

class PluginTransport
{
public:
    PluginTransport() = default;
    
    ~PluginTransport() = default;
    
    struct TimeInfo
    {
        double bpm;
        bool isPlaying;
        
        // samples after last bar downbeat...?
        // samples until next beat...?
        
        enum Source { Host, AbletonLink, Internal };
        Source source;
    };
    
    
    // this method should be fed the output of audioProcessor.getPlayHead()
    TimeInfo processNextBlock (juce::AudioPlayHead* playHead);
    
    bool isAbletonLinkEnabled() const;
    int getNumAbletonLinkSessionPeers() const;
    
    
private:
    TimeInfo fromPlayheadInfo() const;
    TimeInfo fromAbletonLink() const;  // this will return an invalid TimeInfo object if you're not using AbletonLink
    TimeInfo fromInternalMetronome() const;
    
    juce::AudioPlayHead::CurrentPositionInfo playheadInfo;

#if BV_USE_ABLETON_LINK
    ableton::Link abletonLink {120.0};  // constructed w/ initial BPM
#endif
};

}  // namespace
