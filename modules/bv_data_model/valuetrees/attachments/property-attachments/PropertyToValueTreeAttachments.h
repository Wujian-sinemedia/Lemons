
namespace bav
{
/* Updates the value tree with changes from the property object */

class IntNodeToValueTreeAttachment : public NonParamValueTreeNode::Listener,
                                     private juce::AsyncUpdater
{
public:
    IntNodeToValueTreeAttachment (IntValueTreeNode*  intNode,
                                  juce::ValueTree    treeToUse,
                                  juce::UndoManager* um = nullptr)
        : node (intNode)
        , tree (treeToUse)
        , undoManager (um)
    {
        jassert (tree.isValid());

        node->addListener (this);

        currentValue.referTo (tree, DefaultValueTreeIds::NonParameterValue, nullptr);
        currentDefaultValue.referTo (
            tree, DefaultValueTreeIds::NonParameterDefaultValue, nullptr);
    }

    virtual ~IntNodeToValueTreeAttachment() override
    {
        cancelPendingUpdate();
        node->removeListener (this);
    }

    void propertyValueChanged (const juce::String&) override final
    {
        if (juce::MessageManager::getInstance()->isThisTheMessageThread())
        {
            cancelPendingUpdate();
            handleAsyncUpdate();
        }
        else
        {
            triggerAsyncUpdate();
        }
    }

    void propertyDefaultValueChanged (const juce::String&) override final
    {
        if (juce::MessageManager::getInstance()->isThisTheMessageThread())
        {
            cancelPendingUpdate();
            handleAsyncUpdate();
        }
        else
        {
            triggerAsyncUpdate();
        }
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
                undoManager->setCurrentTransactionName (TRANS ("Changed") + " "
                                                        + node->longName);
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
                undoManager->setCurrentTransactionName (
                    TRANS ("Changed default value of") + " " + node->longName);
            }

            currentDefaultValue.setValue (def, undoManager);
        }
    }

private:
    IntValueTreeNode* const node;
    juce::ValueTree         tree;

    juce::CachedValue< int > currentValue;
    juce::CachedValue< int > currentDefaultValue;

    juce::UndoManager* const undoManager;
};


//==============================================================================
//==============================================================================


class FloatNodeToValueTreeAttachment : public NonParamValueTreeNode::Listener,
                                       private juce::AsyncUpdater
{
public:
    FloatNodeToValueTreeAttachment (FloatValueTreeNode* intNode,
                                    juce::ValueTree     treeToUse,
                                    juce::UndoManager*  um = nullptr)
        : node (intNode)
        , tree (treeToUse)
        , undoManager (um)
    {
        jassert (tree.isValid());

        node->addListener (this);

        currentValue.referTo (tree, DefaultValueTreeIds::NonParameterValue, nullptr);
        currentDefaultValue.referTo (
            tree, DefaultValueTreeIds::NonParameterDefaultValue, nullptr);
    }

    virtual ~FloatNodeToValueTreeAttachment() override
    {
        cancelPendingUpdate();
        node->removeListener (this);
    }

    void propertyValueChanged (const juce::String&) override final
    {
        if (juce::MessageManager::getInstance()->isThisTheMessageThread())
        {
            cancelPendingUpdate();
            handleAsyncUpdate();
        }
        else
        {
            triggerAsyncUpdate();
        }
    }

    void propertyDefaultValueChanged (const juce::String&) override final
    {
        if (juce::MessageManager::getInstance()->isThisTheMessageThread())
        {
            cancelPendingUpdate();
            handleAsyncUpdate();
        }
        else
        {
            triggerAsyncUpdate();
        }
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
                undoManager->setCurrentTransactionName (TRANS ("Changed") + " "
                                                        + node->longName);
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
                undoManager->setCurrentTransactionName (
                    TRANS ("Changed default value of") + " " + node->longName);
            }

            currentDefaultValue.setValue (def, undoManager);
        }
    }

private:
    FloatValueTreeNode* const node;
    juce::ValueTree           tree;

    juce::CachedValue< float > currentValue;
    juce::CachedValue< float > currentDefaultValue;

    juce::UndoManager* const undoManager;
};


