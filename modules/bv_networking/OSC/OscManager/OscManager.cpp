namespace bav::network
{
OscManager::OscManager (const String& targetHostName, int portNumber)
: SerializableData (targetHostName + "_OSC")
{
    connect (targetHostName, portNumber);
}

OscManager::~OscManager()
{
    disconnect();
}

void OscManager::connect (const String& targetHostName, int portNumber)
{
    sender.connect (targetHostName, portNumber);
    receiver.connect (portNumber);
    portNum  = portNumber;
    hostName = targetHostName;
}

void OscManager::disconnect()
{
    sender.disconnect();
    receiver.disconnect();
}

void OscManager::toValueTree (ValueTree& t)
{
    t.setProperty ("PortNumber", portNum, nullptr);
    t.setProperty ("HostName", hostName, nullptr);
}

void OscManager::fromValueTree (const ValueTree& t)
{
    disconnect();
    
    connect (t.getProperty ("HostName"),
             t.getProperty ("PortNumber"));
}

}  // namespace bav::network
