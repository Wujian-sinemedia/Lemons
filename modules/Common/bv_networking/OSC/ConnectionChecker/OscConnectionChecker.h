#pragma once

namespace bav::network
{
class OscConnectionChecker : private OscListener
{
public:
    OscConnectionChecker (juce::OSCSender& s, juce::OSCReceiver& r);

    bool isConnected() const { return connected; }

    events::Broadcaster& getBroadcaster() { return broadcaster; }

private:
    void oscMessageReceived (const juce::OSCMessage& message) final;

    void connectionLost();

    void        update();
    void        sendPing();
    juce::int64 getMsSinceLastPingRecieved() const;

    const juce::OSCAddressPattern addressPattern;
    juce::OSCSender&              sender;
    const String                  computerName;

    juce::Time lastReceivedPingTime {juce::Time::getCurrentTime()};
    const int  pingInterval {100};

    events::TimerCallback t {[this]
                             { update(); },
                             pingInterval};
    events::Broadcaster   broadcaster;

    bool connected {false};
};

}  // namespace bav::network
