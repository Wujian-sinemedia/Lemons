
namespace bav
{


/* note: this only works if the underlying parameter objects in the group are derived from my Parameter base class */
static inline void createValueTreeFromParameterTree (juce::ValueTree& tree,
                                                     const juce::AudioProcessorParameterGroup& parameterTree)
{
    using namespace DefaultValueTreeIds;
    
    jassert (tree.isValid());
    
    for (auto* node : parameterTree)
    {
        if (auto* param = node->getParameter())
        {
            if (auto* parameter = dynamic_cast<bav::Parameter*> (param))
            {
                juce::ValueTree parameterTreeNode { ParameterNode };
                
                parameterTreeNode.setProperty (ParameterName,  parameter->parameterNameVerbose, nullptr);
                parameterTreeNode.setProperty (ParameterValue, parameter->getCurrentDenormalizedValue(), nullptr);
                parameterTreeNode.setProperty (ParameterDefaultValue, parameter->getNormalizedDefault(), nullptr);
                parameterTreeNode.setProperty (ParameterIsChanging, false, nullptr);
                
                tree.addChild (parameterTreeNode, -1, nullptr);
            }
            else
            {
                jassertfalse;
            }
        }
        else if (auto* thisGroup = node->getGroup())
        {
            juce::ValueTree parameterGroupNode { ParameterGroupNode };
            createValueTreeFromParameterTree (parameterGroupNode, *thisGroup);
            tree.addChild (parameterGroupNode, -1, nullptr);
        }
    }
}


/* Returns the child tree representing the desired parameter in a ValueTree created by the createValueTreeFromParameterTree method */
static inline juce::ValueTree getChildTreeForParameter (juce::ValueTree& topLevelTree,
                                                        const bav::Parameter* const parameter)
{
    jassert (topLevelTree.isValid() && parameter != nullptr);
    
    auto child = topLevelTree.getChildWithProperty (DefaultValueTreeIds::ParameterName,
                                                    parameter->parameterNameVerbose);
    
    jassert (child.isValid());
    
    return child;
}


//==============================================================================
//==============================================================================

static inline void createValueTreeFromNonParamNodes (juce::ValueTree& tree,
                                                     const NonParamValueTreeNodeGroup& propertyTree)
{
    using namespace DefaultValueTreeIds;
    
    jassert (tree.isValid());
    
    for (auto* node : propertyTree)
    {
        if (auto* thisNode = node->getNode())
        {
            tree.appendChild (thisNode->toValueTree(), nullptr);
        }
        else if (auto* group = node->getGroup())
        {
            juce::ValueTree propertyGroupNode { NonParameterGroupNode };
            createValueTreeFromNonParamNodes (propertyGroupNode, *group);
            tree.appendChild (propertyGroupNode, nullptr);
        }
        else
        {
            jassertfalse;
        }
    }
}

static inline juce::ValueTree getChildTreeForNonParamProperty (juce::ValueTree& topLevelTree,
                                                               const NonParamValueTreeNode* const property)
{
    jassert (topLevelTree.isValid() && property != nullptr);
    
    auto child = topLevelTree.getChildWithProperty (DefaultValueTreeIds::NonParameterName,
                                                    property->longName);
    
    jassert (child.isValid());
    
    return child;
}


}  // namespace

