
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
        const auto currentValue = node->getCurrentValue();
        
        if (currentValue.get() != currentValue)
        {
            if (undoManager != nullptr)
            {
                undoManager->beginNewTransaction();
                undoManager->setCurrentTransactionName (TRANS ("Changed") + " " + node->longName);
            }
            
            currentValue.setValue (currentValue, undoManager);
        }
        
        /* Default value */
        const auto currentDefault = node->getDefaultValue();
        
        if (currentDefaultValue.get() != currentDefault)
        {
            if (undoManager != nullptr)
            {
                undoManager->beginNewTransaction();
                undoManager->setCurrentTransactionName (TRANS ("Changed default value of") + " " + node->longName);
            }
            
            currentDefaultValue.setValue (currentDefault, undoManager);
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
        const auto currentValue = node->getCurrentValue();
        
        if (currentValue.get() != currentValue)
        {
            if (undoManager != nullptr)
            {
                undoManager->beginNewTransaction();
                undoManager->setCurrentTransactionName (TRANS ("Changed") + " " + node->longName);
            }
            
            currentValue.setValue (currentValue, undoManager);
        }
        
        /* Default value */
        const auto currentDefault = node->getDefaultValue();
        
        if (currentDefaultValue.get() != currentDefault)
        {
            if (undoManager != nullptr)
            {
                undoManager->beginNewTransaction();
                undoManager->setCurrentTransactionName (TRANS ("Changed default value of") + " " + node->longName);
            }
            
            currentDefaultValue.setValue (currentDefault, undoManager);
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
        const auto currentValue = node->getCurrentValue();
        
        if (currentValue.get() != currentValue)
        {
            if (undoManager != nullptr)
            {
                undoManager->beginNewTransaction();
                undoManager->setCurrentTransactionName (TRANS ("Changed") + " " + node->longName);
            }
            
            currentValue.setValue (currentValue, undoManager);
        }
        
        /* Default value */
        const auto currentDefault = node->getDefaultValue();
        
        if (currentDefaultValue.get() != currentDefault)
        {
            if (undoManager != nullptr)
            {
                undoManager->beginNewTransaction();
                undoManager->setCurrentTransactionName (TRANS ("Changed default value of") + " " + node->longName);
            }
            
            currentDefaultValue.setValue (currentDefault, undoManager);
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
        const auto currentValue = node->getCurrentValue();
        
        if (currentValue.get() != currentValue)
        {
            if (undoManager != nullptr)
            {
                undoManager->beginNewTransaction();
                undoManager->setCurrentTransactionName (TRANS ("Changed") + " " + node->longName);
            }
            
            currentValue.setValue (currentValue, undoManager);
        }
        
        /* Default value */
        const auto currentDefault = node->getDefaultValue();
        
        if (currentDefaultValue.get() != currentDefault)
        {
            if (undoManager != nullptr)
            {
                undoManager->beginNewTransaction();
                undoManager->setCurrentTransactionName (TRANS ("Changed default value of") + " " + node->longName);
            }
            
            currentDefaultValue.setValue (currentDefault, undoManager);
        }
    }
    
private:
    BoolValueTreeNode* const node;
    juce::ValueTree tree;
    
    juce::CachedValue<juce::String> currentValue;
    juce::CachedValue<juce::String> currentDefaultValue;
    
    juce::UndoManager* const undoManager;
};



//==============================================================================
//==============================================================================



/* Updates the property object with changes from the ValueTree */

//class ValueTreeToPropertyNodeAttachment   :    public juce::ValueTree::Listener
//{
//public:
//    ValueTreeToPropertyNodeAttachment (bav::Parameter* paramToUse,
//                                       juce::ValueTree treeToUse,
//                                       juce::UndoManager* um = nullptr)
//      : param (paramToUse),
//        tree (treeToUse),
//        undoManager (um)
//    {
//        jassert (tree.isValid());
//
//        tree.addListener (this);
//        lastSentChangeState = false;
//
//        currentValue.referTo (tree, DefaultValueTreeIds::ParameterValue, nullptr);
//        currentDefaultValue.referTo (tree, DefaultValueTreeIds::ParameterDefaultValue, nullptr);
//        currentGesture.referTo (tree, DefaultValueTreeIds::ParameterIsChanging, nullptr);
//    }
//
//    virtual ~ValueTreeToPropertyNodeAttachment() = default;
//
//
//    void valueTreePropertyChanged (juce::ValueTree&, const juce::Identifier&) override final
//    {
//        bool needToEndGesture = false;
//
//        /* Gesture state */
//        const auto changeState = currentGesture.get();
//
//        if (changeState != lastSentChangeState)
//        {
//            lastSentChangeState = changeState;
//
//            if (changeState)
//            {
//                if (undoManager != nullptr)
//                {
//                    undoManager->beginNewTransaction();
//                    undoManager->setCurrentTransactionName (TRANS ("Changed") + " " + param->parameterNameVerbose);
//                }
//
//                param->orig()->beginChangeGesture();
//            }
//            else
//            {
//                needToEndGesture = true;
//            }
//        }
//
//        /* Current parameter value */
//        const auto value = currentValue.get();
//
//        if (value != param->getCurrentDenormalizedValue())
//            param->orig()->setValueNotifyingHost (value);
//
//        // if gesture state switched to off, we need to send that message after any value changes
//        if (needToEndGesture)
//            param->orig()->endChangeGesture();
//
//
//        /* Parameter default value */
//        const auto defaultVal = currentDefaultValue.get();
//
//        if (defaultVal != param->getNormalizedDefault())
//        {
//            if (undoManager != nullptr)
//            {
//                undoManager->beginNewTransaction();
//                undoManager->setCurrentTransactionName (TRANS ("Changed default value of") + " " + param->parameterNameVerbose);
//            }
//
//            param->setNormalizedDefault (defaultVal);
//        }
//    }
//
//
//private:
//    bav::Parameter* const param;
//    juce::ValueTree tree;
//
//    bool lastSentChangeState;
//
//    juce::CachedValue<float> currentValue;
//    juce::CachedValue<float> currentDefaultValue;
//    juce::CachedValue<bool>  currentGesture;
//
//    juce::UndoManager* const undoManager;
//};
//
//
////==============================================================================
//
//
//struct PropertyAttachment  :   PropertyNodeToValueTreeAttachment,
//                               ValueTreeToPropertyNodeAttachment
//{
//    PropertyAttachment (bav::Parameter* paramToUse,
//                        juce::ValueTree treeToUse,
//                        juce::UndoManager* um = nullptr)
//
//      : PropertyNodeToValueTreeAttachment (paramToUse, treeToUse, nullptr),
//        ValueTreeToPropertyNodeAttachment (paramToUse, treeToUse, um)  // only use one UndoManager at a time...
//    { }
//};


}  // namespace
