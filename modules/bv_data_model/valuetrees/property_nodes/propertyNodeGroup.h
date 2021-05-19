
namespace bav
{


struct NonParamValueTreeNodeGroup
{
    //==============================================================================
    // A node of a NonParamValueTreeNodeGroup. This can contain either another NonParamValueTreeNodeGroup or a kind of NonParamValueTreeNode
    
    struct NonParamValueTreeNodeGroupNode
    {
        NonParamValueTreeNodeGroupNode (NonParamValueTreeNodeGroupNode&& other)
            : group (std::move (other.group)), node (std::move (other.node))
        { }
        
        ~NonParamValueTreeNodeGroupNode() = default;
        
        //
        
        NonParamValueTreeNodeGroup* getGroup() const { return group.get(); }
        NonParamValueTreeNode* getNode() const { return node.get(); }
        
    private:
        NonParamValueTreeNodeGroupNode (std::unique_ptr<NonParamValueTreeNodeGroup> group_)
            : group (std::move (group_))
        { }
        
        NonParamValueTreeNodeGroupNode (std::unique_ptr<NonParamValueTreeNode> node_)
            : node (std::move (node_))
        { }
        
        //
        
        std::unique_ptr<NonParamValueTreeNodeGroup> group;
        std::unique_ptr<NonParamValueTreeNode> node;
        
        friend struct NonParamValueTreeNodeGroup;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NonParamValueTreeNodeGroupNode)
    };
    
    //==============================================================================
    
    NonParamValueTreeNodeGroup (const juce::String& groupName)
    : name (TRANS (groupName))
    { }
    
    template <typename NodeOrSubgroup>
    NonParamValueTreeNodeGroup (const juce::String& groupName,
                                std::unique_ptr<NodeOrSubgroup> child)
    : NonParamValueTreeNodeGroup (groupName)
    {
        addChild (std::move (child));
    }
    
    template <typename NodeOrSubgroup, typename... Args>
    NonParamValueTreeNodeGroup (const juce::String& groupName,
                                std::unique_ptr<NodeOrSubgroup> firstChild, Args&&... remainingChildren)
    : NonParamValueTreeNodeGroup (groupName, std::move (firstChild))
    {
        addChild (std::forward<Args> (remainingChildren)...);
    }
    
    //
    
    const NonParamValueTreeNodeGroupNode* const* begin() const noexcept
    {
        return const_cast<const NonParamValueTreeNodeGroupNode**> (children.begin());
    }
    
    const NonParamValueTreeNodeGroupNode* const* end() const noexcept
    {
        return const_cast<const NonParamValueTreeNodeGroupNode**> (children.end());
    }
    
    
    template <typename NodeOrSubgroup>
    void addChild (std::unique_ptr<NodeOrSubgroup> child)
    {
        /* If you get a compiler error here, then you are attempting to add a child that is neither a NonParamValueTreeNodeGroup or a NonParamValueTreeNode. */
        append (std::move (child));
    }
    
    template <typename NodeOrSubgroup, typename... Args>
    void addChild (std::unique_ptr<NodeOrSubgroup> firstChild, Args&&... remainingChildren)
    {
        addChild (std::move (firstChild));
        addChild (std::forward<Args> (remainingChildren)...);
    }
    
    
    const juce::String name;
    
    //
    
private:
    void append (std::unique_ptr<NonParamValueTreeNodeGroup> group)
    {
        children.add (new NonParamValueTreeNodeGroupNode (std::move (group)));
    }
    
    void append (std::unique_ptr<NonParamValueTreeNode> node)
    {
        children.add (new NonParamValueTreeNodeGroupNode (std::move (node)));
    }
    
    //
    
    juce::OwnedArray <NonParamValueTreeNodeGroupNode> children;
};


}  // namespace
