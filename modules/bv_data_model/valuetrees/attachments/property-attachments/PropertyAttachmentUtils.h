
namespace bav
{
struct PropertyAttachmentBase
{
    PropertyAttachmentBase()  = default;
    ~PropertyAttachmentBase() = default;
};


struct IntPropertyAttachment : PropertyAttachmentBase,
                               IntNodeToValueTreeAttachment,
                               ValueTreeToIntNodeAttachment
{
    IntPropertyAttachment (IntValueTreeNode*  intNode,
                           juce::ValueTree    tree,
                           juce::UndoManager* um = nullptr)

        : IntNodeToValueTreeAttachment (intNode, tree, um)
        , ValueTreeToIntNodeAttachment (intNode, tree)
    {
    }
};


struct FloatPropertyAttachment : PropertyAttachmentBase,
                                 FloatNodeToValueTreeAttachment,
                                 ValueTreeToFloatNodeAttachment
{
    FloatPropertyAttachment (FloatValueTreeNode* floatNode,
                             juce::ValueTree     tree,
                             juce::UndoManager*  um = nullptr)

        : FloatNodeToValueTreeAttachment (floatNode, tree, um)
        , ValueTreeToFloatNodeAttachment (floatNode, tree)
    {
    }
};


struct BoolPropertyAttachment : PropertyAttachmentBase,
                                BoolNodeToValueTreeAttachment,
                                ValueTreeToBoolNodeAttachment
{
    BoolPropertyAttachment (BoolValueTreeNode* boolNode,
                            juce::ValueTree    tree,
                            juce::UndoManager* um = nullptr)

        : BoolNodeToValueTreeAttachment (boolNode, tree, um)
        , ValueTreeToBoolNodeAttachment (boolNode, tree)
    {
    }
};


struct StringPropertyAttachment : PropertyAttachmentBase,
                                  StringNodeToValueTreeAttachment,
                                  ValueTreeToStringNodeAttachment
{
    StringPropertyAttachment (StringValueTreeNode* boolNode,
                              juce::ValueTree      tree,
                              juce::UndoManager*   um = nullptr)

        : StringNodeToValueTreeAttachment (boolNode, tree, um)
        , ValueTreeToStringNodeAttachment (boolNode, tree)
    {
    }
};


//==============================================================================
//==============================================================================


static inline void createTwoWayPropertyValueTreeAttachments (
    juce::OwnedArray< PropertyAttachmentBase >&  attachments,
    juce::ValueTree                              propertyTree,
    int                                          totalNumProperties,
    std::function< NonParamValueTreeNode*(int) > findProperty,
    juce::UndoManager*                           um                     = nullptr,
    int                                          propertyIndexToStartAt = 0)
{
    jassert (propertyTree.isValid());

    attachments.ensureStorageAllocated (totalNumProperties);

    for (int i = propertyIndexToStartAt;
         i < propertyIndexToStartAt + totalNumProperties;
         ++i)
    {
        auto* property = findProperty (i);
        jassert (property != nullptr);

        auto tree = getChildTreeForNonParamProperty (propertyTree, property);

        if (auto* intNode = dynamic_cast< IntValueTreeNode* > (property))
        {
            attachments.add (new IntPropertyAttachment (intNode, tree, um));
        }
        else if (auto* floatNode = dynamic_cast< FloatValueTreeNode* > (property))
        {
            attachments.add (new FloatPropertyAttachment (floatNode, tree, um));
        }
        else if (auto* boolNode = dynamic_cast< BoolValueTreeNode* > (property))
        {
            attachments.add (new BoolPropertyAttachment (boolNode, tree, um));
        }
        else if (auto* stringNode = dynamic_cast< StringValueTreeNode* > (property))
        {
            attachments.add (new StringPropertyAttachment (stringNode, tree, um));
        }
        else
        {
            jassertfalse;
        }
    }
}


} // namespace bav
