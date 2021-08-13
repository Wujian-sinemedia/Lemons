#pragma once

namespace bav::plugin
{
struct Preset : SerializableData
{
    void serialize (TreeReflector& ref) final;

    String name, category, author, description;

    ValueTree state {name};

    Version version;
};

}  // namespace bav::plugin
