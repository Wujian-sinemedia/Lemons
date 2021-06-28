
namespace bav::network
{
OscConnectionChecker::OscConnectionChecker (juce::OSCSender& s, juce::OSCReceiver& r)
    : OscListener (r),
      addressPattern ("/OscConnectionChecker"), sender (s), computerName (juce::SystemStats::getComputerName())
{
}

void OscConnectionChecker::oscMessageReceived (const juce::OSCMessage& message)
{
    if (message.getAddressPattern() != addressPattern || message.isEmpty()) return;

    lastReceivedPingTime = juce::Time::getCurrentTime();
    connected            = true;
}

void OscConnectionChecker::update()
{
    sendPing();

    if (getMsSinceLastPingRecieved() >= pingInterval * 2)
    {
        connectionLost();
    }
}

void OscConnectionChecker::sendPing()
{
    sender.send (addressPattern, computerName);
}


inline juce::int64 msSinceTime (const juce::Time& time)
{
    return juce::Time::getCurrentTime().toMilliseconds() - time.toMilliseconds();
}


juce::int64 OscConnectionChecker::getMsSinceLastPingRecieved() const
{
    return msSinceTime (lastReceivedPingTime);
}

void OscConnectionChecker::connectionLost()
{
    if (connected)
    {
        connected = false;
        broadcaster.trigger();
    }
}


}  // namespace bav::network
