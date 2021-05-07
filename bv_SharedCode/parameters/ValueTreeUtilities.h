
namespace bav
{

/* note: this only works if the underlying parameter objects in the group are derived from my Parameter base class */
static inline void createValueTreeFromParameterTree (juce::ValueTree& tree,
                                                     const juce::AudioProcessorParameterGroup& parameterTree)
{
    for (auto* node : parameterTree)
    {
        if (auto* param = node->getParameter())
        {
            if (auto* parameter = dynamic_cast<bav::Parameter*>(param))
            {
                juce::ValueTree parameterTreeNode { "Parameter" };
                
                parameterTreeNode.setProperty ("Name",  parameterNameVerbose, nullptr);
                parameterTreeNode.setProperty ("Value", parameter->getCurrentDenormalizedValue(), nullptr);
                parameterTreeNode.setProperty ("IsChanging", false, nullptr);
                
                tree.addChild (parameterTreeNode, nullptr);
            }
        }
        else if (auto* thisGroup = node->getGroup())
        {
            createValueTreeFromParameterTree (tree, *thisGroup);
        }
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
        children.add (toVar (c));
    
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
                v.addChild (fromVar (child), -1, nullptr);
        
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
