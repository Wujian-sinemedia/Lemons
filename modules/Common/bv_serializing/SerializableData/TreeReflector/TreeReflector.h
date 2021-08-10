#pragma once

namespace bav
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

}  // namespace bav

#include "detail/detail.h"
