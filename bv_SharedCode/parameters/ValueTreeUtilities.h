
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
            createValueTreeFromParameterTree (tree, *thisGroup);
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


static inline void createTwoWayParameterValueTreeAttachments (juce::OwnedArray<bav::ParameterAttachment>& attachments,
                                                              juce::ValueTree parameterValueTree,
                                                              int totalNumParams,
                                                              std::function< bav::Parameter* (int) > findParameter,
                                                              juce::UndoManager* um = nullptr)
{
    jassert (parameterValueTree.isValid());
    
    attachments.ensureStorageAllocated (totalNumParams);
    
    for (int i = 0; i < totalNumParams; ++i)
    {
        auto parameter = findParameter (i);
        jassert (parameter != nullptr);
        
        attachments.add (new bav::ParameterAttachment (parameter,
                                                       bav::getChildTreeForParameter (parameterValueTree, parameter),
                                                       um));
    }
}


static inline void createReadOnlyParameterValueTreeAttachments (juce::OwnedArray<bav::ValueTreeToParameterAttachment>& attachments,
                                                                juce::ValueTree parameterValueTree,
                                                                int totalNumParams,
                                                                std::function< bav::Parameter* (int) > findParameter,
                                                                juce::UndoManager* um = nullptr)
{
    jassert (parameterValueTree.isValid());
    
    attachments.ensureStorageAllocated (totalNumParams);
    
    for (int i = 0; i < totalNumParams; ++i)
    {
        auto parameter = findParameter (i);
        jassert (parameter != nullptr);
        
        attachments.add (new bav::ValueTreeToParameterAttachment (parameter,
                                                                  bav::getChildTreeForParameter (parameterValueTree, parameter),
                                                                  um));
    }
}


static inline void createWriteOnlyParameterValueTreeAttachments (juce::OwnedArray<bav::ParameterToValueTreeAttachment>& attachments,
                                                                 juce::ValueTree parameterValueTree,
                                                                 int totalNumParams,
                                                                 std::function< bav::Parameter* (int) > findParameter,
                                                                 juce::UndoManager* um = nullptr)
{
    jassert (parameterValueTree.isValid());
    
    attachments.ensureStorageAllocated (totalNumParams);
    
    for (int i = 0; i < totalNumParams; ++i)
    {
        auto parameter = findParameter (i);
        jassert (parameter != nullptr);
        
        attachments.add (new bav::ParameterToValueTreeAttachment (parameter,
                                                                  bav::getChildTreeForParameter (parameterValueTree, parameter),
                                                                  um));
    }
}


//==============================================================================
//==============================================================================

static inline juce::var valueTreeToVar (const juce::ValueTree& v)
{
    auto obj = new juce::DynamicObject();
    
    obj->setProperty ("_name", v.getType().toString());
    
    juce::Array<juce::var> children;
    
    for (auto c : v)
        children.add (valueTreeToVar (c));
    
    if (children.size() > 0)
        obj->setProperty ("_children", children);
    
    for (int i = 0; i < v.getNumProperties(); i++)
    {
        auto name = v.getPropertyName (i).toString();
        auto val  = v.getProperty (name, {});
        
        if (auto mb = val.getBinaryData())
        {
            obj->setProperty ("base64:" + name, mb->toBase64Encoding());
        }
        else
        {
            // These types can't be stored as JSON!
            jassert (! val.isObject());
            jassert (! val.isMethod());
            jassert (! val.isArray());
            
            obj->setProperty (name, val.toString());
        }
    }
    
    return juce::var (obj);
}


static juce::String valueTreeToJSON (const juce::ValueTree& v)
{
    auto obj = valueTreeToVar (v);
    return juce::JSON::toString (obj);
}

//==============================================================================
//==============================================================================

static inline juce::ValueTree valueTreefromVar (const juce::var& obj)
{
    if (auto dobj = obj.getDynamicObject())
    {
        juce::ValueTree v (dobj->getProperty ("_name").toString());
        
        auto c = dobj->getProperty ("_children");
        
        if (c.isArray())
            for (auto& child : *c.getArray())
                v.addChild (valueTreefromVar (child), -1, nullptr);
        
        auto properties = dobj->getProperties();
        
        for (auto itr : properties)
        {
            auto name = itr.name.toString();
            
            if (name == "_name" || name == "_children")
                continue;
            
            if (name.startsWith ("base64:"))
            {
                juce::MemoryBlock mb;
                if (mb.fromBase64Encoding (itr.value.toString()))
                    v.setProperty (name.substring (7), juce::var (mb), nullptr);
            }
            else
            {
                v.setProperty (name, juce::var (itr.value), nullptr);
            }
        }
        
        return v;
    }
    return {};
}


inline juce::ValueTree valueTreeFromJSON (const juce::String& jsonText)
{
    juce::var obj = juce::JSON::parse (jsonText);
    
    if (obj.isObject())
        return valueTreefromVar (obj);
    
    return {};
}


}  // namespace
