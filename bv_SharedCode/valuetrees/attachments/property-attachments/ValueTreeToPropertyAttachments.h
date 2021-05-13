
namespace bav
{

/* Updates the property object with changes from the ValueTree */

class ValueTreeToIntNodeAttachment  :   public juce::ValueTree::Listener
{
public:
    ValueTreeToIntNodeAttachment (bav::IntValueTreeNode* intNode,
                                  juce::ValueTree treeToUse)
    : node (intNode),
    tree (treeToUse)
    {
        jassert (tree.isValid());
        
        tree.addListener (this);
        
        currentValue.referTo (tree, DefaultValueTreeIds::NonParameterValue, nullptr);
        currentDefaultValue.referTo (tree, DefaultValueTreeIds::NonParameterDefaultValue, nullptr);
    }
    
    void valueTreePropertyChanged (juce::ValueTree&, const juce::Identifier&) override final
    {
        node->setValue (currentValue.get());
        
        /* Property default value */
        const auto defaultVal = currentDefaultValue.get();
        
        if (defaultVal != node->getDefaultValue())
            node->setDefaultValue (defaultVal);
    }
    
private:
    bav::IntValueTreeNode* const node;
    juce::ValueTree tree;
    
    juce::CachedValue<int> currentValue;
    juce::CachedValue<int> currentDefaultValue;
};


//==============================================================================
//==============================================================================


class ValueTreeToFloatNodeAttachment  :   public juce::ValueTree::Listener
{
public:
    ValueTreeToFloatNodeAttachment (bav::FloatValueTreeNode* floatNode,
                                    juce::ValueTree treeToUse)
    : node (floatNode),
    tree (treeToUse)
    {
        jassert (tree.isValid());
        
        tree.addListener (this);
        
        currentValue.referTo (tree, DefaultValueTreeIds::NonParameterValue, nullptr);
        currentDefaultValue.referTo (tree, DefaultValueTreeIds::NonParameterDefaultValue, nullptr);
    }
    
    void valueTreePropertyChanged (juce::ValueTree&, const juce::Identifier&) override final
    {
        node->setValue (currentValue.get());
        
        /* Property default value */
        const auto defaultVal = currentDefaultValue.get();
        
        if (defaultVal != node->getDefaultValue())
            node->setDefaultValue (defaultVal);
    }
    
private:
    bav::FloatValueTreeNode* const node;
    juce::ValueTree tree;
    
    juce::CachedValue<float> currentValue;
    juce::CachedValue<float> currentDefaultValue;
};


//==============================================================================
//==============================================================================


class ValueTreeToBoolNodeAttachment  :   public juce::ValueTree::Listener
{
public:
    ValueTreeToBoolNodeAttachment (bav::BoolValueTreeNode* boolNode,
                                   juce::ValueTree treeToUse)
    : node (boolNode),
    tree (treeToUse)
    {
        jassert (tree.isValid());
        
        tree.addListener (this);
        
        currentValue.referTo (tree, DefaultValueTreeIds::NonParameterValue, nullptr);
        currentDefaultValue.referTo (tree, DefaultValueTreeIds::NonParameterDefaultValue, nullptr);
    }
    
    void valueTreePropertyChanged (juce::ValueTree&, const juce::Identifier&) override final
    {
        node->setValue (currentValue.get());
        
        /* Property default value */
        const auto defaultVal = currentDefaultValue.get();
        
        if (defaultVal != node->getDefaultValue())
            node->setDefaultValue (defaultVal);
    }
    
private:
    bav::BoolValueTreeNode* const node;
    juce::ValueTree tree;
    
    juce::CachedValue<bool> currentValue;
    juce::CachedValue<bool> currentDefaultValue;
};


//==============================================================================
//==============================================================================


class ValueTreeToStringNodeAttachment  :   public juce::ValueTree::Listener
{
public:
    ValueTreeToStringNodeAttachment (bav::StringValueTreeNode* stringNode,
                                     juce::ValueTree treeToUse)
    : node (stringNode),
    tree (treeToUse)
    {
        jassert (tree.isValid());
        
        tree.addListener (this);
        
        currentValue.referTo (tree, DefaultValueTreeIds::NonParameterValue, nullptr);
        currentDefaultValue.referTo (tree, DefaultValueTreeIds::NonParameterDefaultValue, nullptr);
    }
    
    void valueTreePropertyChanged (juce::ValueTree&, const juce::Identifier&) override final
    {
        node->setValue (currentValue.get());
        
        /* Property default value */
        const auto defaultVal = currentDefaultValue.get();
        
        if (defaultVal != node->getDefaultValue())
            node->setDefaultValue (defaultVal);
    }
    
private:
    bav::StringValueTreeNode* const node;
    juce::ValueTree tree;
    
    juce::CachedValue<juce::String> currentValue;
    juce::CachedValue<juce::String> currentDefaultValue;
};


}  // namespace
