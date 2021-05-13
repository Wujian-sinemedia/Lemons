
namespace bav
{


/* Updates the ValueTree with changes from the property object */
template<typename NodeType>
class PropertyNodeToValueTreeAttachment   :     public  NonParamValueTreeNode::Listener,
                                                private juce::AsyncUpdater
{
public:
    PropertyNodeToValueTreeAttachment (NonParamValueTreeNode* nodeToUse,
                                       juce::ValueTree treeToUse,
                                       juce::UndoManager* um = nullptr)
      : node (nodeToUse),
        tree (treeToUse),
        undoManager (um)
    {
        jassert (tree.isValid());
        
        node->addListener (this);
        
        currentValue.referTo (tree, DefaultValueTreeIds::NonParameterValue, nullptr);
        currentDefaultValue.referTo (tree, DefaultValueTreeIds::NonParameterDefaultValue, nullptr);
    }
    
    virtual ~PropertyNodeToValueTreeAttachment() override
    {
        cancelPendingUpdate();
        node->removeListener (this);
    }
    
    
    void propertyValueChanged (const juce::String& currentValueAsString) override final
    {
        if (juce::MessageManager::getInstance()->isThisTheMessageThread())
            handleUpdateNowIfNeeded();
        else
            triggerAsyncUpdate();
    }
    
    void propertyDefaultValueChanged (const juce::String& currentValueAsString) override final
    {
        if (juce::MessageManager::getInstance()->isThisTheMessageThread())
            handleUpdateNowIfNeeded();
        else
            triggerAsyncUpdate();
    }
    
    void handleAsyncUpdate() override final
    {
        updateValueTree();
    }
    
    virtual void updateValueTree() = 0;
    
    
protected:
    NonParamValueTreeNode* const node;
    juce::ValueTree tree;
    
    juce::CachedValue<NodeType> currentValue;
    juce::CachedValue<NodeType> currentDefaultValue;
    
    juce::UndoManager* const undoManager;
};

template class PropertyNodeToValueTreeAttachment<int>;
template class PropertyNodeToValueTreeAttachment<float>;
template class PropertyNodeToValueTreeAttachment<bool>;
template class PropertyNodeToValueTreeAttachment<juce::String>;

//---------------------------------------------------------------

struct IntNodeToValueTreeAttachment    :    PropertyNodeToValueTreeAttachment<int>
{
    using Base = PropertyNodeToValueTreeAttachment<int>;
    
    IntNodeToValueTreeAttachment (IntValueTreeNode* intNode,
                                  juce::ValueTree treeToUse,
                                  juce::UndoManager* um = nullptr)
        : Base (intNode, treeToUse, um),
          node (intNode)
    { }
    
    void updateValueTree() override final
    {
        /* Current value */
        const auto currentValue = node->getCurrentValue();
        
        if (Base::currentValue.get() != currentValue)
        {
            if (Base::undoManager != nullptr)
            {
                Base::undoManager->beginNewTransaction();
                Base::undoManager->setCurrentTransactionName (TRANS ("Changed") + " " + node->longName);
            }
            
            Base::currentValue.setValue (currentValue, Base::undoManager);
        }
        
        /* Default value */
        const auto currentDefault = node->getDefaultValue();
        
        if (Base::currentDefaultValue.get() != currentDefault)
        {
            if (Base::undoManager != nullptr)
            {
                Base::undoManager->beginNewTransaction();
                Base::undoManager->setCurrentTransactionName (TRANS ("Changed default value of") + " " + node->longName);
            }
            
            Base::currentDefaultValue.setValue (currentDefault, undoManager);
        }
    }
    
private:
    IntValueTreeNode* const node;
};

struct FloatNodeToValueTreeAttachment  :    PropertyNodeToValueTreeAttachment<float>
{
    using Base = PropertyNodeToValueTreeAttachment<float>;
    
    FloatNodeToValueTreeAttachment (FloatValueTreeNode* floatNode,
                                    juce::ValueTree treeToUse,
                                    juce::UndoManager* um = nullptr)
      : Base (floatNode, treeToUse, um),
        node (floatNode)
    { }
    