//==============================================================================
//==============================================================================


class BoolNodeToValueTreeAttachment : public NonParamValueTreeNode::Listener,
                                      private juce::AsyncUpdater
{
public:
    BoolNodeToValueTreeAttachment (BoolValueTreeNode* intNode,
                                   juce::ValueTree    treeToUse,
                                   juce::UndoManager* um = nullptr)
        : node (intNode)
        , tree (treeToUse)
        , undoManager (um)
    {
        jassert (tree.isValid());

        node->addListener (this);

        currentValue.referTo (tree, DefaultValueTreeIds::NonParameterValue, nullptr);
        currentDefaultValue.referTo (
            tree, DefaultValueTreeIds::NonParameterDefaultValue, nullptr);
    }

    virtual ~BoolNodeToValueTreeAttachment() override
    {
        cancelPendingUpdate();
        node->removeListener (this);
    }

    void propertyValueChanged (const juce::String&) override final
    {
        if (juce::MessageManager::getInstance()->isThisTheMessageThread())
        {
            cancelPendingUpdate();
            handleAsyncUpdate();
        }
        else
        {
            triggerAsyncUpdate();
        }
    }

    void propertyDefaultValueChanged (const juce::String&) override final
    {
        if (juce::MessageManager::getInstance()->isThisTheMessageThread())
        {
            cancelPendingUpdate();
            handleAsyncUpdate();
        }
        else
        {
            triggerAsyncUpdate();
        }
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
                undoManager->setCurrentTransactionName (TRANS ("Changed") + " "
                                                        + node->longName);
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
                undoManager->setCurrentTransactionName (
                    TRANS ("Changed default value of") + " " + node->longName);
            }

            currentDefaultValue.setValue (def, undoManager);
        }
    }

private:
    BoolValueTreeNode* const node;
    juce::ValueTree          tree;

    juce::CachedValue< bool > currentValue;
    juce::CachedValue< bool > currentDefaultValue;

    juce::UndoManager* const undoManager;
};


//==============================================================================
//==============================================================================


class StringNodeToValueTreeAttachment : public NonParamValueTreeNode::Listener,
                                        private juce::AsyncUpdater
{
public:
    StringNodeToValueTreeAttachment (StringValueTreeNode* intNode,
                                     juce::ValueTree      treeToUse,
                                     juce::UndoManager*   um = nullptr)
        : node (intNode)
        , tree (treeToUse)
        , undoManager (um)
    {
        jassert (tree.isValid());

        node->addListener (this);

        currentValue.referTo (tree, DefaultValueTreeIds::NonParameterValue, nullptr);
        currentDefaultValue.referTo (
            tree, DefaultValueTreeIds::NonParameterDefaultValue, nullptr);
    }

    virtual ~StringNodeToValueTreeAttachment() override
    {
        cancelPendingUpdate();
        node->removeListener (this);
    }

    void propertyValueChanged (const juce::String&) override final
    {
        if (juce::MessageManager::getInstance()->isThisTheMessageThread())
        {
            cancelPendingUpdate();
            handleAsyncUpdate();
        }
        else
        {
            triggerAsyncUpdate();
        }
    }

    void propertyDefaultValueChanged (const juce::String&) override final
    {
        if (juce::MessageManager::getInstance()->isThisTheMessageThread())
        {
            cancelPendingUpdate();
            handleAsyncUpdate();
        }
        else
        {
            triggerAsyncUpdate();
        }
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
                undoManager->setCurrentTransactionName (TRANS ("Changed") + " "
                                                        + node->longName);
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
                undoManager->setCurrentTransactionName (
                    TRANS ("Changed default value of") + " " + node->longName);
            }

            currentDefaultValue.setValue (def, undoManager);
        }
    }

private:
    StringValueTreeNode* const node;
    juce::ValueTree            tree;

    juce::CachedValue< juce::String > currentValue;
    juce::CachedValue< juce::String > currentDefaultValue;

    juce::UndoManager* const undoManager;
};


} // namespace bav
