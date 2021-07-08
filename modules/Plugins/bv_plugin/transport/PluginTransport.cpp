
namespace bav::plugin
{
Transport::TimeInfo Transport::processNextBlock (juce::AudioPlayHead* playHead)
{
    if (playHead != nullptr)
        if (playHead->getCurrentPosition (playheadInfo))
            return fromPlayheadInfo();

    if (isAbletonLinkEnabled())
        return fromAbletonLink();

    return fromInternalMetronome();
}

Transport::TimeInfo Transport::fromPlayheadInfo() const
{
    TimeInfo info;

    info.bpm       = playheadInfo.bpm;
    info.isPlaying = playheadInfo.isPlaying;

    // update Link & internal metronome w bpm...

    info.source = TimeInfo::Host;

    return info;
}

Transport::TimeInfo Transport::fromInternalMetronome() const
{
    TimeInfo info;

    //info.bpm = playheadInfo.bpm;
    info.isPlaying = true;

    info.source = TimeInfo::Internal;

    return info;
}

Transport::TimeInfo Transport::fromAbletonLink() const
{
#if BV_USE_ABLETON_LINK
    TimeInfo info;

    auto sessionState = abletonLink.captureAudioSessionState();

    info.bpm       = sessionState.tempo();
    info.isPlaying = sessionState.isPlaying();

    // update internal metronome w bpm...

    info.source = TimeInfo::AbletonLink;

    return info;
#else
    return {};
#endif
}

bool Transport::isAbletonLinkEnabled() const
{
#if BV_USE_ABLETON_LINK
    return abletonLink.isEnabled();
#else
    return false;
#endif
}

int Transport::getNumAbletonLinkSessionPeers() const
{
#if BV_USE_ABLETON_LINK
    return static_cast< int > (abletonLink.numPeers());
#else
    return 0;
#endif
}

}  // namespace bav
