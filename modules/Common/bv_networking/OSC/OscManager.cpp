namespace bav::network
{
OscManager::OscManager (const String& dataName)
: SerializableData (dataName + "_OSC")
{ }

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


void OscManager::serialize (TreeReflector& ref)
{
    ref.add ("PortNumber", portNum);
    ref.add ("HostName", hostName);
    
    if (ref.isLoading())
    {
        disconnect();
        connect (hostName, portNum);
    }
}

}  // namespace bav::network