    void updateValueTree() override final
    {
        /* Current value */
        const auto currentValue = node->getCurrentValue();
        
        if (Base::currentValue.get() != currentValue)
        {
            if (Base::undoManager != nullptr)
            {
                Base::undoManager->beginNewTransaction();
                Base::undoManager->setCurrentTransactionName (TRANS ("Changed") + " " + node->longName);
            }
            
            Base::currentValue.setValue (currentValue, Base::undoManager);
        }
        
        /* Default value */
        const auto currentDefault = node->getDefaultValue();
        
        if (Base::currentDefaultValue.get() != currentDefault)
        {
            if (Base::undoManager != nullptr)
            {
                Base::undoManager->beginNewTransaction();
                Base::undoManager->setCurrentTransactionName (TRANS ("Changed default value of") + " " + node->longName);
            }
            
            Base::currentDefaultValue.setValue (currentDefault, undoManager);
        }
    }
    
private:
    FloatValueTreeNode* const node;
};

struct BoolNodeToValueTreeAttachment   :    PropertyNodeToValueTreeAttachment<bool>
{
    using Base = PropertyNodeToValueTreeAttachment<bool>;
    
    BoolNodeToValueTreeAttachment (BoolValueTreeNode* boolNode,
                                   juce::ValueTree treeToUse,
                                   juce::UndoManager* um = nullptr)
      : Base (boolNode, treeToUse, um),
        node (boolNode)
    { }
    
    void updateValueTree() override final
    {
        /* Current value */
        const auto currentValue = node->getCurrentValue();
        
        if (Base::currentValue.get() != currentValue)
        {
            if (Base::undoManager != nullptr)
            {
                Base::undoManager->beginNewTransaction();
                Base::undoManager->setCurrentTransactionName (TRANS ("Changed") + " " + node->longName);
            }
            
            Base::currentValue.setValue (currentValue, Base::undoManager);
        }
        
        /* Default value */
        const auto currentDefault = node->getDefaultValue();
        
        if (Base::currentDefaultValue.get() != currentDefault)
        {
            if (Base::undoManager != nullptr)
            {
                Base::undoManager->beginNewTransaction();
                Base::undoManager->setCurrentTransactionName (TRANS ("Changed default value of") + " " + node->longName);
            }
            
            Base::currentDefaultValue.setValue (currentDefault, undoManager);
        }
    }
    
private:
    BoolValueTreeNode* const node;
};

struct StringNodeToValueTreeAttachment :    PropertyNodeToValueTreeAttachment<juce::String>
{
    using Base = PropertyNodeToValueTreeAttachment<juce::String>;
    
    StringNodeToValueTreeAttachment (StringValueTreeNode* boolNode,
                                     juce::ValueTree treeToUse,
                                     juce::UndoManager* um = nullptr)
      : Base (boolNode, treeToUse, um),
        node (boolNode)
    { }
    
    void updateValueTree() override final
    {
        /* Current value */
        const auto currentValue = node->getCurrentValue();
        
        if (Base::currentValue.get() != currentValue)
        {
            if (Base::undoManager != nullptr)
            {
                Base::undoManager->beginNewTransaction();
                Base::undoManager->setCurrentTransactionName (TRANS ("Changed") + " " + node->longName);
            }
            
            Base::currentValue.setValue (currentValue, Base::undoManager);
        }
        
        /* Default value */
        const auto currentDefault = node->getDefaultValue();
        
        if (Base::currentDefaultValue.get() != currentDefault)
        {
            if (Base::undoManager != nullptr)
            {
                Base::undoManager->beginNewTransaction();
                Base::undoManager->setCurrentTransactionName (TRANS ("Changed default value of") + " " + node->longName);
            }
            
            Base::currentDefaultValue.setValue (currentDefault, undoManager);
        }
    }
    
private:
    StringValueTreeNode* const node;
};



//==============================================================================
//==============================================================================


/* Updates the parameter object with changes from the ValueTree */
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
