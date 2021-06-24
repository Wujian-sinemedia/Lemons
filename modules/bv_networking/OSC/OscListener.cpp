namespace bav::network
{

OscListener::OscListener (juce::OSCReceiver& receiverToUse)
: receiver (receiverToUse)
{
    receiver.addListener (this);
}

OscListener::~OscListener()
{
    receiver.removeListener (this);
}

}
