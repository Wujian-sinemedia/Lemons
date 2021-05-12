

namespace bav
{


static inline juce::AudioProcessorParameterGroup* findParameterSubgroup (const juce::AudioProcessorParameterGroup* parameterTree,
                                                                         const juce::String& subgroupName)
{
    jassert (parameterTree != nullptr);
    
    for (auto* node : *parameterTree)
        if (auto* group = node->getGroup())
            if (group->getName() == subgroupName)
                return group;
    
    return nullptr;
}


//==============================================================================
//==============================================================================


static inline void parseParameterTreeForParameterPointers (const juce::AudioProcessorParameterGroup* group,
                                                           std::vector< bav::Parameter* >& pointers)
{
    jassert (group != nullptr);
    
    for (auto* node : *group)
    {
        if (auto* rawParam = node->getParameter())
        {
            if (auto* parameter = dynamic_cast<bav::Parameter*> (rawParam))
                pointers.push_back (parameter);
            else
                jassertfalse;
        }
        else if (auto* thisGroup = node->getGroup())
        {
            parseParameterTreeForParameterPointers (thisGroup, pointers);
        }
    }
}


}  // namespace
