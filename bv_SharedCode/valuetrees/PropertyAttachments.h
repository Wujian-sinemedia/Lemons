
namespace bav
{

/* Updates the value tree with changes from the property object */

class IntNodeToValueTreeAttachment  :      public  NonParamValueTreeNode::Listener,
                                           private juce::AsyncUpdater
{
public:
    IntNodeToValueTreeAttachment (IntValueTreeNode* intNode,
                                  juce::ValueTree treeToUse,
                                  juce::UndoManager* um = nullptr)
        : node (intNode),
          tree (treeToUse),
          undoManager (um)
    {
        jassert (tree.isValid());
        
        node->addListener (this);
        
        currentValue.referTo (tree, DefaultValueTreeIds::NonParameterValue, nullptr);
        currentDefaultValue.referTo (tree, DefaultValueTreeIds::NonParameterDefaultValue, nullptr);
    }
    
    virtual ~IntNodeToValueTreeAttachment() override
    {
        cancelPendingUpdate();
        node->removeListener (this);
    }
    
    void propertyValueChanged (const juce::String&) override final
    {
        if (juce::MessageManager::getInstance()->isThisTheMessageThread())
            handleUpdateNowIfNeeded();
        else
            triggerAsyncUpdate();
    }
    
    void propertyDefaultValueChanged (const juce::String&) override final
    {
        if (juce::MessageManager::getInstance()->isThisTheMessageThread())
            handleUpdateNowIfNeeded();
        else
            triggerAsyncUpdate();
    }
    
    void handleAsyncUpdate() override final
    {
        /* Current value */
        const auto val = node->getCurrentValue();
        
        if (currentValue.get() != val)
        {
            if (undoManager != nullptr)
            {
                undoManager->beginNewTransaction();
                undoManager->setCurrentTransactionName (TRANS ("Changed") + " " + node->longName);
            }
            
            currentValue.setValue (val, undoManager);
        }
        
        /* Default value */
        const auto def = node->getDefaultValue();
        
        if (currentDefaultValue.get() != def)
        {
            if (undoManager != nullptr)
            {
                undoManager->beginNewTransaction();
                undoManager->setCurrentTransactionName (TRANS ("Changed default value of") + " " + node->longName);
            }
            
            currentDefaultValue.setValue (def, undoManager);
        }
    }
    
private:
    IntValueTreeNode* const node;
    juce::ValueTree tree;
    
    juce::CachedValue<int> currentValue;
    juce::CachedValue<int> currentDefaultValue;
    
    juce::UndoManager* const undoManager;
};


//


class FloatNodeToValueTreeAttachment  :      public  NonParamValueTreeNode::Listener,
                                             private juce::AsyncUpdater
{
public:
    FloatNodeToValueTreeAttachment (FloatValueTreeNode* intNode,
                                    juce::ValueTree treeToUse,
                                    juce::UndoManager* um = nullptr)
      : node (intNode),
        tree (treeToUse),
        undoManager (um)
    {
        jassert (tree.isValid());
        
        node->addListener (this);
        
        currentValue.referTo (tree, DefaultValueTreeIds::NonParameterValue, nullptr);
        currentDefaultValue.referTo (tree, DefaultValueTreeIds::NonParameterDefaultValue, nullptr);
    }
    
    virtual ~FloatNodeToValueTreeAttachment() override
    {
        cancelPendingUpdate();
        node->removeListener (this);
    }
    
    void propertyValueChanged (const juce::String&) override final
    {
        if (juce::MessageManager::getInstance()->isThisTheMessageThread())
            handleUpdateNowIfNeeded();
        else
            triggerAsyncUpdate();
    }
    
    void propertyDefaultValueChanged (const juce::String&) override final
    {
        if (juce::MessageManager::getInstance()->isThisTheMessageThread())
            handleUpdateNowIfNeeded();
        else
            triggerAsyncUpdate();
    }
    
    void handleAsyncUpdate() override final
    {
        /* Current value */
        const auto val = node->getCurrentValue();
        
        if (currentValue.get() != val)
        {
            if (undoManager != nullptr)
            {
                undoManager->beginNewTransaction();
                undoManager->setCurrentTransactionName (TRANS ("Changed") + " " + node->longName);
            }
            
            currentValue.setValue (val, undoManager);
        }
        
        /* Default value */
        const auto def = node->getDefaultValue();
        
        if (currentDefaultValue.get() != def)
        {
            if (undoManager != nullptr)
            {
                undoManager->beginNewTransaction();
                undoManager->setCurrentTransactionName (TRANS ("Changed default value of") + " " + node->longName);
            }
            
            currentDefaultValue.setValue (def, undoManager);
        }
    }
    
private:
    FloatValueTreeNode* const node;
    juce::ValueTree tree;
    
    juce::CachedValue<float> currentValue;
    juce::CachedValue<float> currentDefaultValue;
    
    juce::UndoManager* const undoManager;
};


//


class BoolNodeToValueTreeAttachment  :      public  NonParamValueTreeNode::Listener,
                                            private juce::AsyncUpdater
{
public:
    BoolNodeToValueTreeAttachment (BoolValueTreeNode* intNode,
                                   juce::ValueTree treeToUse,
                                   juce::UndoManager* um = nullptr)
      : node (intNode),
        tree (treeToUse),
        undoManager (um)
    {
        jassert (tree.isValid());
        
        node->addListener (this);
        
        currentValue.referTo (tree, DefaultValueTreeIds::NonParameterValue, nullptr);
        currentDefaultValue.referTo (tree, DefaultValueTreeIds::NonParameterDefaultValue, nullptr);
    }
    
    virtual ~BoolNodeToValueTreeAttachment() override
    {
        cancelPendingUpdate();
        node->removeListener (this);
    }
    
    void propertyValueChanged (const juce::String&) override final
    {
        if (juce::MessageManager::getInstance()->isThisTheMessageThread())
            handleUpdateNowIfNeeded();
        else
            triggerAsyncUpdate();
    }
    
    void propertyDefaultValueChanged (const juce::String&) override final
    {
        if (juce::MessageManager::getInstance()->isThisTheMessageThread())
            handleUpdateNowIfNeeded();
        else
            triggerAsyncUpdate();
    }
    
    void handleAsyncUpdate() override final
    {
        /* Current value */
        const auto val = node->getCurrentValue();
        
        if (currentValue.get() != val)
        {
            if (undoManager != nullptr)
            {
                undoManager->beginNewTransaction();
                undoManager->setCurrentTransactionName (TRANS ("Changed") + " " + node->longName);
            }
            
            currentValue.setValue (val, undoManager);
        }
        
        /* Default value */
        const auto def = node->getDefaultValue();
        
        if (currentDefaultValue.get() != def)
        {
            if (undoManager != nullptr)
            {
                undoManager->beginNewTransaction();
                undoManager->setCurrentTransactionName (TRANS ("Changed default value of") + " " + node->longName);
            }
            
            currentDefaultValue.setValue (def, undoManager);
        }
    }
    
private:
    BoolValueTreeNode* const node;
    juce::ValueTree tree;
    
    juce::CachedValue<bool> currentValue;
    juce::CachedValue<bool> currentDefaultValue;
    
    juce::UndoManager* const undoManager;
};


//


class StringNodeToValueTreeAttachment  :      public  NonParamValueTreeNode::Listener,
                                              private juce::AsyncUpdater
{
public:
    StringNodeToValueTreeAttachment (StringValueTreeNode* intNode,
                                     juce::ValueTree treeToUse,
                                     juce::UndoManager* um = nullptr)
      : node (intNode),
        tree (treeToUse),
        undoManager (um)
    {
        jassert (tree.isValid());
        
        node->addListener (this);
        
        currentValue.referTo (tree, DefaultValueTreeIds::NonParameterValue, nullptr);
        currentDefaultValue.referTo (tree, DefaultValueTreeIds::NonParameterDefaultValue, nullptr);
    }
    
    virtual ~StringNodeToValueTreeAttachment() override
    {
        cancelPendingUpdate();
        node->removeListener (this);
    }
    
    void propertyValueChanged (const juce::String&) override final
    {
        if (juce::MessageManager::getInstance()->isThisTheMessageThread())
            handleUpdateNowIfNeeded();
        else
            triggerAsyncUpdate();
    }
    
    void propertyDefaultValueChanged (const juce::String&) override final
    {
        if (juce::MessageManager::getInstance()->isThisTheMessageThread())
            handleUpdateNowIfNeeded();
        else
            triggerAsyncUpdate();
    }
    
    void handleAsyncUpdate() override final
    {
        /* Current value */
        const auto val = node->getCurrentValue();
        
        if (currentValue.get() != val)
        {
            if (undoManager != nullptr)
            {
                undoManager->beginNewTransaction();
                undoManager->setCurrentTransactionName (TRANS ("Changed") + " " + node->longName);
            }
            
            currentValue.setValue (val, undoManager);
        }
        
        /* Default value */
        const auto def = node->getDefaultValue();
        
        if (currentDefaultValue.get() != def)
        {
            if (undoManager != nullptr)
            {
                undoManager->beginNewTransaction();
                undoManager->setCurrentTransactionName (TRANS ("Changed default value of") + " " + node->longName);
            }
            
            currentDefaultValue.setValue (def, undoManager);
        }
    }
    
private:
    StringValueTreeNode* const node;
    juce::ValueTree tree;
    
    juce::CachedValue<juce::String> currentValue;
    juce::CachedValue<juce::String> currentDefaultValue;
    
    juce::UndoManager* const undoManager;
};



//==============================================================================
//==============================================================================


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
        /* Current property value */
        const auto value = currentValue.get();
        
