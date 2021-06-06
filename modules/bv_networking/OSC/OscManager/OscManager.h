
#pragma once

namespace bav::network
{
struct OscManager
{
public:
    OscManager() = default;
    OscManager (const String& targetHostName, int portNumber = 53100);
    virtual ~OscManager();

    void connect (const String& targetHostName, int portNumber = 53100);
    void disconnect();

    juce::OSCSender   sender;
    juce::OSCReceiver receiver;
};

}  // namespace bav::network
