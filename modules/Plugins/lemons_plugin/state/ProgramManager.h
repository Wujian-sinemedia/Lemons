/*
 ======================================================================================
 
 ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 
 This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 
 ======================================================================================
 */

#pragma once

namespace lemons::plugin
{

struct Program
{
    [[nodiscard]] ValueTree saveToValueTree() const;
    
    [[nodiscard]] static Program fromValueTree (const ValueTree& tree);
    
    ValueTree state;
    String    name;
};


struct ProgramManager final
{
    [[nodiscard]] Program* getProgramAtIndex (int index);
    
    [[nodiscard]] const Program* getProgramAtIndex (int index) const;
    
    [[nodiscard]] int numPrograms() const noexcept;
    
    [[nodiscard]] ValueTree saveAllToValueTree() const;
    
    void restoreAllFromValueTree (const ValueTree& tree);
    
    
    static constexpr auto valueTreeType = "Programs";
    
private:
    juce::Array<Program> programs;
};

}
