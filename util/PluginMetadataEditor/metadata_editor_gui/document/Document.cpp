namespace lemons::PluginMetadataEditor
{

namespace documentVT
{
static constexpr auto treeType = "PluginMetadataEditorDocument";
static constexpr auto projectNameProp = "ProjectName";
}

ValueTree Document::toValueTree() const
{
    using namespace documentVT;
    
    ValueTree tree { treeType };
    
    tree.appendChild (metadata.toValueTree(), nullptr);
    
    tree.setProperty (projectNameProp, projectName, nullptr);
    
    return tree;
}

Document Document::fromValueTree (const ValueTree& tree)
{
    using namespace documentVT;
    
    Document document;
    
    if (! tree.hasType (treeType))
        return document;
    
    const auto child = tree.getChildWithName (plugin::PluginMetadata::valueTreeType);
    
    if (child.isValid())
        document.metadata = plugin::PluginMetadata::fromValueTree (child);
    
    if (tree.hasProperty (projectNameProp))
        document.projectName = tree.getProperty (projectNameProp).toString();
    
    return document;
}


namespace files
{

template <FileType Type>
Document loadDocument (const File& file)
{
    return Document::fromValueTree (loadValueTree<Type> (file));
}

template Document loadDocument <FileType::XML> (const File&);
template Document loadDocument <FileType::JSON> (const File&);
template Document loadDocument <FileType::Opaque> (const File&);


template <FileType Type>
bool saveDocument (const Document& layout, const File& file)
{
    return saveValueTree<Type> (file, layout.toValueTree());
}

template bool saveDocument<FileType::XML> (const Document&, const File&);
template bool saveDocument<FileType::JSON> (const Document&, const File&);
template bool saveDocument<FileType::Opaque> (const Document&, const File&);

}

namespace binary
{

template <FileType Type>
Document getDocument (const String& filename)
{
    return Document::fromValueTree (getValueTree<Type> (filename));
}

template Document getDocument <FileType::XML> (const String&);
template Document getDocument <FileType::JSON> (const String&);
template Document getDocument <FileType::Opaque> (const String&);

}

}

