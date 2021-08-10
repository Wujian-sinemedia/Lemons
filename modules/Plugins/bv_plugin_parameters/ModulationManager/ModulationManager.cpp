
namespace bav::plugin
{
void ModulationManager::addConnection (Parameter& parameter, LFO& lfo)
{
}

void ModulationManager::removeConnection (Parameter& parameter, LFO& lfo)
{
}

void ModulationManager::serialize (TreeReflector& ref)
{
}

ModulationManager::Connection::Connection (Parameter& paramToUse, LFO& lfoToUse)
    : parameter (paramToUse), lfo (lfoToUse)
{
}

ModulationManager::Connection::~Connection()
{
}

void ModulationManager::Connection::serialize (TreeReflector& ref)
{
}

}  // namespace bav::plugin
