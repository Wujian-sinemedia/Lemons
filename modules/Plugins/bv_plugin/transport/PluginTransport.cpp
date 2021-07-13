
namespace bav::plugin
{
Transport::TimeInfo Transport::processNextBlock (juce::AudioPlayHead* playHead)
{
    if (playHead != nullptr)
        if (playHead->getCurrentPosition (playheadInfo))
            return fromPlayheadInfo();

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

}  // namespace bav
