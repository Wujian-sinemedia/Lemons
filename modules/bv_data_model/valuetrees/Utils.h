
namespace bav
{
static inline void parsePropertyTreeForPropertyPointers (
    const NonParamValueTreeNodeGroup*      group,
    std::vector< NonParamValueTreeNode* >& pointers)
{
    jassert (group != nullptr);

    for (auto* node : *group)
    {
        if (auto* prop = node->getNode()) { pointers.push_back (prop); }
        else if (auto* thisGroup = node->getGroup())
        {
            parsePropertyTreeForPropertyPointers (thisGroup, pointers);
        }
    }
}


/* note: this only works if the underlying parameter objects in the group are derived from my Parameter base class */
static inline void createValueTreeFromParameterTree (
    juce::ValueTree& tree, const juce::AudioProcessorParameterGroup& parameterTree)
{
    using namespace DefaultValueTreeIds;

    jassert (tree.isValid());

    for (auto* node : parameterTree)
    {
        if (auto* param = node->getParameter())
        {
            if (auto* parameter = dynamic_cast< bav::Parameter* > (param))
            {
                auto child = parameter->toValueTree();
                jassert (child.isValid());
                tree.addChild (child, -1, nullptr);
            }
            else
            {
                jassertfalse;
            }
        }
        else if (auto* thisGroup = node->getGroup())
        {
            juce::ValueTree parameterGroupNode {ParameterGroupNode};
            createValueTreeFromParameterTree (parameterGroupNode, *thisGroup);
            tree.addChild (parameterGroupNode, -1, nullptr);
        }
    }
}


/* Returns the child tree representing the desired parameter in a ValueTree created by the createValueTreeFromParameterTree method */
static inline juce::ValueTree
    getChildTreeForParameter (juce::ValueTree&            topLevelTree,
                              const bav::Parameter* const parameter)
{
    jassert (topLevelTree.isValid() && parameter != nullptr);

    auto child = topLevelTree.getChildWithProperty (
        DefaultValueTreeIds::ParameterName, parameter->parameterNameVerbose);

    jassert (child.isValid());

    return child;
}


} // namespace bav
