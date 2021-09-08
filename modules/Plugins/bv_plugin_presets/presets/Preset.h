#pragma once

#include <bv_serializing/bv_serializing.h>

namespace bav::plugin
{
struct Preset : SerializableData
{
    String name, category, author, description;

    ValueTree state {name};

    Version version;

private:
    void serialize (TreeReflector& ref) final;
};

}  // namespace bav::plugin
