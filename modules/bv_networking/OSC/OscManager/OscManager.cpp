namespace bav::network
{
OscManager::OscManager (const String& targetHostName, int portNumber)
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
}

void OscManager::disconnect()
{
    sender.disconnect();
    receiver.disconnect();
}

}  // namespace bav::network
