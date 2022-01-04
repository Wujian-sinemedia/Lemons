#pragma once

namespace lemons::plugin
{

struct PluginCategory
{
    enum class SortMethod
    {
        sortByCategory,
        sortByManufacturer,
        sortByFormat
    };
    
    using SortingCallback = std::function<bool(const juce::PluginDescription&, const PluginCategory&)>;
    
    using SubcategoryNamingCallback = std::function<String(const juce::PluginDescription&)>;
    
    explicit PluginCategory (const String& categoryName,
                                      const juce::KnownPluginList::PluginTree& folder,
                                      SortMethod sortMethodToUse,
                                      SortingCallback sortingCallback,
                                      SubcategoryNamingCallback subcategoryNaming);
    
    [[nodiscard]] static std::unique_ptr<PluginCategory> scanDirectory (juce::FileSearchPath rootDirectory,
                                                                        const File& blacklistFile,
                                                                        SortMethod sortMethod);
    
    [[nodiscard]] SortMethod getSortMethod() const noexcept;
    
    [[nodiscard]] String getName() const;
    
    [[nodiscard]] const juce::OwnedArray<PluginCategory>& getSubcategories() const;
    
    [[nodiscard]] const juce::Array<juce::PluginDescription>& getPlugins() const;
    
private:
    
    void addFromPluginTree (const juce::KnownPluginList::PluginTree& tree);
    
    SortingCallback shouldPutInThisCategory;
    
    SubcategoryNamingCallback getSubcategoryName;
    
    SortMethod sortMethod;
    
    String name;
    
    juce::Array<juce::PluginDescription> plugins;
    
    juce::OwnedArray<PluginCategory> subcategories;
};


}
