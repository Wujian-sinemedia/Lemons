#pragma once

#include <juce_data_structures/juce_data_structures.h>
#include <lemons_core/lemons_core.h>

namespace lemons
{
struct SerializableData;


struct TreeReflector
{
    explicit TreeReflector (const ValueTree& treeToUse);

    virtual bool isLoading() const = 0;
    bool         isSaving() const;

    ValueTree& getRawDataTree();

    template < typename Type >
    void add (const String& propertyName, Type& object);

    template < typename Type >
    void addLambdaSet (const String& propertyName,
                       std::function< Type() >
                           saveToTree,
                       std::function< void (Type&) >
                           loadFromTree);

private:
    template < typename Type >
    void load (const String& propertyName, Type& object);

    template < typename Type >
    void save (const String& propertyName, Type& object);

    template < class ContainerType >
    void loadContainer (const String& propertyName, ContainerType& container);

    template < class ContainerType >
    void saveContainer (const String& propertyName, ContainerType& container);

    void loadDataChild (const String& propertyName, SerializableData& data) const;
    void saveDataChild (const String& propertyName, SerializableData& data);

    ValueTree tree;
};

struct TreeLoader : TreeReflector
{
    using TreeReflector::TreeReflector;

    bool isLoading() const final;
};

struct TreeSaver : TreeReflector
{
    using TreeReflector::TreeReflector;

    bool isLoading() const final;
};

}  // namespace lemons

#include "detail/detail.h"
