namespace lemons::plugin
{

PluginCategory::PluginCategory (const String& categoryName,
                const juce::KnownPluginList::PluginTree& folder,
                SortMethod sortMethodToUse,
                SortingCallback sortingCallback,
                SubcategoryNamingCallback subcategoryNaming)
: shouldPutInThisCategory(sortingCallback), getSubcategoryName(subcategoryNaming), sortMethod (sortMethodToUse), name(categoryName)
{
    addFromPluginTree (folder);
}

PluginCategory::SortMethod PluginCategory::getSortMethod() const noexcept
{
    return sortMethod;
}

String PluginCategory::getName() const
{
    return name;
}

const juce::OwnedArray<PluginCategory>& PluginCategory::getSubcategories() const
{
    return subcategories;
}

const juce::Array<juce::PluginDescription>& PluginCategory::getPlugins() const
{
    return plugins;
}


juce::KnownPluginList::SortMethod getSortingMethod (PluginCategory::SortMethod sortMethod)
{
    if (sortMethod == PluginCategory::SortMethod::sortByCategory)
        return juce::KnownPluginList::SortMethod::sortByCategory;
    
    if (sortMethod == PluginCategory::SortMethod::sortByManufacturer)
        return juce::KnownPluginList::SortMethod::sortByManufacturer;
    
    return juce::KnownPluginList::SortMethod::sortByFormat;
}


void PluginCategory::addFromPluginTree (const juce::KnownPluginList::PluginTree& tree)
{
    for (const auto& plugin : tree.plugins)
    {
        if (shouldPutInThisCategory (plugin, *this))
        {
            plugins.add (plugin);
            continue;
        }
        
        auto subcategory = [&]() -> PluginCategory*
        {
            const auto subcategoryName = getSubcategoryName (plugin);
            
            for (auto* subcat : subcategories)
                if (subcat->getName() == subcategoryName)
                    return subcat;
            
            auto subTree = juce::KnownPluginList::createTree (tree.plugins, getSortingMethod (sortMethod));
            
            return subcategories.add (new PluginCategory (subcategoryName,
                                                          *subTree, sortMethod,
                                                          shouldPutInThisCategory, getSubcategoryName));
        }();
        
        jassert (subcategory != nullptr);
        
        subcategory->plugins.add (plugin);
    }
    
    for (const auto* subfolder : tree.subFolders)
        addFromPluginTree (*subfolder);
}



juce::AudioPluginFormatManager& getPluginFormatManager()
{
    struct DefaultPluginFormats final : public juce::AudioPluginFormatManager
    {
        explicit DefaultPluginFormats()
        {
            addDefaultFormats();
        }
    };
    
    static DefaultPluginFormats manager;
    
    return manager;
}


std::function<String(const juce::PluginDescription&)> getSubcategoryNamingCallback (PluginCategory::SortMethod sortMethod)
{
    if (sortMethod == PluginCategory::SortMethod::sortByCategory)
    {
        return [](const juce::PluginDescription& d){ return d.category; };
    }
    
    if (sortMethod == PluginCategory::SortMethod::sortByManufacturer)
    {
        return [](const juce::PluginDescription& d){ return d.manufacturerName; };
    }
    
    return [](const juce::PluginDescription& d){ return d.pluginFormatName; };
}


std::function<bool(const juce::PluginDescription&, const PluginCategory&)> getSortingCallback (PluginCategory::SortMethod sortMethod)
{
    if (sortMethod == PluginCategory::SortMethod::sortByCategory)
    {
        return [](const juce::PluginDescription& d, const PluginCategory& c)
        {
            for (const auto& plugin : c.getPlugins())
                if (d.category != plugin.category)
                    return false;
            
            return true;
        };
    }
    
    if (sortMethod == PluginCategory::SortMethod::sortByManufacturer)
    {
        return [](const juce::PluginDescription& d, const PluginCategory& c)
        {
            for (const auto& plugin : c.getPlugins())
                if (d.manufacturerName != plugin.manufacturerName)
                    return false;
            
            return true;
        };
    }
    
    return [](const juce::PluginDescription& d, const PluginCategory& c)
    {
        for (const auto& plugin : c.getPlugins())
            if (d.pluginFormatName != plugin.pluginFormatName)
                return false;
        
        return true;
    };
}


std::unique_ptr<PluginCategory> PluginCategory::scanDirectory (juce::FileSearchPath rootDirectory,
                                                               const File& blacklistFile,
                                                               SortMethod sortMethod)
{
    juce::KnownPluginList list;
    
    for (auto* format : getPluginFormatManager().getFormats())
    {
        juce::PluginDirectoryScanner scanner { list, *format, rootDirectory, true, blacklistFile, true };
        
        while (true)
        {
            String name;
            auto res = scanner.scanNextFile (true, name);
            
            if (! res)
                break;
        }
    }
    
    auto tree = juce::KnownPluginList::createTree (list.getTypes(), getSortingMethod (sortMethod));
    
    return std::make_unique<PluginCategory> ("Plugins", *tree, sortMethod,
                                             getSortingCallback (sortMethod),
                                             getSubcategoryNamingCallback (sortMethod));
}

}

