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

namespace lemons::plugin
{

static constexpr auto programVTtype = "Program";
static constexpr auto nameProperty  = "Name";

ValueTree Program::saveToValueTree() const
{
	ValueTree tree { programVTtype };

	tree.appendChild (state, nullptr);

	tree.setProperty (nameProperty, name, nullptr);

	return tree;
}

Program Program::fromValueTree (const ValueTree& tree)
{
	Program program;

	if (tree.hasType (programVTtype))
	{
		program.name  = tree.getProperty (nameProperty);
		program.state = tree.getChild (0);
	}
	else
	{
		jassertfalse;
	}

	return program;
}

/*--------------------------------------------------------------------------------------------------------------*/

Program* ProgramManager::getProgramAtIndex (int index)
{
	if (index >= programs.size())
		return nullptr;

	return &programs.getReference (index);
}

const Program* ProgramManager::getProgramAtIndex (int index) const
{
	if (index >= programs.size())
		return nullptr;

	return &programs.getReference (index);
}

int ProgramManager::numPrograms() const noexcept
{
	return programs.size();
}

ValueTree ProgramManager::saveAllToValueTree() const
{
	ValueTree tree { valueTreeType };

	for (const auto& program : programs)
		tree.appendChild (program.saveToValueTree(), nullptr);

	return tree;
}

void ProgramManager::restoreAllFromValueTree (const ValueTree& tree)
{
	programs.clearQuick();

	if (! tree.hasType (valueTreeType))
		return;

	for (int i = 0; i < tree.getNumChildren(); ++i)
		programs.add (Program::fromValueTree (tree.getChild (i)));
}

}  // namespace lemons::plugin