        if (value != node->getCurrentValue())
            node->setValue (value);
        
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


//


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
        /* Current property value */
        const auto value = currentValue.get();
        
        if (value != node->getCurrentValue())
            node->setValue (value);
        
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


//


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
        /* Current property value */
        const auto value = currentValue.get();
        
        if (value != node->getCurrentValue())
            node->setValue (value);
        
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


//


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
        /* Current property value */
        const auto value = currentValue.get();
        
        if (value != node->getCurrentValue())
            node->setValue (value);
        
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


//==============================================================================
//==============================================================================


struct IntPropertyAttachment    :   IntNodeToValueTreeAttachment,
                                    ValueTreeToIntNodeAttachment
{
    IntPropertyAttachment (IntValueTreeNode* intNode,
                           juce::ValueTree tree,
                           juce::UndoManager* um = nullptr)
    
      : IntNodeToValueTreeAttachment (intNode, tree, um),
        ValueTreeToIntNodeAttachment (intNode, tree)
    { }
};


struct FloatPropertyAttachment  :   FloatNodeToValueTreeAttachment,
                                    ValueTreeToFloatNodeAttachment
{
    FloatPropertyAttachment (FloatValueTreeNode* floatNode,
                             juce::ValueTree tree,
                             juce::UndoManager* um = nullptr)
    
      : FloatNodeToValueTreeAttachment (floatNode, tree, um),
        ValueTreeToFloatNodeAttachment (floatNode, tree)
    { }
};


struct BoolPropertyAttachment  :    BoolNodeToValueTreeAttachment,
                                    ValueTreeToBoolNodeAttachment
{
    BoolPropertyAttachment (BoolValueTreeNode* boolNode,
                            juce::ValueTree tree,
                            juce::UndoManager* um = nullptr)
    
      : BoolNodeToValueTreeAttachment (boolNode, tree, um),
        ValueTreeToBoolNodeAttachment (boolNode, tree)
    { }
};


struct StringPropertyAttachment  :    StringNodeToValueTreeAttachment,
                                      ValueTreeToStringNodeAttachment
{
    StringPropertyAttachment (StringValueTreeNode* boolNode,
                              juce::ValueTree tree,
                              juce::UndoManager* um = nullptr)
    
      : StringNodeToValueTreeAttachment (boolNode, tree, um),
        ValueTreeToStringNodeAttachment (boolNode, tree)
    { }
};


}  // namespace
